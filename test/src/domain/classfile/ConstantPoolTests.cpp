#include <gtest/gtest.h>

#include "domain/classfile/ClassFileConstants.h"
#include "domain/classfile/constantpool/ConstantPool.h"

using namespace kapa::tarracsh::domain::classfile::constantpool;

namespace {

ConstantPoolRecord makeClassInfo(u2 nameIndex) {
    ConstantPoolRecord record{};
    record.classInfo.tag = JVM_CONSTANT_Class;
    record.classInfo.nameIndex = nameIndex;
    return record;
}

ConstantPoolRecord makeStringInfo(u2 stringIndex) {
    ConstantPoolRecord record{};
    record.stringInfo.tag = JVM_CONSTANT_String;
    record.stringInfo.stringIndex = stringIndex;
    return record;
}

ConstantPoolRecord makeIntegerInfo(int value) {
    ConstantPoolRecord record{};
    record.integerInfo.tag = JVM_CONSTANT_Integer;
    record.integerInfo.value = value;
    return record;
}

ConstantPoolRecord makeFloatInfo(float value) {
    ConstantPoolRecord record{};
    record.floatInfo.tag = JVM_CONSTANT_Float;
    record.floatInfo.u4Value = *reinterpret_cast<uint32_t *>(&value);
    return record;
}

ConstantPoolRecord makeLongInfo(long long value) {
    ConstantPoolRecord record{};
    record.longInfo.tag = JVM_CONSTANT_Long;
    record.longInfo.highBytes = static_cast<uint32_t>(value >> 32);
    record.longInfo.lowBytes = static_cast<uint32_t>(value & 0xffffffffULL);
    return record;
}

ConstantPoolRecord makeDoubleInfo(double value) {
    ConstantPoolRecord record{};
    record.doubleInfo.tag = JVM_CONSTANT_Double;
    uint64_t bits = *reinterpret_cast<uint64_t *>(&value);
    record.doubleInfo.highBytes = static_cast<uint32_t>(bits >> 32);
    record.doubleInfo.lowBytes = static_cast<uint32_t>(bits & 0xffffffffULL);
    return record;
}

}  // namespace

TEST(ConstantPoolTests, ReturnsClassNameFromClassInfo) {
    ConstantPool pool;
    pool.addUtf8Record("ExampleClass");
    auto classInfo = makeClassInfo(1);
    pool.addRecord(classInfo, sizeof(classInfo.classInfo));

    EXPECT_EQ(pool.getClassInfoName(2), "ExampleClass");
}

TEST(ConstantPoolTests, FormatsStringAndIntegerConstants) {
    ConstantPool pool;
    pool.addUtf8Record("alpha");
    auto stringInfo = makeStringInfo(1);
    pool.addRecord(stringInfo, sizeof(stringInfo.stringInfo));

    auto integerInfo = makeIntegerInfo(42);
    pool.addRecord(integerInfo, sizeof(integerInfo.integerInfo));

    EXPECT_EQ(pool.getConstantValueString(2), "alpha");
    EXPECT_EQ(pool.getConstantValueString(3), "42");
}

TEST(ConstantPoolTests, FormatsFloatsLongsAndDoubles) {
    ConstantPool pool;

    auto floatInfo = makeFloatInfo(3.5f);
    pool.addRecord(floatInfo, sizeof(floatInfo.floatInfo));

    auto longInfo = makeLongInfo(922337203685477580LL);
    pool.addRecord(longInfo, sizeof(longInfo.longInfo));
    pool.addEmptyIndex();  // Long consumes two entries.

    auto doubleInfo = makeDoubleInfo(6.25);
    pool.addRecord(doubleInfo, sizeof(doubleInfo.doubleInfo));
    pool.addEmptyIndex();  // Double consumes two entries.

    EXPECT_EQ(pool.getConstantValueString(1), "3.500000");
    EXPECT_EQ(pool.getConstantValueString(2), "922337203685477580");
    EXPECT_EQ(pool.getConstantValueString(4), "6.250000");
}

TEST(ConstantPoolTests, TypeStringParsesDescriptors) {
    ConstantPool pool;
    pool.addUtf8Record("(Ljava/lang/String;)V");

    const auto descriptor = pool.getTypeString(1);
    EXPECT_NE(descriptor.find("String"), std::string::npos);
}

TEST(ConstantPoolTests, NextIndexSkipsEmptyEntries) {
    ConstantPool pool;
    auto stringInfo = makeStringInfo(0);
    pool.addRecord(stringInfo, sizeof(stringInfo.stringInfo));
    pool.addEmptyIndex();
    auto next = pool.getNextIndex(1);
    EXPECT_EQ(next, 3);
}
