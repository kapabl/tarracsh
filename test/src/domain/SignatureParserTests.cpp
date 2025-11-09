#include <gtest/gtest.h>

#include <cstdlib>
#include <cstring>
#include <vector>

#include "domain/classfile/AttributeStructures.h"
#include "domain/classfile/AttributeTags.h"
#include "domain/classfile/constantpool/ConstantPool.h"
#include "domain/classfile/constantpool/ConstpoolStructures.h"
#include "domain/classfile/reader/VectorReader.h"
#include "domain/classfile/signature/SignatureParser.h"

using namespace kapa::tarracsh::domain;
using namespace classfile;
using namespace classfile::attribute;
using namespace classfile::constantpool;
using namespace classfile::reader;
using namespace classfile::signature;

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

void appendU2LittleEndian(std::vector<u1> &buffer, u2 value) {
    buffer.push_back(static_cast<u1>(value & 0xFF));
    buffer.push_back(static_cast<u1>((value >> 8) & 0xFF));
}

AttributeInfo makeSignatureAttribute(u2 nameIndex, u2 signatureIndex, AttributeOwner owner) {
    AttributeInfo attribute{};
    attribute.nameIndex = nameIndex;
    attribute.length = 2;
    attribute.owner = owner;
    appendU2LittleEndian(attribute.info, signatureIndex);
    return attribute;
}

std::string toString(const ConstantPool &pool, const AttributeInfo &attribute) {
    std::vector<u1> payload(attribute.info.begin(), attribute.info.end());
    VectorReader reader(payload, true);
    SignatureParser parser(pool, attribute, reader);
    return parser.toString();
}

template <typename SigT>
SigT readSignature(const ConstantPool &pool, const AttributeInfo &attribute) {
    std::vector<u1> payload(attribute.info.begin(), attribute.info.end());
    VectorReader reader(payload, true);
    SignatureParser parser(pool, attribute, reader);
    SigT signature{};
    parser.read(signature);
    return signature;
}

} // namespace

TEST(SignatureParserTests, ReadsMethodSignatureAttribute) {
    ConstantPool pool;
    const auto signatureNameIndex = addUtf8(SIGNATURE, pool);
    const auto descriptorIndex = addUtf8("(Ljava/lang/String;)V", pool);

    auto attribute = makeSignatureAttribute(signatureNameIndex, descriptorIndex, AttributeOwner::Method);

    const auto methodSignature = readSignature<MethodSignature>(pool, attribute);
    EXPECT_EQ(methodSignature.signatureIndex, descriptorIndex);

    std::vector<u1> payload(attribute.info.begin(), attribute.info.end());
    VectorReader reader(payload, true);
    SignatureParser parser(pool, attribute, reader);
    EXPECT_EQ(parser.getString(methodSignature), "(Ljava/lang/String;)V");
}

TEST(SignatureParserTests, ReadsFieldSignatureAttribute) {
    ConstantPool pool;
    const auto signatureNameIndex = addUtf8(SIGNATURE, pool);
    const auto descriptorIndex = addUtf8("Ljava/util/List<Ljava/lang/String;>;", pool);

    auto attribute = makeSignatureAttribute(signatureNameIndex, descriptorIndex, AttributeOwner::Field);
    const auto fieldSignature = readSignature<FieldSignature>(pool, attribute);
    EXPECT_EQ(fieldSignature.signatureIndex, descriptorIndex);

    EXPECT_EQ(toString(pool, attribute), "Ljava/util/List<Ljava/lang/String;>;");
}

TEST(SignatureParserTests, ToStringHandlesClassOwner) {
    ConstantPool pool;
    const auto signatureNameIndex = addUtf8(SIGNATURE, pool);
    const auto classDescriptor = addUtf8("Ljava/lang/Object;", pool);

    auto attribute = makeSignatureAttribute(signatureNameIndex, classDescriptor, AttributeOwner::ClassFile);
    EXPECT_EQ(toString(pool, attribute), "Ljava/lang/Object;");
}

TEST(SignatureParserTests, ToStringHandlesMethodOwner) {
    ConstantPool pool;
    const auto signatureNameIndex = addUtf8(SIGNATURE, pool);
    const auto methodDescriptor = addUtf8("(Ljava/lang/String;)Ljava/lang/Object;", pool);

    auto attribute = makeSignatureAttribute(signatureNameIndex, methodDescriptor, AttributeOwner::Method);
    EXPECT_EQ(toString(pool, attribute), "(Ljava/lang/String;)Ljava/lang/Object;");
}

TEST(SignatureParserTests, ToStringHandlesFieldOwner) {
    ConstantPool pool;
    const auto signatureNameIndex = addUtf8(SIGNATURE, pool);
    const auto fieldDescriptor = addUtf8("Ljava/lang/String;", pool);

    auto attribute = makeSignatureAttribute(signatureNameIndex, fieldDescriptor, AttributeOwner::Field);
    EXPECT_EQ(toString(pool, attribute), "Ljava/lang/String;");
}
