#include <gtest/gtest.h>

#include "infrastructure/string/StringUtils.h"

using namespace kapa::infrastructure::string::stringUtils;

TEST(StringUtilsEscapeUtf8, EscapesControlCharacters) {
    const std::string input = "line\n\t\"quoted\"";
    const auto output = escapeUtf8(input);
    EXPECT_EQ(output, "line\\x0a\\x09\\x22quoted\\x22");
}

TEST(StringUtilsReplaceAll, ReplacesAllOccurrences) {
    const std::string input = "foo_bar_bar";
    const auto output = replaceAll(input, "bar", "baz");
    EXPECT_EQ(output, "foo_baz_baz");
}

TEST(StringUtilsTrim, TrimsWhitespaceCharacters) {
    const std::string input = "\r\n  data\t ";
    EXPECT_EQ(trim(input), "data");
}

TEST(StringUtilsJoinVector, JoinsStringsWithDelimiter) {
    const std::vector<std::string> parts{"a", "b", "c"};
    EXPECT_EQ(join(parts, std::string(",")), "a,b,c");
}

TEST(StringUtilsSplit, SplitsOnDelimiter) {
    const std::string input = "alpha::beta::gamma";
    const auto tokens = split(input, "::");
    ASSERT_EQ(tokens.size(), 3U);
    EXPECT_EQ(tokens[0], "alpha");
    EXPECT_EQ(tokens[1], "beta");
    EXPECT_EQ(tokens[2], "gamma");
}

TEST(StringUtilsPathToClassname, ConvertsSlashesToDots) {
    const std::string path = "pkg/subpkg/ClassName";
    EXPECT_EQ(pathToClassname(path), "pkg.subpkg.ClassName");
}

TEST(StringUtilsBytesToHexString, ProducesHexRepresentation) {
    const unsigned char bytes[]{0xDE, 0xAD, 0xBE, 0xEF};
    EXPECT_EQ(bytesToHexString(bytes, 4), "deadbeef");
}

TEST(StringUtilsSizeToHumanReadable, FormatsKilobytes) {
    EXPECT_EQ(sizeToHumanReadable(2048), "2.00 KB");
}
