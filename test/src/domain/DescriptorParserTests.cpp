#include <gtest/gtest.h>

#include "domain/classfile/DescriptorParser.h"
#include "domain/classfile/MethodDescriptorParser.h"

using namespace kapa::tarracsh::domain::classfile;
using kapa::tarracsh::domain::classfile::constantpool::Descriptor;
using kapa::tarracsh::domain::classfile::constantpool::MethodDescriptor;

namespace {

Descriptor parseDescriptor(const std::string &descriptorString) {
    DescriptorParser parser(descriptorString);
    return parser.getDescriptor();
}

MethodDescriptor parseMethodDescriptor(const std::string &descriptorString) {
    MethodDescriptorParser parser(descriptorString);
    return parser.getDescriptor();
}

} // namespace

TEST(DescriptorParserTests, ParsesPrimitiveDescriptors) {
    const auto descriptor = parseDescriptor("I");
    EXPECT_FALSE(descriptor.isArray);
    EXPECT_FALSE(descriptor.isClass);
    EXPECT_EQ(descriptor.type, "int");
    EXPECT_EQ(descriptor.toString(), "int");
}

TEST(DescriptorParserTests, ParsesMultiDimensionalClassArrays) {
    const auto descriptor = parseDescriptor("[[Ljava/lang/String;");
    EXPECT_TRUE(descriptor.isArray);
    EXPECT_TRUE(descriptor.isClass);
    EXPECT_EQ(descriptor.dimensions, 2);
    EXPECT_EQ(descriptor.type, "java.lang.String");
    EXPECT_EQ(descriptor.toString(), "java.lang.String[][]");
}

TEST(MethodDescriptorParserTests, ParsesArgumentsAndReturnType) {
    const auto descriptor = parseMethodDescriptor("(ILjava/lang/String;[[I)Ljava/util/List;");
    ASSERT_EQ(descriptor.arguments.size(), 3);
    EXPECT_EQ(descriptor.arguments[0].toString(), "int");
    EXPECT_EQ(descriptor.arguments[1].toString(), "java.lang.String");
    EXPECT_EQ(descriptor.arguments[2].toString(), "int[][]");
    EXPECT_EQ(descriptor.returnType.toString(), "java.util.List");
    EXPECT_EQ(descriptor.argumentsToString(), "(int, java.lang.String, int[][])");
}
