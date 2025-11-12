#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#define private public
#define protected public
#include "interfaces/tarracsh/App.h"
#undef private
#undef protected

#include "infrastructure/log/Log.h"

using kapa::tarracsh::app::App;

namespace {

std::filesystem::path makeTempDir(const std::string &prefix) {
    static std::atomic<uint64_t> counter{0};
    const auto base = std::filesystem::temp_directory_path();
    const auto now = std::chrono::steady_clock::now().time_since_epoch().count();
    auto dir = base / (prefix + std::to_string(now) + "-" + std::to_string(counter.fetch_add(1)));
    std::filesystem::create_directories(dir);
    return dir;
}

std::vector<char *> makeArgv(std::vector<std::string> &args) {
    std::vector<char *> argv;
    argv.reserve(args.size());
    for (auto &arg : args) {
        argv.push_back(arg.data());
    }
    return argv;
}

class AppTestFixture : public ::testing::Test {
protected:
    void SetUp() override {
        originalHome = std::getenv("HOME") ? std::optional<std::string>(std::getenv("HOME")) : std::nullopt;
        tempHome = makeTempDir("tarracsh-app-home-");
        setenv("HOME", tempHome.string().c_str(), 1);

        log = std::make_shared<kapa::infrastructure::log::Log>();
        log->init((tempHome / "test.log").string());

        App::_app = std::make_unique<App>("", "tarracsh", log);
    }

    void TearDown() override {
        App::_app.reset();
        if (originalHome) {
            setenv("HOME", originalHome->c_str(), 1);
        } else {
            unsetenv("HOME");
        }
        std::error_code ec;
        std::filesystem::remove_all(tempHome, ec);
    }

    std::shared_ptr<kapa::infrastructure::log::Log> log;
    std::optional<std::string> originalHome;
    std::filesystem::path tempHome;
};

}  // namespace

TEST_F(AppTestFixture, ParsesDigestQueryAndSetsDefaultPaths) {
    ASSERT_NE(nullptr, App::_app);
    auto &app = *App::_app;

    std::vector<std::string> args = {"tarracsh", "public-digest", "--query", "list files"};
    auto argv = makeArgv(args);

    const auto exitCode = app.parseCli(static_cast<int>(args.size()), argv.data());
    EXPECT_EQ(0, exitCode);
    EXPECT_TRUE(app._options.isPublicDigest);
    EXPECT_EQ("list files", app._options.digest.queryValue);

    const auto expectedOutput = tempHome / "tarracsh" / "output";
    EXPECT_EQ(expectedOutput, std::filesystem::path(app._options.outputDir));
    EXPECT_EQ(expectedOutput / "output.log", std::filesystem::path(app._options.logFile));
}

TEST_F(AppTestFixture, StartFailsWhenNoSubcommandIsProvided) {
    ASSERT_NE(nullptr, App::_app);
    auto &app = *App::_app;

    std::vector<std::string> args = {"tarracsh"};
    auto argv = makeArgv(args);

    const auto exitCode = app.start(static_cast<int>(args.size()), argv.data());
    EXPECT_EQ(1, exitCode);
}

TEST_F(AppTestFixture, ParseSubcommandHonorsPauseFlag) {
    ASSERT_NE(nullptr, App::_app);
    auto &app = *App::_app;

    std::vector<std::string> args = {
        "tarracsh",
        "--pause",
        "parse",
        "--input",
        "/tmp/sample.jar",
    };
    auto argv = makeArgv(args);

    const auto exitCode = app.parseCli(static_cast<int>(args.size()), argv.data());
    EXPECT_EQ(0, exitCode);
    EXPECT_TRUE(app._options.isParse);
    EXPECT_TRUE(app._options.pause);
    EXPECT_EQ("/tmp/sample.jar", app._options.parse.input);
}
