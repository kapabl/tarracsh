#include <gtest/gtest.h>

#include <vector>
#include <string>
#include <tuple>

#include "domain/classfile/types/AttributeStructures.h"
#include "domain/classfile/AttributeTags.h"
#include "domain/classfile/AnnotationsParser.h"
#include "domain/classfile/constantpool/ConstantPool.h"
#include "domain/classfile/constantpool/ConstpoolStructures.h"

using namespace kapa::tarracsh::domain;
using namespace classfile;
using namespace classfile::attribute;
using namespace classfile::constantpool;
using namespace classfile::annotations;

namespace {

u2 addUtf8(const std::string &value, ConstantPool &pool) {
    const auto allocSize = sizeof(ConstantPoolRecord::utf8Info.tag) +
                           sizeof(ConstantPoolRecord::utf8Info.length) +
                           value.size() + 1;
    auto *record = static_cast<ConstantPoolRecord *>(std::malloc(allocSize));
    record->utf8Info.tag = JVM_CONSTANT_Utf8;
    record->utf8Info.length = static_cast<u2>(value.size());
    std::memcpy(record->utf8Info.bytes, value.data(), value.size());
    record->utf8Info.bytes[value.size()] = 0;
    pool.addRecord(*record, allocSize);
    std::free(record);
    return static_cast<u2>(pool.getPoolSize() - 1);
}

u2 addStringLiteral(const std::string &value, ConstantPool &pool) {
    const auto utf8Index = addUtf8(value, pool);
    ConstantPoolRecord record{};
    record.stringInfo.tag = JVM_CONSTANT_String;
    record.stringInfo.stringIndex = utf8Index;
    pool.addRecord(record);
    return static_cast<u2>(pool.getPoolSize() - 1);
}

u2 addInteger(int value, ConstantPool &pool) {
    ConstantPoolRecord record{};
    record.integerInfo.tag = JVM_CONSTANT_Integer;
    record.integerInfo.value = value;
    pool.addRecord(record);
    return static_cast<u2>(pool.getPoolSize() - 1);
}

void appendU1(std::vector<u1> &buffer, u1 value) {
    buffer.push_back(value);
}

void appendU2(std::vector<u1> &buffer, u2 value) {
    buffer.push_back(static_cast<u1>(value & 0xFF));
    buffer.push_back(static_cast<u1>((value >> 8) & 0xFF));
}

std::vector<u1> makeTargetU1(u1 value) {
    std::vector<u1> buffer;
    appendU1(buffer, value);
    return buffer;
}

std::vector<u1> makeTargetU2(u2 value) {
    std::vector<u1> buffer;
    appendU2(buffer, value);
    return buffer;
}

std::vector<u1> makeTargetPair(u1 first, u1 second) {
    std::vector<u1> buffer;
    appendU1(buffer, first);
    appendU1(buffer, second);
    return buffer;
}

std::vector<u1> makeLocalVarTarget(u2 startPC, u2 length, u2 index) {
    std::vector<u1> buffer;
    appendU2(buffer, 1); // table_length
    appendU2(buffer, startPC);
    appendU2(buffer, length);
    appendU2(buffer, index);
    return buffer;
}

std::vector<u1> makeTypeArgumentTarget(u2 offset, u2 typeArgumentIndex) {
    std::vector<u1> buffer;
    appendU2(buffer, offset);
    appendU2(buffer, typeArgumentIndex);
    return buffer;
}

void appendTypeAnnotation(std::vector<u1> &buffer,
                          u1 targetType,
                          const std::vector<u1> &targetInfo,
                          u2 typeIndex) {
    appendU1(buffer, targetType);
    buffer.insert(buffer.end(), targetInfo.begin(), targetInfo.end());
    appendU1(buffer, 0); // type path length
    appendU2(buffer, typeIndex);
    appendU2(buffer, 0); // num_element_value_pairs
}

AttributeInfo makeAttribute(ConstantPool &pool,
                            const std::string &name,
                            std::vector<u1> info,
                            AttributeOwner owner) {
    AttributeInfo attribute{};
    attribute.nameIndex = addUtf8(name, pool);
    attribute.length = static_cast<u4>(info.size());
    attribute.owner = owner;
    attribute.info = std::move(info);
    return attribute;
}

std::string parseAnnotations(const ConstantPool &pool,
                             const AttributeInfo &attribute,
                             std::string (AnnotationsParser::*method)() const) {
    reader::VectorReader reader(attribute.info, true);
    AnnotationsParser parser(pool, attribute, reader);
    return (parser.*method)();
}

} // namespace

