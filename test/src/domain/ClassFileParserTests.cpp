#include <gtest/gtest.h>

#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <memory>
#include <string_view>
#include <vector>
#include <cstring>
#include <cstdint>

#include "domain/classfile/reader/ClassFileReader.h"
#include "domain/classfile/ClassFileParser.h"
#include "domain/classfile/ClassFileConstants.h"
#include "domain/classfile/signature/SignatureParser.h"
#include "domain/classfile/reader/VectorReader.h"
#include "infrastructure/log/Log.h"

using namespace kapa::tarracsh::domain;
using namespace classfile;
using namespace classfile::reader;

namespace {

std::vector<char> loadBinary(const std::filesystem::path &path) {
    std::ifstream file(path, std::ios::binary);
    return std::vector<char>(std::istreambuf_iterator<char>(file), {});
}

std::filesystem::path testResource(const std::string &relative) {
    if (const char *srcDir = std::getenv("TEST_SRCDIR")) {
        if (const char *workspace = std::getenv("TEST_WORKSPACE")) {
            return std::filesystem::path(srcDir) / workspace / relative;
        }
    }
    return std::filesystem::path(PROJECT_SOURCE_DIR) / relative;
}

void appendU2(std::vector<char> &buffer, uint16_t value) {
    buffer.push_back(static_cast<char>((value >> 8) & 0xFF));
    buffer.push_back(static_cast<char>(value & 0xFF));
}

void appendU4(std::vector<char> &buffer, uint32_t value) {
    buffer.push_back(static_cast<char>((value >> 24) & 0xFF));
    buffer.push_back(static_cast<char>((value >> 16) & 0xFF));
    buffer.push_back(static_cast<char>((value >> 8) & 0xFF));
    buffer.push_back(static_cast<char>(value & 0xFF));
}

void appendUtf8Entry(std::vector<char> &buffer, std::string_view text) {
    buffer.push_back(0x01);
    appendU2(buffer, static_cast<uint16_t>(text.size()));
    buffer.insert(buffer.end(), text.begin(), text.end());
}

void appendClassEntry(std::vector<char> &buffer, uint16_t nameIndex) {
    buffer.push_back(0x07);
    appendU2(buffer, nameIndex);
}

void appendStringEntry(std::vector<char> &buffer, uint16_t stringIndex) {
    buffer.push_back(0x08);
    appendU2(buffer, stringIndex);
}

void appendFloatEntry(std::vector<char> &buffer, float value) {
    static_assert(sizeof(float) == sizeof(uint32_t));
    uint32_t raw{};
    std::memcpy(&raw, &value, sizeof(float));
    buffer.push_back(0x04);
    appendU4(buffer, raw);
}

void appendLongEntry(std::vector<char> &buffer, int64_t value) {
    buffer.push_back(0x05);
    const auto high = static_cast<uint32_t>((static_cast<uint64_t>(value) >> 32) & 0xFFFFFFFFULL);
    const auto low = static_cast<uint32_t>(static_cast<uint64_t>(value) & 0xFFFFFFFFULL);
    appendU4(buffer, high);
    appendU4(buffer, low);
}

void appendDoubleEntry(std::vector<char> &buffer, double value) {
    static_assert(sizeof(double) == sizeof(uint64_t));
    uint64_t raw{};
    std::memcpy(&raw, &value, sizeof(double));
    buffer.push_back(0x06);
    appendU4(buffer, static_cast<uint32_t>((raw >> 32) & 0xFFFFFFFFULL));
    appendU4(buffer, static_cast<uint32_t>(raw & 0xFFFFFFFFULL));
}

void appendNameAndTypeEntry(std::vector<char> &buffer, uint16_t nameIndex, uint16_t descriptorIndex) {
    buffer.push_back(0x0C);
    appendU2(buffer, nameIndex);
    appendU2(buffer, descriptorIndex);
}

void appendMethodrefEntry(std::vector<char> &buffer, uint16_t classIndex, uint16_t nameAndTypeIndex) {
    buffer.push_back(0x0A);
    appendU2(buffer, classIndex);
    appendU2(buffer, nameAndTypeIndex);
}

void appendMethodTypeEntry(std::vector<char> &buffer, uint16_t descriptorIndex) {
    buffer.push_back(0x10);
    appendU2(buffer, descriptorIndex);
}

void appendMethodHandleEntry(std::vector<char> &buffer, uint8_t referenceKind, uint16_t referenceIndex) {
    buffer.push_back(0x0F);
    buffer.push_back(static_cast<char>(referenceKind));
    appendU2(buffer, referenceIndex);
}

void appendDynamicEntry(std::vector<char> &buffer, uint16_t bootstrapIndex, uint16_t nameAndTypeIndex) {
    buffer.push_back(0x11);
    appendU2(buffer, bootstrapIndex);
    appendU2(buffer, nameAndTypeIndex);
}

void appendInvokeDynamicEntry(std::vector<char> &buffer, uint16_t bootstrapIndex, uint16_t nameAndTypeIndex) {
    buffer.push_back(0x12);
    appendU2(buffer, bootstrapIndex);
    appendU2(buffer, nameAndTypeIndex);
}

void appendModuleEntry(std::vector<char> &buffer, uint16_t nameIndex) {
    buffer.push_back(0x13);
    appendU2(buffer, nameIndex);
}

void appendPackageEntry(std::vector<char> &buffer, uint16_t nameIndex) {
    buffer.push_back(0x14);
    appendU2(buffer, nameIndex);
}

std::filesystem::path makeTempPath(const std::string &suffix) {
    const auto base = std::filesystem::temp_directory_path();
    const auto now = std::chrono::steady_clock::now().time_since_epoch().count();
    const auto candidate = base / ("tarracsh-" + std::to_string(now) + suffix);
    return candidate;
}

std::shared_ptr<kapa::infrastructure::log::Log> makeLog(const std::filesystem::path &path) {
    auto log = std::make_shared<kapa::infrastructure::log::Log>();
    std::filesystem::create_directories(path.parent_path());
    log->init(path.string());
    return log;
}

class BufferReader final : public ClassFileReader {
public:
    explicit BufferReader(std::vector<char> data)
        : _data(std::move(data)) {
        _size = static_cast<std::streamsize>(_data.size());
        readHeader();
    }

