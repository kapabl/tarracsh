#include <gtest/gtest.h>

#include <filesystem>
#include <chrono>

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