TEST(AnnotationsParserTests, RendersRuntimeVisibleAnnotations) {
    ConstantPool pool;
    const auto annotationType = addUtf8("Lpkg/MyAnno;", pool);
    const auto elementName = addUtf8("value", pool);
    const auto literal = addStringLiteral("hello", pool);

    std::vector<u1> info;
    appendU2(info, 1); // num_annotations
    appendU2(info, annotationType);
    appendU2(info, 1); // num_element_value_pairs
    appendU2(info, elementName);
    appendU1(info, JVM_SIGNATURE_STRING);
    appendU2(info, literal);

    auto attribute = makeAttribute(pool, RUNTIME_VISIBLE_ANNOTATIONS, info, AttributeOwner::ClassFile);
    const auto output = parseAnnotations(pool, attribute, &AnnotationsParser::toStringRuntimeAnnotations);
    EXPECT_NE(output.find("pkg.MyAnno"), std::string::npos);
    EXPECT_NE(output.find("hello"), std::string::npos);
}

TEST(AnnotationsParserTests, RendersRuntimeParameterAnnotations) {
    ConstantPool pool;
    const auto annotationType = addUtf8("Lpkg/Param;", pool);
    const auto elementName = addUtf8("flag", pool);
    const auto boolValue = addInteger(1, pool);

    std::vector<u1> info;
    appendU1(info, 1); // parameter count
    appendU2(info, 1); // num annotations for parameter 0
    appendU2(info, annotationType);
    appendU2(info, 1); // num element value pairs
    appendU2(info, elementName);
    appendU1(info, JVM_SIGNATURE_BOOLEAN);
    appendU2(info, boolValue);

    auto attribute = makeAttribute(pool,
                                   RUNTIME_VISIBLE_PARAMETER_ANNOTATIONS,
                                   info,
                                   AttributeOwner::Method);

    const auto output = parseAnnotations(
        pool, attribute, &AnnotationsParser::toStringRuntimeParameterAnnotations);
    EXPECT_NE(output.find("pkg.Param"), std::string::npos);
    EXPECT_NE(output.find("flag=(1)"), std::string::npos);
}

TEST(AnnotationsParserTests, RendersRuntimeTypeAnnotations) {
    ConstantPool pool;
    const auto annotationType = addUtf8("Lpkg/TypeAnno;", pool);

    std::vector<u1> info;
    appendU2(info, 1); // num annotations
    appendU1(info, 0x13); // target_type (field)
    appendU1(info, 0); // type_path length
    appendU2(info, annotationType);
    appendU2(info, 0); // num element value pairs

    auto attribute = makeAttribute(pool,
                                   RUNTIME_INVISIBLE_TYPE_ANNOTATIONS,
                                   info,
                                   AttributeOwner::Field);
    const auto output = parseAnnotations(pool,
                                         attribute,
                                         &AnnotationsParser::toStringRuntimeTypeAnnotations);
    EXPECT_NE(output.find("pkg.TypeAnno"), std::string::npos);
}

TEST(AnnotationsParserTests, RendersAnnotationDefault) {
    ConstantPool pool;
    const auto literal = addStringLiteral("default", pool);

    std::vector<u1> info;
    appendU1(info, JVM_SIGNATURE_STRING);
    appendU2(info, literal);

    auto attribute = makeAttribute(pool, ANNOTATION_DEFAULT, info, AttributeOwner::Method);

    reader::VectorReader reader(attribute.info, true);
    AnnotationsParser parser(pool, attribute, reader);
    const auto output = parser.toStringAnnotationDefault();
    EXPECT_NE(output.find("default"), std::string::npos);
}

