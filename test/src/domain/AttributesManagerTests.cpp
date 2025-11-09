#include <gtest/gtest.h>

#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

#include "domain/classfile/types/AttributeStructures.h"
#include "domain/classfile/AttributesManager.h"
#include "domain/classfile/ClassFileConstants.h"
#include "domain/classfile/AttributeTags.h"
#include "domain/classfile/constantpool/ConstantPool.h"
#include "domain/classfile/constantpool/ConstpoolStructures.h"

using namespace kapa::tarracsh::domain;
using namespace classfile;
using namespace classfile::attribute;
using namespace classfile::constantpool;

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

u2 addInteger(int value, ConstantPool &pool) {
    ConstantPoolRecord record{};
    record.integerInfo.tag = JVM_CONSTANT_Integer;
    record.integerInfo.value = value;
    pool.addRecord(record);
    return static_cast<u2>(pool.getPoolSize() - 1);
}

u2 addClassInfo(const std::string &name, ConstantPool &pool) {
    const auto utf8Index = addUtf8(name, pool);
    ConstantPoolRecord record{};
    record.classInfo.tag = JVM_CONSTANT_Class;
    record.classInfo.nameIndex = utf8Index;
    pool.addRecord(record);
    return static_cast<u2>(pool.getPoolSize() - 1);
}

void appendU2(std::vector<u1> &buffer, u2 value) {
    buffer.push_back(static_cast<u1>(value & 0xFF));
    buffer.push_back(static_cast<u1>((value >> 8) & 0xFF));
}

void appendU1(std::vector<u1> &buffer, u1 value) {
    buffer.push_back(value);
}

void appendU4(std::vector<u1> &buffer, u4 value) {
    buffer.push_back(static_cast<u1>(value & 0xFF));
    buffer.push_back(static_cast<u1>((value >> 8) & 0xFF));
    buffer.push_back(static_cast<u1>((value >> 16) & 0xFF));
    buffer.push_back(static_cast<u1>((value >> 24) & 0xFF));
}

u2 addNameAndType(const std::string &name, const std::string &descriptor, ConstantPool &pool) {
    ConstantPoolRecord record{};
    record.nameAndTypeInfo.tag = JVM_CONSTANT_NameAndType;
    record.nameAndTypeInfo.nameIndex = addUtf8(name, pool);
    record.nameAndTypeInfo.descriptorIndex = addUtf8(descriptor, pool);
    pool.addRecord(record);
    return static_cast<u2>(pool.getPoolSize() - 1);
}

u2 addMethodref(const std::string &className,
                const std::string &memberName,
                const std::string &descriptor,
                ConstantPool &pool) {
    ConstantPoolRecord record{};
    record.methodrefInfo.tag = JVM_CONSTANT_Methodref;
    record.methodrefInfo.classIndex = addClassInfo(className, pool);
    record.methodrefInfo.nameAndTypeIndex = addNameAndType(memberName, descriptor, pool);
    pool.addRecord(record);
    return static_cast<u2>(pool.getPoolSize() - 1);
}

u2 addMethodHandle(u1 referenceKind, u2 referenceIndex, ConstantPool &pool) {
    ConstantPoolRecord record{};
    record.methodHandleInfo.tag = JVM_CONSTANT_MethodHandle;
    record.methodHandleInfo.referenceKind = static_cast<MethodHandleSubtypes>(referenceKind);
    record.methodHandleInfo.referenceIndex = referenceIndex;
    pool.addRecord(record);
    return static_cast<u2>(pool.getPoolSize() - 1);
}

u2 addStringLiteral(const std::string &value, ConstantPool &pool) {
    ConstantPoolRecord record{};
    record.stringInfo.tag = JVM_CONSTANT_String;
    record.stringInfo.stringIndex = addUtf8(value, pool);
    pool.addRecord(record);
    return static_cast<u2>(pool.getPoolSize() - 1);
}

attribute::AttributeInfo makeAttribute(ConstantPool &pool,
                                       const std::string &name,
                                       std::vector<u1> info,
                                       attribute::AttributeOwner owner = attribute::AttributeOwner::ClassFile) {
    attribute::AttributeInfo attribute{};
    attribute.owner = owner;
    attribute.nameIndex = addUtf8(name, pool);
    attribute.length = static_cast<u4>(info.size());
    attribute.info = std::move(info);
    return attribute;
}

