#include <gtest/gtest.h>

#include <vector>
#include <string>

#include "domain/classfile/AttributeStructures.h"
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
