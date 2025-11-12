#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <chrono>
#include <string>
#include <vector>

#include "infrastructure/filesystem/Utils.h"

using namespace kapa::infrastructure::filesystem::utils;

namespace {

std::filesystem::path tempPath(const std::string &suffix) {
    const auto base = std::filesystem::temp_directory_path();
    const auto path = base / ("tarracsh-fs-" + suffix + "-" +
                              std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()));
    return path;
}

} // namespace

TEST(FilesystemUtilsTests, DetectsFileTypes) {
    EXPECT_TRUE(isClassfile("Foo.class"));
    EXPECT_FALSE(isClassfile("Foo.txt"));

    EXPECT_TRUE(isJar("sample.jar"));
    EXPECT_FALSE(isJar("sample.zip"));

    EXPECT_TRUE(isJMod("module.jmod"));
    EXPECT_FALSE(isJMod("module.jar"));
}

TEST(FilesystemUtilsTests, WritesAndReadsLines) {
    const auto file = tempPath("lines.txt");
    writeLines(file.string(), {"alpha", "beta"});
    const auto content = readFileContent(file.string());
    EXPECT_NE(content.find("alpha"), std::string::npos);
    EXPECT_NE(content.find("beta"), std::string::npos);
}

TEST(FilesystemUtilsTests, EnsuresDirectoryExists) {
    const auto dir = tempPath("dir");
    ensureDir(dir);
    EXPECT_TRUE(std::filesystem::exists(dir));
}

TEST(FilesystemUtilsTests, ConvertsClassnameToPath) {
    const auto path = classnameToPath("org/kapa/Main");
    EXPECT_EQ(path, std::filesystem::path("org/kapa/Main"));

}

TEST(FilesystemUtilsTests, BacksUpPreviousFile) {
    const auto file = tempPath("backup.txt");
    {
        std::ofstream out(file);
        out << "original";
    }
    const auto prevFile = file.string() + ".prev";
    // Precreate prev file to make sure backupPrevFile deletes it.
    {
        std::ofstream out(prevFile);
        out << "old";
    }

    backupPrevFile(file.string());

    EXPECT_FALSE(std::filesystem::exists(file));
    ASSERT_TRUE(std::filesystem::exists(prevFile));
    std::ifstream in(prevFile);
    std::string content;
    std::getline(in, content);
    EXPECT_EQ(content, "original");
}

TEST(FilesystemUtilsTests, StdoutFilePrintsContents) {
    const auto file = tempPath("stdout.txt");
    writeLines(file.string(), {"first", "second"});

    testing::internal::CaptureStdout();
    stdoutFile(file.string());
    const auto output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find("first"), std::string::npos);
    EXPECT_NE(output.find("second"), std::string::npos);
}

TEST(FilesystemUtilsTests, StdoutLinesPrintsEachLine) {
    testing::internal::CaptureStdout();
    stdoutLines({"one", "two"});
    const auto output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "one\ntwo\n");
}

TEST(FilesystemUtilsTests, LastWriteTimestampMatchesFilesystem) {
    const auto file = tempPath("timestamp.txt");
    writeLines(file.string(), {"data"});

    const auto expected = std::chrono::duration_cast<std::chrono::microseconds>(
        std::filesystem::last_write_time(file).time_since_epoch()).count();
    const auto actual = getLastWriteTimestamp(file.string());

    EXPECT_EQ(actual, expected);
}