std::string renderAttribute(const ConstantPool &pool, const AttributeInfo &attribute) {
    AttributesManager manager(pool);
    return manager.toString(attribute);
}

} // namespace

TEST(AttributesManagerTests, RendersConstantValueAttribute) {
    ConstantPool pool;
    const auto attributeNameIndex = addUtf8(CONSTANT_VALUE, pool);
    const auto constantIndex = addInteger(42, pool);

    AttributeInfo attribute{};
    attribute.owner = AttributeOwner::Field;
    attribute.nameIndex = attributeNameIndex;
    attribute.length = 2;
    appendU2(attribute.info, constantIndex);

    EXPECT_EQ(renderAttribute(pool, attribute), "Attr - ConstantValue: 42");
}

TEST(AttributesManagerTests, RendersConstantValueStringAttribute) {
    ConstantPool pool;
    const auto attributeNameIndex = addUtf8(CONSTANT_VALUE, pool);
    const auto stringIndex = addStringLiteral("hello", pool);

    AttributeInfo attribute{};
    attribute.owner = AttributeOwner::Field;
    attribute.nameIndex = attributeNameIndex;
    attribute.length = 2;
    appendU2(attribute.info, stringIndex);

    EXPECT_EQ(renderAttribute(pool, attribute), "Attr - ConstantValue: hello");
}

TEST(AttributesManagerTests, RendersSourceFileAttribute) {
    ConstantPool pool;
    const auto attributeNameIndex = addUtf8(SOURCE_FILE, pool);
    const auto filenameIndex = addUtf8("Sample.java", pool);

    AttributeInfo attribute{};
    attribute.owner = AttributeOwner::ClassFile;
    attribute.nameIndex = attributeNameIndex;
    attribute.length = 2;
    appendU2(attribute.info, filenameIndex);

    EXPECT_EQ(renderAttribute(pool, attribute), "Attr - SourceFile: Sample.java");
}

TEST(AttributesManagerTests, RendersInnerClassesAttribute) {
    ConstantPool pool;
    const auto attributeNameIndex = addUtf8(INNER_CLASSES, pool);
    const auto innerClassInfoIndex = addClassInfo("pkg/Outer$Inner", pool);
    const auto outerClassInfoIndex = addClassInfo("pkg/Outer", pool);
    const auto innerNameIndex = addUtf8("Inner", pool);

    AttributeInfo attribute{};
    attribute.owner = AttributeOwner::ClassFile;
    attribute.nameIndex = attributeNameIndex;
    attribute.length = 10;
    attribute.info = {};
    appendU2(attribute.info, 1); // number_of_classes
    appendU2(attribute.info, innerClassInfoIndex);
    appendU2(attribute.info, outerClassInfoIndex);
    appendU2(attribute.info, innerNameIndex);
    appendU2(attribute.info, JVM_ACC_PUBLIC | JVM_ACC_STATIC);

    const auto output = renderAttribute(pool, attribute);
    EXPECT_NE(output.find("Attr - InnerClasses"), std::string::npos);
    EXPECT_NE(output.find("outer class: pkg/Outer"), std::string::npos);
    EXPECT_NE(output.find("inner class: pkg/Outer$Inner"), std::string::npos);
    EXPECT_NE(output.find("class: Inner"), std::string::npos);
}

TEST(AttributesManagerTests, RendersSourceDebugExtensionAttribute) {
    ConstantPool pool;
    auto attribute = makeAttribute(pool,
                                   SOURCE_DEBUG_EXTENSION,
                                   {'S', 'M', 'A', 'P'});

    EXPECT_EQ(renderAttribute(pool, attribute), "Attr - SourceDebugExtension SMAP");
}

TEST(AttributesManagerTests, RendersEmptyRuntimeVisibleAnnotations) {
    ConstantPool pool;
    auto attribute = makeAttribute(pool,
                                   RUNTIME_VISIBLE_ANNOTATIONS,
                                   {0x00, 0x00});

    const auto output = renderAttribute(pool, attribute);
    EXPECT_NE(output.find("Attr - RuntimeVisibleAnnotations"), std::string::npos);
}

