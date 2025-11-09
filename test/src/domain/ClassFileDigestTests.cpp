#include <filesystem>
#include <cstdlib>
#include <gtest/gtest.h>
#include <memory>

#include "domain/digest/ClassFileDigest.h"
#include "domain/digest/DigestUtils.h"
#include "domain/classfile/reader/FileReader.h"
#include "domain/classfile/ClassFileParser.h"
#include "infrastructure/log/Log.h"
#include "infrastructure/string/StringUtils.h"

using namespace kapa::tarracsh::domain;
using namespace digest;
using namespace classfile;
using namespace kapa::infrastructure;

namespace {

using DigestCol = kapa::infrastructure::db::table::column::DigestCol;

std::string digestToHex(const DigestCol &digest) {
    return string::stringUtils::bytesToHexString(digest.buf, DIGEST_LENGTH);
}

std::filesystem::path testResource(const std::string &relative) {
    if (const char *srcDir = std::getenv("TEST_SRCDIR")) {
        if (const char *workspace = std::getenv("TEST_WORKSPACE")) {
            return std::filesystem::path(srcDir) / workspace / relative;
        }
    }
    return std::filesystem::path(PROJECT_SOURCE_DIR) / relative;
}

} // namespace

TEST(ClassFileDigest, ProducesDeterministicDigestForSampleClass) {
    const auto classPath =
        testResource("test-subjects/org/kapa/reti/ast/BoolLiteralNode.class");
    reader::FileReader reader(classPath.string());
    auto log = std::make_shared<log::Log>();
    ClassFileParser parser(reader, classPath.string(), log);

    ASSERT_TRUE(parser.parse()) << "Failed to parse " << classPath;

    ClassFileDigest digestCalculator(parser);

    const auto digestCol = digestCalculator.digest();
    const auto hex = digestToHex(digestCol);

    static constexpr auto kExpectedDigest =
        "013c4f3967293fd9f02ea8478149ea75794de80ad7793ee5be5985505708ec920000000000000000";
    EXPECT_EQ(hex, kExpectedDigest);

    const auto digestColRepeat = digestCalculator.digest();
    EXPECT_EQ(digestToHex(digestColRepeat), hex);
}

TEST(ClassFileDigest, DistinguishesDifferentClasses) {
    const auto boolLiteralPath =
        testResource("test-subjects/org/kapa/reti/ast/BoolLiteralNode.class");
    const auto stringLiteralPath =
        testResource("test-subjects/org/kapa/reti/ast/StringLiteralNode.class");

    reader::FileReader boolReader(boolLiteralPath.string());
    auto log = std::make_shared<log::Log>();
    ClassFileParser boolParser(boolReader, boolLiteralPath.string(), log);
    ASSERT_TRUE(boolParser.parse());

    reader::FileReader stringReader(stringLiteralPath.string());
    ClassFileParser stringParser(stringReader, stringLiteralPath.string(), log);
    ASSERT_TRUE(stringParser.parse());

    ClassFileDigest boolDigest(boolParser);
    ClassFileDigest stringDigest(stringParser);

    const auto boolDigestHex = digestToHex(boolDigest.digest());
    const auto stringDigestHex = digestToHex(stringDigest.digest());

    EXPECT_NE(boolDigestHex, stringDigestHex);
}