TEST(AnnotationsParserTests, DescribesAllRuntimeTypeAnnotationTargets) {
    ConstantPool pool;
    std::vector<u2> typeIndices;
    std::vector<std::string> descriptors{
        "Lpkg/TParamClass;", "Lpkg/TParamMethod;", "Lpkg/TSuper;", "Lpkg/TClassBound;",
        "Lpkg/TMethodBound;", "Lpkg/TField;", "Lpkg/TField2;", "Lpkg/TField3;", "Lpkg/TFormalParam;",
        "Lpkg/TThrows;", "Lpkg/TLocal;", "Lpkg/TResource;", "Lpkg/TCatch;", "Lpkg/TInstanceof;",
        "Lpkg/TNew;", "Lpkg/TMethodNew;", "Lpkg/TMethodId;", "Lpkg/TTypeCast;", "Lpkg/TArgCtor;",
        "Lpkg/TArgMember;", "Lpkg/TArgCtorNew;", "Lpkg/TArgCtorId;"
    };
    for (const auto &descriptor : descriptors) {
        typeIndices.push_back(addUtf8(descriptor, pool));
    }

    std::vector<std::tuple<u1, std::vector<u1>, u2>> typeAnnotations{
        {0x00, makeTargetU1(2), typeIndices[0]},
        {0x01, makeTargetU1(3), typeIndices[1]},
        {0x10, makeTargetU2(7), typeIndices[2]},
        {0x11, makeTargetPair(1, 2), typeIndices[3]},
        {0x12, makeTargetPair(3, 4), typeIndices[4]},
        {0x13, {}, typeIndices[5]},
        {0x14, {}, typeIndices[6]},
        {0x15, {}, typeIndices[7]},
        {0x16, makeTargetU1(5), typeIndices[8]},
        {0x17, makeTargetU2(9), typeIndices[9]},
        {0x40, makeLocalVarTarget(10, 2, 1), typeIndices[10]},
        {0x41, makeLocalVarTarget(20, 3, 2), typeIndices[11]},
        {0x42, makeTargetU2(11), typeIndices[12]},
        {0x43, makeTargetU2(12), typeIndices[13]},
        {0x44, makeTargetU2(13), typeIndices[14]},
        {0x45, makeTargetU2(14), typeIndices[15]},
        {0x46, makeTargetU2(15), typeIndices[16]},
        {0x47, makeTypeArgumentTarget(16, 1), typeIndices[17]},
        {0x48, makeTypeArgumentTarget(17, 2), typeIndices[18]},
        {0x49, makeTypeArgumentTarget(18, 3), typeIndices[19]},
        {0x4A, makeTypeArgumentTarget(19, 4), typeIndices[20]},
        {0x4B, makeTypeArgumentTarget(20, 5), typeIndices[21]},
    };

    std::vector<u1> info;
    appendU2(info, static_cast<u2>(typeAnnotations.size()));
    for (const auto &[targetType, targetInfo, typeIndex] : typeAnnotations) {
        appendTypeAnnotation(info, targetType, targetInfo, typeIndex);
    }

    auto attribute = makeAttribute(pool,
                                   RUNTIME_VISIBLE_TYPE_ANNOTATIONS,
                                   info,
                                   AttributeOwner::ClassFile);
    const auto output = parseAnnotations(pool,
                                         attribute,
                                         &AnnotationsParser::toStringRuntimeTypeAnnotations);

    EXPECT_NE(output.find("pkg.TParamClass: index"), std::string::npos);
    EXPECT_NE(output.find("pkg.TParamMethod: index"), std::string::npos);
    EXPECT_NE(output.find("pkg.TSuper: index: 7 super-type"), std::string::npos);
    EXPECT_NE(output.find("pkg.TClassBound: index, bound"), std::string::npos);
    EXPECT_NE(output.find("pkg.TMethodBound: index, bound"), std::string::npos);
    EXPECT_NE(output.find("pkg.TField: field"), std::string::npos);
    EXPECT_NE(output.find("pkg.TField2: field"), std::string::npos);
    EXPECT_NE(output.find("pkg.TField3: field"), std::string::npos);
    EXPECT_NE(output.find("pkg.TFormalParam: param index"), std::string::npos);
    EXPECT_NE(output.find("pkg.TThrows: exception index"), std::string::npos);
    EXPECT_NE(output.find("pkg.TLocal: local var: local var locations"), std::string::npos);
    EXPECT_NE(output.find("pkg.TResource: local res var: local var locations"), std::string::npos);
    EXPECT_NE(output.find("pkg.TCatch: type exception param index"), std::string::npos);
    EXPECT_NE(output.find("pkg.TInstanceof: instanceof bytecode offset"), std::string::npos);
    EXPECT_NE(output.find("pkg.TNew: new bytecode offset"), std::string::npos);
    EXPECT_NE(output.find("pkg.TMethodNew: methodref ::new bytecode offset"), std::string::npos);
    EXPECT_NE(output.find("pkg.TMethodId: methodref ::Identifier bytecode offset"), std::string::npos);
    EXPECT_NE(output.find("pkg.TTypeCast: type-cast bytecode offset"), std::string::npos);
    EXPECT_NE(output.find("pkg.TArgCtor: arg. gen constructor bytecode offset"), std::string::npos);
    EXPECT_NE(output.find("pkg.TArgMember: arg. gen member call bytecode offset"), std::string::npos);
    EXPECT_NE(output.find("pkg.TArgCtorNew: arg. gen constructor methodref ::new bytecode offset"), std::string::npos);
    EXPECT_NE(output.find("pkg.TArgCtorId: arg. gen constructor methodref ::Identifier bytecode offset"), std::string::npos);
}

