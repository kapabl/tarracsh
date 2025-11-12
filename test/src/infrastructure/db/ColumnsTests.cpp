#include <gtest/gtest.h>

#include <array>
#include <chrono>
#include <cstdint>
#include <filesystem>
#include <numeric>
#include <string>
#include <vector>

#include "infrastructure/db/Database.h"
#include "infrastructure/db/columns/Columns.h"
#include "infrastructure/log/Log.h"

using namespace kapa::infrastructure::db;
using namespace kapa::infrastructure::db::table::column;
using kapa::infrastructure::log::Log;

namespace {

std::string tempDir() {
    static const auto dir =
        (std::filesystem::temp_directory_path() / "columns-tests").string();
    return dir;
}

Log &testLog() {
    static Log logInstance;
    return logInstance;
}

class DummyDatabase : public Database {
public:
    DummyDatabase()
        : Database(Database::Config{tempDir(), &testLog()}, false) {}
};

} // namespace

TEST(ColumnsTests, StorageTypeToStringCoversKnownValues) {
    EXPECT_EQ(StorageTypeToString(StorageType::Int32), "signed 32 bits");
    EXPECT_EQ(StorageTypeToString(StorageType::UInt64), "unsigned 64 bits");
    EXPECT_EQ(StorageTypeToString(StorageType::Digest), "Digest");
    EXPECT_EQ(
        StorageTypeToString(static_cast<StorageType>(-1)),
        "unknown");
}

TEST(ColumnsTests, DisplayAsToStringCoversKnownValues) {
    EXPECT_EQ(displayAsToString(DisplayAs::AsBool), "bool true/false");
    EXPECT_EQ(
        displayAsToString(DisplayAs::AsEntryType),
        "EntryType(Classfile, Jar, Directory)");
    EXPECT_EQ(
        displayAsToString(static_cast<DisplayAs>(-1)),
        "unknown");
}

TEST(ColumnsTests, RefColDetectsInvalid) {
    RefCol ref{};
    EXPECT_FALSE(ref.isInvalid());
    ref.id = RefCol::InvalidRef;
    EXPECT_TRUE(ref.isInvalid());
}

TEST(ColumnsTests, DigestColAssignsAndComparesBuffers) {
    DigestCol digest;
    std::vector<unsigned char> bytes(DIGEST_LENGTH);
    std::iota(bytes.begin(), bytes.end(), 0);
    digest = bytes;
    EXPECT_TRUE(digest == bytes);
    bytes[0] ^= 0xFF;
    EXPECT_FALSE(digest == bytes);
}

TEST(ColumnsTests, PropertiesValueToStringFormatsIntegralTypes) {
    DummyDatabase db;

    int32_t intValue = -42;
    Properties intProp("signed", StorageType::Int32, DisplayAs::AsInt32, 0);
    EXPECT_EQ(
        intProp.valueToString(reinterpret_cast<char *>(&intValue), db, false),
        "-42");

    uint32_t uintValue = 123;
    Properties uintProp("u32", StorageType::UInt32, DisplayAs::AsUInt32, 0);
    EXPECT_EQ(
        uintProp.valueToString(reinterpret_cast<char *>(&uintValue), db, false),
        "123");

    uint64_t bigValue = 9999;
    Properties bigProp("u64", StorageType::UInt64, DisplayAs::AsUInt64, 0);
    EXPECT_EQ(
        bigProp.valueToString(reinterpret_cast<char *>(&bigValue), db, false),
        "9999");
}

TEST(ColumnsTests, PropertiesValueToStringFormatsBoolAndSizeAndDigest) {
    DummyDatabase db;

    unsigned char flag = 1;
    Properties boolProp("flag", StorageType::UInt32, DisplayAs::AsBool, 0);
    EXPECT_EQ(
        boolProp.valueToString(reinterpret_cast<char *>(&flag), db, false),
        "true");
    flag = 0;
    EXPECT_EQ(
        boolProp.valueToString(reinterpret_cast<char *>(&flag), db, false),
        "false");

    uint64_t bytes = 4096;
    Properties sizeProp("size", StorageType::UInt64, DisplayAs::AsSize, 0);
    EXPECT_EQ(
        sizeProp.valueToString(reinterpret_cast<char *>(&bytes), db, false),
        "4.00 KB");

    std::array<char, DIGEST_LENGTH> digestBytes{};
    for (size_t i = 0; i < digestBytes.size(); ++i) {
        digestBytes[i] = static_cast<char>(i);
    }
    Properties digestProp("digest", StorageType::Digest, DisplayAs::AsDigest, 0);
    const auto digestString =
        digestProp.valueToString(digestBytes.data(), db, false);
    EXPECT_NE(digestString.find("00"), std::string::npos);
    EXPECT_NE(digestString.find("01"), std::string::npos);
}

TEST(ColumnsTests, PropertiesValueToStringFormatsStringRawWhenRequested) {
    DummyDatabase db;
    uint64_t stringIndex = 321;
    Properties strProp("str", StorageType::String, DisplayAs::AsString, 0);
    EXPECT_EQ(
        strProp.valueToString(
            reinterpret_cast<char *>(&stringIndex), db, true),
        "321");
}