    void readBytes(char *buffer, unsigned int count) override {
        if (_position + count > _data.size()) {
            throw std::runtime_error("Read past end of buffer");
        }
        std::memcpy(buffer, _data.data() + _position, count);
        _position += count;
        _lastReadCount = count;
    }

private:
    std::vector<char> _data;
    size_t _position{0};
};

} // namespace

TEST(ClassFileReaderTests, ReadsConstantPoolCount) {
    const auto classPath =
        testResource("test-subjects/org/kapa/reti/ast/BoolLiteralNode.class");
    BufferReader reader(loadBinary(classPath));

    EXPECT_TRUE(reader.isValidHeader());
    const auto constantPoolCount = reader.readU2();
    EXPECT_GT(constantPoolCount, 1);
}

TEST(ClassFileReaderTests, ReadsPrimitiveValuesSequentially) {
    std::vector<char> data{
        static_cast<char>(0xCA), static_cast<char>(0xFE), static_cast<char>(0xBA), static_cast<char>(0xBE),
        0x00, 0x00, 0x00, 0x34,
        0x01, 0x02,
        0x03, 0x04,
        0x05, 0x06, 0x07, 0x08,
        static_cast<char>(0xFF)
    };
    BufferReader reader(std::move(data));

    ASSERT_TRUE(reader.isValidHeader());
    EXPECT_EQ(reader.readU2(), 0x0102);
    EXPECT_EQ(reader.readU2Reversed(), 0x0403);
    EXPECT_EQ(reader.readU4(), 0x05060708);
    EXPECT_EQ(reader.readU1(), 0xFF);
}

TEST(ClassFileParserTests, ParsesValidClassFile) {
    const auto classPath =
        testResource("test-subjects/org/kapa/reti/ast/BoolLiteralNode.class");

    BufferReader reader(loadBinary(classPath));
    auto log = makeLog(makeTempPath("parser.log"));

    ClassFileParser parser(reader, classPath.string(), log);
    ASSERT_TRUE(parser.parse());
    EXPECT_FALSE(parser.getMethods().empty());
    EXPECT_FALSE(parser.getFields().empty());
}

