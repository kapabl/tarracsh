#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>

#include "domain/Options.h"

using namespace kapa::tarracsh::domain;

namespace {

std::filesystem::path createTempFile(const std::string &suffix) {
    const auto tempDir = std::filesystem::temp_directory_path();
    std::filesystem::path temp;
    do {
        temp = tempDir / ("tarracsh-" + std::to_string(std::rand()) + suffix);
    } while (std::filesystem::exists(temp));
    std::ofstream(temp).close();
    return temp;
}

} // namespace

TEST(Options, CanPrintProgressTrueWhenNoVerboseFlags) {
    Options options;
    options.parse.print = false;
    options.parse.printConstantPool = false;
    options.digest.isDiff = false;
    options.digest.server.isServerMode = false;

    EXPECT_TRUE(options.canPrintProgress());
}

TEST(Options, CanPrintProgressFalseWhenAnyVerboseFlagSet) {
    Options options;
    options.parse.print = true;
    EXPECT_FALSE(options.canPrintProgress());
}

TEST(Options, GetSubCommandOptionsResolvesActiveMode) {
    Options options;
    options.isCallGraph = false;
    options.isPublicDigest = true;
    EXPECT_EQ(&options.getSubCommandOptions(), &options.digest);

    options.isPublicDigest = false;
    options.isCallGraph = true;
    EXPECT_EQ(&options.getSubCommandOptions(), &options.callGraph);

    options.isCallGraph = false;
    EXPECT_EQ(&options.getSubCommandOptions(), &options.parse);
}

TEST(SubCommandOptions, ProcessInputDirectorySetsFlag) {
    SubCommandOptions subOptions;
    subOptions.input = std::filesystem::temp_directory_path().string();
    EXPECT_TRUE(subOptions.processInput());
    EXPECT_TRUE(subOptions.isDir);
    EXPECT_FALSE(subOptions.isJar);
    EXPECT_FALSE(subOptions.isClassFile);
}

TEST(SubCommandOptions, ProcessInputRecognizesJar) {
    const auto tempFile = createTempFile(".jar");
    SubCommandOptions subOptions;
    subOptions.input = tempFile.string();
    EXPECT_TRUE(subOptions.processInput());
    EXPECT_TRUE(subOptions.isJar);
    EXPECT_FALSE(subOptions.isDir);
    EXPECT_FALSE(subOptions.isClassFile);
    std::filesystem::remove(tempFile);
}

TEST(SubCommandOptions, ProcessInputRecognizesClassFile) {
    const auto tempFile = createTempFile(".class");
    SubCommandOptions subOptions;
    subOptions.input = tempFile.string();
    EXPECT_TRUE(subOptions.processInput());
    EXPECT_TRUE(subOptions.isClassFile);
    EXPECT_FALSE(subOptions.isJar);
    EXPECT_FALSE(subOptions.isDir);
    std::filesystem::remove(tempFile);
}

TEST(SubCommandOptions, InvalidInputReturnsFalse) {
    SubCommandOptions subOptions;
    subOptions.input = "/path/that/does/not/exist";
    EXPECT_FALSE(subOptions.isValidInput());
}

TEST(ServerOptions, FormatsListenAddress) {
    ServerOptions options;
    options.listenAddress = "127.0.0.1";
    options.port = 8080;
    EXPECT_EQ(options.getListenServerAddress(), "127.0.0.1:8080");
}

TEST(ClientOptions, FormatsServerAddress) {
    ClientOptions options;
    options.host = "example.com";
    options.port = 9090;
    EXPECT_EQ(options.getServerAddress(), "example.com:9090");
}