TEST(AttributesManagerTests, RendersNestMembersAttributeEvenWhenEmpty) {
    ConstantPool pool;
    auto attribute = makeAttribute(pool,
                                   NEST_MEMBERS,
                                   {0x00, 0x00});

    EXPECT_EQ(renderAttribute(pool, attribute), "Attr - NestMembers (empty)");
}

TEST(AttributesManagerTests, RendersNestMembersAttributeWithClasses) {
    ConstantPool pool;
    const auto innerIndex = addClassInfo("pkg/Outer$Inner", pool);
    const auto helperIndex = addClassInfo("pkg/Outer$Helper", pool);

    std::vector<u1> info;
    appendU2(info, 2);
    appendU2(info, innerIndex);
    appendU2(info, helperIndex);

    auto attribute = makeAttribute(pool,
                                   NEST_MEMBERS,
                                   info);

    const auto output = renderAttribute(pool, attribute);
    EXPECT_NE(output.find("pkg/Outer$Inner"), std::string::npos);
    EXPECT_NE(output.find("pkg/Outer$Helper"), std::string::npos);
}

TEST(AttributesManagerTests, RendersNestHostAttribute) {
    ConstantPool pool;
    const auto hostIndex = addClassInfo("pkg/Host", pool);

    std::vector<u1> info;
    appendU2(info, hostIndex);

    auto attribute = makeAttribute(pool,
                                   NEST_HOST,
                                   info);

    const auto output = renderAttribute(pool, attribute);
    EXPECT_NE(output.find("host=pkg/Host"), std::string::npos);
}

TEST(AttributesManagerTests, RendersLineNumberTableAttribute) {
    ConstantPool pool;
    std::vector<u1> info;
    appendU2(info, 2); // table length
    appendU2(info, 0);
    appendU2(info, 10);
    appendU2(info, 6);
    appendU2(info, 99);

    auto attribute = makeAttribute(pool,
                                   LINE_NUMBER_TABLE,
                                   info,
                                   AttributeOwner::CodeAttribute);

    const auto output = renderAttribute(pool, attribute);
    EXPECT_NE(output.find("Attr - LineNumberTable"), std::string::npos);
    EXPECT_NE(output.find("count=2"), std::string::npos);
    EXPECT_NE(output.find("pc:6"), std::string::npos);
    EXPECT_NE(output.find("line:99"), std::string::npos);
}

TEST(AttributesManagerTests, RendersRuntimeVisibleAnnotationsWithPayloads) {
    ConstantPool pool;
    const auto typeIndex = addUtf8("Lpkg/MyAnno;", pool);
    const auto valueNameIndex = addUtf8("value", pool);
    const auto answerNameIndex = addUtf8("answer", pool);
    const auto stringValueIndex = addUtf8("payload", pool);
    const auto intValueIndex = addInteger(7, pool);

    std::vector<u1> info;
    appendU2(info, 1); // num_annotations
    appendU2(info, typeIndex);
    appendU2(info, 2); // num_element_value_pairs
    appendU2(info, valueNameIndex);
    appendU1(info, JVM_SIGNATURE_STRING);
    appendU2(info, stringValueIndex);
    appendU2(info, answerNameIndex);
    appendU1(info, JVM_SIGNATURE_INT);
    appendU2(info, intValueIndex);

    auto attribute = makeAttribute(pool,
                                   RUNTIME_VISIBLE_ANNOTATIONS,
                                   info);

    const auto output = renderAttribute(pool, attribute);
    EXPECT_NE(output.find("pkg.MyAnno"), std::string::npos);
    EXPECT_NE(output.find("value=(\"payload\")"), std::string::npos);
    EXPECT_NE(output.find("answer=(7)"), std::string::npos);
}

TEST(AttributesManagerTests, RendersLocalVariableTableAttribute) {
    ConstantPool pool;
    const auto nameIndex = addUtf8("value", pool);
    const auto descriptorIndex = addUtf8("I", pool);

    std::vector<u1> info;
    appendU2(info, 1); // entry count
    appendU2(info, 0); // start_pc
    appendU2(info, 5); // length
    appendU2(info, nameIndex);
    appendU2(info, descriptorIndex);
    appendU2(info, 1); // slot

    auto attribute = makeAttribute(pool,
                                   LOCAL_VARIABLE_TABLE,
                                   info,
                                   AttributeOwner::CodeAttribute);

    const auto output = renderAttribute(pool, attribute);
    EXPECT_NE(output.find("Attr - LocalVariableTable"), std::string::npos);
    EXPECT_NE(output.find("slot:1"), std::string::npos);
    EXPECT_NE(output.find("name:value"), std::string::npos);
    EXPECT_NE(output.find("type:int"), std::string::npos);
}