TEST(ClassFileParserTests, ParsesInterfacesAndSourceFileAttribute) {
    std::vector<char> bytes;
    appendU4(bytes, 0xCAFEBABE);
    appendU2(bytes, 0);  // minor
    appendU2(bytes, 52); // major
    appendU2(bytes, 7);  // constant pool count

    appendUtf8Entry(bytes, "Sample");               // #1
    appendClassEntry(bytes, 1);                     // #2
    appendUtf8Entry(bytes, "java/lang/Object");     // #3
    appendClassEntry(bytes, 3);                     // #4
    appendUtf8Entry(bytes, "Sample.java");          // #5
    appendUtf8Entry(bytes, "SourceFile");           // #6

    appendU2(bytes, 0x0021); // access_flags (public + super)
    appendU2(bytes, 2);      // this_class (#2)
    appendU2(bytes, 4);      // super_class (#4)

    appendU2(bytes, 1);      // interfaces_count
    appendU2(bytes, 4);      // interface -> using java/lang/Object for simplicity

    appendU2(bytes, 0);      // fields_count
    appendU2(bytes, 0);      // methods_count

    appendU2(bytes, 1);      // attributes_count
    appendU2(bytes, 6);      // attribute_name_index -> SourceFile
    appendU4(bytes, 2);      // attribute_length
    appendU2(bytes, 5);      // sourcefile_index -> Sample.java

    BufferReader reader(bytes);
    auto log = makeLog(makeTempPath("parser-interfaces.log"));
    ClassFileParser parser(reader, "Sample.class", log);
    ASSERT_TRUE(parser.parse());

    EXPECT_EQ(parser.getInterfaces().size(), 1U);
    EXPECT_EQ(parser.getInterfaces()[0], 4);
    EXPECT_EQ(parser.getMainClassname(), "Sample");
    EXPECT_EQ(parser.getSuperClassIndex(), 4);
    ASSERT_EQ(parser.getAttributes().size(), 1U);
    const auto &attribute = parser.getAttributes()[0];
    EXPECT_EQ(attribute.nameIndex, 6);
    EXPECT_EQ(attribute.length, 2U);
    ASSERT_EQ(attribute.info.size(), 2U);
    EXPECT_EQ(static_cast<uint16_t>((attribute.info[0] << 8) | attribute.info[1]), 5);
}

TEST(ClassFileParserTests, FailsGracefullyOnTruncatedInput) {
    std::vector<char> truncated{
        static_cast<char>(0xCA), static_cast<char>(0xFE), static_cast<char>(0xBA), static_cast<char>(0xBE),
        0x00, 0x00, 0x00, 0x34};
    BufferReader reader(truncated);
    auto log = makeLog(makeTempPath("parser-fail.log"));

    ClassFileParser parser(reader, "InMemory.class", log);
    EXPECT_FALSE(parser.parse());
}

TEST(ClassFileReaderTests, ByteSwapHelpers) {
    const unsigned short originalShort = 0x1234;
    EXPECT_EQ(ClassFileReader::swapShort(originalShort), static_cast<unsigned short>(0x3412));

    const unsigned int originalInt = 0x01020304;
    EXPECT_EQ(ClassFileReader::swapLong(originalInt), 0x04030201U);
}

TEST(ClassFileParserTests, FailsOnUnknownConstantPoolTag) {
    std::vector<char> bytes;
    appendU4(bytes, 0xCAFEBABE);
    appendU2(bytes, 0);          // minor
    appendU2(bytes, 52);         // major
    appendU2(bytes, 7);          // constant pool count

    appendUtf8Entry(bytes, "Example");          // #1
    appendClassEntry(bytes, 1);                 // #2
    appendUtf8Entry(bytes, "java/lang/Object"); // #3
    appendClassEntry(bytes, 3);                 // #4
    appendUtf8Entry(bytes, "tests/Bad");        // #5
    appendClassEntry(bytes, 5);                 // #6

    bytes.push_back(static_cast<char>(0x63));   // invalid tag
    appendU2(bytes, 0x0001);                    // dummy payload

    appendU2(bytes, 0x0021); // access_flags
    appendU2(bytes, 6);      // this_class
    appendU2(bytes, 4);      // super_class
    appendU2(bytes, 0);      // interfaces_count
    appendU2(bytes, 0);      // fields_count
    appendU2(bytes, 0);      // methods_count
    appendU2(bytes, 0);      // attributes_count

    BufferReader reader(bytes);
    auto log = makeLog(makeTempPath("parser-invalid.log"));
    ClassFileParser parser(reader, "Invalid.class", log);
    EXPECT_FALSE(parser.parse());
}