TEST(AnnotationsParserTests, HandlesComplexAnnotationElementValues) {
    ConstantPool pool;
    const auto annotationType = addUtf8("Lpkg/Composite;", pool);
    const auto nestedType = addUtf8("Lpkg/Nested;", pool);

    const auto textName = addUtf8("text", pool);
    const auto countName = addUtf8("count", pool);
    const auto enumName = addUtf8("status", pool);
    const auto className = addUtf8("clazz", pool);
    const auto nestedName = addUtf8("nested", pool);
    const auto arrayName = addUtf8("values", pool);
    const auto nestedValueName = addUtf8("value", pool);

    const auto textValue = addUtf8("hello", pool);
    const auto countValue = addInteger(42, pool);
    const auto enumType = addUtf8("Lpkg/Status;", pool);
    const auto enumConstName = addUtf8("READY", pool);
    const auto classLiteral = addUtf8("java/lang/String", pool);
    const auto nestedLiteral = addUtf8("child", pool);
    const auto arrayFirst = addUtf8("first", pool);
    const auto arraySecond = addInteger(2, pool);

    std::vector<u1> info;
    appendU2(info, 1); // num_annotations
    appendU2(info, annotationType);
    appendU2(info, 6); // num_element_value_pairs

    const auto appendConstValue = [&](u2 nameIndex, u1 tag, u2 valueIndex) {
        appendU2(info, nameIndex);
        appendU1(info, tag);
        appendU2(info, valueIndex);
    };

    appendConstValue(textName, JVM_SIGNATURE_STRING, textValue);
    appendConstValue(countName, JVM_SIGNATURE_INT, countValue);

    appendU2(info, enumName);
    appendU1(info, JVM_SIGNATURE_ENUM2);
    appendU2(info, enumType);
    appendU2(info, enumConstName);

    appendConstValue(className, JVM_SIGNATURE_CLASS2, classLiteral);

    appendU2(info, nestedName);
    appendU1(info, JVM_SIGNATURE_ANNOTATION);
    appendU2(info, nestedType);
    appendU2(info, 1); // nested pairs
    appendConstValue(nestedValueName, JVM_SIGNATURE_STRING, nestedLiteral);

    appendU2(info, arrayName);
    appendU1(info, JVM_SIGNATURE_ARRAY);
    appendU2(info, 2); // array length
    appendU1(info, JVM_SIGNATURE_STRING);
    appendU2(info, arrayFirst);
    appendU1(info, JVM_SIGNATURE_INT);
    appendU2(info, arraySecond);

    auto attribute = makeAttribute(pool,
                                   RUNTIME_VISIBLE_ANNOTATIONS,
                                   info,
                                   AttributeOwner::ClassFile);

    const auto output = parseAnnotations(pool, attribute, &AnnotationsParser::toStringRuntimeAnnotations);
    EXPECT_NE(output.find("text=(\"hello\")"), std::string::npos);
    EXPECT_NE(output.find("count=(42)"), std::string::npos);
    EXPECT_NE(output.find("status=(Lpkg/Status;=\"READY\")"), std::string::npos);
    EXPECT_NE(output.find("clazz=(java/lang/String)"), std::string::npos);
    EXPECT_NE(output.find("nested=(pkg.Nested: (value=(\"child\")))"), std::string::npos);
    EXPECT_NE(output.find("values=(\"first\",2)"), std::string::npos);
}