TEST(AttributesManagerTests, RendersLocalVariableTypeTableAttribute) {
    ConstantPool pool;
    const auto nameIndex = addUtf8("items", pool);
    const auto signatureIndex = addUtf8("Ljava/util/List<Ljava/lang/String;>;", pool);

    std::vector<u1> info;
    appendU2(info, 1); // entry count
    appendU2(info, 2); // start_pc
    appendU2(info, 4); // length
    appendU2(info, nameIndex);
    appendU2(info, signatureIndex);
    appendU2(info, 3); // slot

    auto attribute = makeAttribute(pool,
                                   LOCAL_VARIABLE_TYPE_TABLE,
                                   info,
                                   AttributeOwner::CodeAttribute);

    const auto output = renderAttribute(pool, attribute);
    EXPECT_NE(output.find("Attr - LocalVariableTypeTable"), std::string::npos);
    EXPECT_NE(output.find("slot:3"), std::string::npos);
    EXPECT_NE(output.find("signature:Ljava/util/List<Ljava/lang/String;>;"), std::string::npos);
}

TEST(AttributesManagerTests, RendersMethodParametersAttribute) {
    ConstantPool pool;
    const auto paramNameIndex = addUtf8("arg", pool);

    std::vector<u1> info;
    appendU1(info, 2); // parameter count
    appendU2(info, paramNameIndex);
    appendU2(info, JVM_ACC_FINAL);
    appendU2(info, 0); // unnamed parameter
    appendU2(info, JVM_ACC_SYNTHETIC);

    auto attribute = makeAttribute(pool,
                                   METHOD_PARAMETERS,
                                   info,
                                   AttributeOwner::Method);

    const auto output = renderAttribute(pool, attribute);
    EXPECT_NE(output.find("count=2"), std::string::npos);
    EXPECT_NE(output.find("name:arg"), std::string::npos);
    EXPECT_NE(output.find("access:final"), std::string::npos);
    EXPECT_NE(output.find("<unnamed>"), std::string::npos);
    EXPECT_NE(output.find("synthetic"), std::string::npos);
}

TEST(AttributesManagerTests, RendersSyntheticAttribute) {
    ConstantPool pool;
    auto attribute = makeAttribute(pool,
                                   SYNTHETIC,
                                   {},
                                   AttributeOwner::ClassFile);

    const auto output = renderAttribute(pool, attribute);
    EXPECT_NE(output.find("(synthetic)"), std::string::npos);
}

TEST(AttributesManagerTests, RendersDeprecatedAttribute) {
    ConstantPool pool;
    auto attribute = makeAttribute(pool,
                                   DEPRECATED,
                                   {},
                                   AttributeOwner::ClassFile);

    const auto output = renderAttribute(pool, attribute);
    EXPECT_NE(output.find("(deprecated)"), std::string::npos);
}

TEST(AttributesManagerTests, RendersExceptionsAttribute) {
    ConstantPool pool;
    const auto exA = addClassInfo("java/lang/Exception", pool);
    const auto exB = addClassInfo("java/lang/RuntimeException", pool);

    std::vector<u1> info;
    appendU2(info, 2);
    appendU2(info, exA);
    appendU2(info, exB);

    auto attribute = makeAttribute(pool,
                                   EXCEPTIONS,
                                   info,
                                   AttributeOwner::Method);

    const auto output = renderAttribute(pool, attribute);
    EXPECT_NE(output.find("count=2"), std::string::npos);
    EXPECT_NE(output.find("java/lang/Exception"), std::string::npos);
    EXPECT_NE(output.find("java/lang/RuntimeException"), std::string::npos);
}