TEST(ClassFileParserTests, ParsesExtendedConstantPoolEntries) {
    std::vector<char> bytes;
    appendU4(bytes, 0xCAFEBABE);
    appendU2(bytes, 0);
    appendU2(bytes, 52);
    appendU2(bytes, 26); // constant pool count

    appendUtf8Entry(bytes, "Example");          // #1
    appendClassEntry(bytes, 1);                 // #2
    appendUtf8Entry(bytes, "java/lang/Object"); // #3
    appendClassEntry(bytes, 3);                 // #4
    appendUtf8Entry(bytes, "Sample.java");      // #5
    appendUtf8Entry(bytes, "SourceFile");       // #6
    appendUtf8Entry(bytes, "SomeString");       // #7
    appendStringEntry(bytes, 7);                // #8
    appendFloatEntry(bytes, 3.5F);              // #9
    appendLongEntry(bytes, 0x0102030405060708LL); // #10 (+ #11 placeholder)
    appendDoubleEntry(bytes, 1234.5);           // #12 (+ #13 placeholder)
    appendUtf8Entry(bytes, "(I)V");             // #14
    appendUtf8Entry(bytes, "methodName");       // #15
    appendNameAndTypeEntry(bytes, 15, 14);      // #16
    appendMethodTypeEntry(bytes, 14);           // #17
    appendMethodrefEntry(bytes, 2, 16);         // #18
    appendMethodHandleEntry(bytes, JVM_REF_invokeStatic, 18); // #19
    appendDynamicEntry(bytes, 1, 16);           // #20
    appendInvokeDynamicEntry(bytes, 1, 16);     // #21
    appendUtf8Entry(bytes, "module/name");      // #22
    appendModuleEntry(bytes, 22);               // #23
    appendUtf8Entry(bytes, "pkg/name");         // #24
    appendPackageEntry(bytes, 24);              // #25

    appendU2(bytes, 0x0021); // access_flags
    appendU2(bytes, 2);      // this_class
    appendU2(bytes, 4);      // super_class
    appendU2(bytes, 0);      // interfaces_count
    appendU2(bytes, 0);      // fields_count
    appendU2(bytes, 0);      // methods_count
    appendU2(bytes, 1);      // attributes_count
    appendU2(bytes, 6);      // SourceFile attribute name
    appendU4(bytes, 2);      // attribute_length
    appendU2(bytes, 5);      // sourcefile_index

    BufferReader reader(bytes);
    auto log = makeLog(makeTempPath("parser-extended.log"));
    ClassFileParser parser(reader, "Extended.class", log);
    ASSERT_TRUE(parser.parse());

    auto &pool = parser.getConstantPool();
    EXPECT_EQ(pool.getClassInfoName(2), "Example");
    EXPECT_FLOAT_EQ(pool.getEntry(9).floatInfo.getFloat(), 3.5F);
    EXPECT_EQ(pool.getEntry(10).longInfo.getLongLong(), 0x0102030405060708LL);
    EXPECT_DOUBLE_EQ(pool.getEntry(12).doubleInfo.getDouble(), 1234.5);
    EXPECT_EQ(pool.getEntry(17).methodTypeInfo.descriptorIndex, 14);
    EXPECT_EQ(pool.getEntry(19).methodHandleInfo.referenceKind, JVM_REF_invokeStatic);
    EXPECT_EQ(pool.getEntry(19).methodHandleInfo.referenceIndex, 18);
    EXPECT_EQ(pool.getEntry(20).dynamicInfo.bootstrapMethodAttrIndex, 1);
    EXPECT_EQ(pool.getEntry(20).dynamicInfo.nameAndTypeIndex, 16);
    EXPECT_EQ(pool.getEntry(21).invokeDynamicInfo.bootstrapMethodAttrIndex, 1);
    EXPECT_EQ(pool.getEntry(21).invokeDynamicInfo.nameAndTypeIndex, 16);
    EXPECT_EQ(pool.getEntry(23).moduleInfo.nameIndex, 22);
    EXPECT_EQ(pool.getEntry(25).packageInfo.nameIndex, 24);
    EXPECT_EQ(pool.getEntry(8).stringInfo.stringIndex, 7);
}

TEST(ClassFileSignatureParser, ReadsSignatureAttribute) {
    constantpool::ConstantPool pool;

    const std::string signatureValue = "Lfoo/Bar;";
    const auto allocSize = sizeof(constantpool::ConstantPoolRecord::utf8Info.tag) +
                           sizeof(constantpool::ConstantPoolRecord::utf8Info.length) +
                           signatureValue.size() + 1;
    auto *signatureRecord = static_cast<constantpool::ConstantPoolRecord *>(std::malloc(allocSize));
    signatureRecord->utf8Info.tag = JVM_CONSTANT_Utf8;
    signatureRecord->utf8Info.length = static_cast<constantpool::u2>(signatureValue.size());
    std::memcpy(signatureRecord->utf8Info.bytes, signatureValue.data(), signatureValue.size());
    signatureRecord->utf8Info.bytes[signatureValue.size()] = 0;
    pool.addRecord(*signatureRecord, allocSize);
    std::free(signatureRecord);

    attribute::AttributeInfo attribute{};
    attribute.owner = attribute::AttributeOwner::ClassFile;
    attribute.nameIndex = 0;
    attribute.length = 2;
    attribute.info = {0x01, 0x00}; // points to constant pool index 1 (big endian)

    std::vector<constantpool::u1> payload(attribute.info.begin(), attribute.info.end());
    reader::VectorReader reader(payload, true);

    signature::SignatureParser parser(pool, attribute, reader);
    signature::ClassSignature signature;
    parser.read(signature);
    EXPECT_EQ(signature.signatureIndex, 1);

    EXPECT_EQ(pool.getString(signature.signatureIndex), "Lfoo/Bar;");
}