TEST(AttributesManagerTests, RendersPermittedSubclassesAttribute) {
    ConstantPool pool;
    const auto fooIndex = addClassInfo("pkg/Foo", pool);
    const auto barIndex = addClassInfo("pkg/Bar", pool);

    std::vector<u1> info;
    appendU2(info, 2);
    appendU2(info, fooIndex);
    appendU2(info, barIndex);

    auto attribute = makeAttribute(pool,
                                   PERMITTED_SUBCLASSES,
                                   info);

    const auto output = renderAttribute(pool, attribute);
    EXPECT_NE(output.find("pkg/Foo"), std::string::npos);
    EXPECT_NE(output.find("pkg/Bar"), std::string::npos);
}

TEST(AttributesManagerTests, RendersEnclosingMethodAttribute) {
    ConstantPool pool;
    const auto hostClass = addClassInfo("pkg/Outer$Inner", pool);
    const auto nameAndType = addNameAndType("helper", "(I)V", pool);

    std::vector<u1> info;
    appendU2(info, hostClass);
    appendU2(info, nameAndType);

    auto attribute = makeAttribute(pool,
                                   ENCLOSING_METHOD,
                                   info);

    const auto output = renderAttribute(pool, attribute);
    EXPECT_NE(output.find("host=pkg/Outer$Inner"), std::string::npos);
    EXPECT_NE(output.find("helper(int) -> void"), std::string::npos);
}

TEST(AttributesManagerTests, RendersBootstrapMethodsAttribute) {
    ConstantPool pool;
    const auto methodRef = addMethodref("pkg/Bootstrap", "factory", "(Ljava/lang/String;)Lpkg/Sample;", pool);
    const auto handle = addMethodHandle(JVM_REF_invokeStatic, methodRef, pool);
    const auto argument = addStringLiteral("payload", pool);

    std::vector<u1> info;
    appendU2(info, 1); // num bootstrap methods
    appendU2(info, handle);
    appendU2(info, 1); // arg count
    appendU2(info, argument);

    auto attribute = makeAttribute(pool,
                                   BOOTSTRAP_METHODS,
                                   info);

    const auto output = renderAttribute(pool, attribute);
    EXPECT_NE(output.find("count=1"), std::string::npos);
    EXPECT_NE(output.find("invokeStatic pkg/Bootstrap::factory"), std::string::npos);
    EXPECT_NE(output.find("payload"), std::string::npos);
}

TEST(AttributesManagerTests, RendersSignatureAttribute) {
    ConstantPool pool;
    const auto signatureIndex = addUtf8("Ljava/util/List<Ljava/lang/String;>;", pool);

    std::vector<u1> info;
    appendU2(info, signatureIndex);

    auto attribute = makeAttribute(pool,
                                   SIGNATURE,
                                   info,
                                   AttributeOwner::Field);

    const auto output = renderAttribute(pool, attribute);
    EXPECT_NE(output.find("Ljava/util/List<Ljava/lang/String;>;"), std::string::npos);
}

TEST(AttributesManagerTests, RendersRecordAttribute) {
    ConstantPool pool;
    const auto componentName = addUtf8("value", pool);
    const auto descriptorIndex = addUtf8("Ljava/lang/String;", pool);

    std::vector<u1> info;
    appendU2(info, 1); // components
    appendU2(info, componentName);
    appendU2(info, descriptorIndex);
    appendU2(info, 0); // nested attributes

    auto attribute = makeAttribute(pool,
                                   RECORD,
                                   info);

    const auto output = renderAttribute(pool, attribute);
    EXPECT_NE(output.find("components=1"), std::string::npos);
    EXPECT_NE(output.find("value:java.lang.String"), std::string::npos);
}

TEST(AttributesManagerTests, RendersStackMapTableAttribute) {
    ConstantPool pool;
    std::vector<u1> info;
    appendU2(info, 2); // number_of_entries
    info.push_back(0); // same_frame
    info.push_back(64); // same_locals_1_stack_item_frame
    info.push_back(JVM_ITEM_Integer);

    auto attribute = makeAttribute(pool,
                                   STACK_MAP_TABLE,
                                   info,
                                   AttributeOwner::CodeAttribute);

    const auto output = renderAttribute(pool, attribute);
    EXPECT_NE(output.find("frames=2"), std::string::npos);
    EXPECT_NE(output.find("same=1"), std::string::npos);
    EXPECT_NE(output.find("same_locals=1"), std::string::npos);
}
