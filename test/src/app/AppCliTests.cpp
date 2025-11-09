#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <optional>
#include <string>
#include <vector>

#include "interfaces/tarracsh/App.h"
#include "infrastructure/filesystem/Utils.h"

using namespace kapa::tarracsh::app;

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

class ScopedEnvVar {
public:
    ScopedEnvVar(std::string name, const std::string &value)
        : _name(std::move(name)) {
        const char *current = std::getenv(_name.c_str());
        if (current != nullptr) {
            _original = current;
        }
        setenv(_name.c_str(), value.c_str(), 1);
    }

    ~ScopedEnvVar() {
        if (_original) {
            setenv(_name.c_str(), _original->c_str(), 1);
        } else {
            unsetenv(_name.c_str());
        }
    }

private:
    std::string _name;
    std::optional<std::string> _original;
};

} // namespace

TEST(AppCliTests, ParseCommandSetsOptionsAndFailsOnInvalidInput) {
    const auto tempHome = makeTempDir("tarracsh-app-parse-");
    ScopedEnvVar homeGuard("HOME", tempHome.string());
    const auto missingInput = tempHome / "missing.class";

    std::vector<std::string> args{
        "tarracsh",
        "parse",
        "--input", missingInput.string()
    };
    auto argv = makeArgv(args);

    const auto exitCode = App::run(static_cast<int>(argv.size()), argv.data());
    EXPECT_EQ(exitCode, 1);

    const auto &options = App::getGlobalOptions();
    EXPECT_TRUE(options.isParse);
    EXPECT_FALSE(options.isPublicDigest);
    EXPECT_FALSE(options.isCallGraph);
    const auto expectedOutputDir = (tempHome / "tarracsh" / "output").string();
    EXPECT_EQ(options.outputDir, expectedOutputDir);
    EXPECT_EQ(options.logFile, (std::filesystem::path(expectedOutputDir) / "output.log").string());
    EXPECT_EQ(options.parse.input, missingInput.string());

    std::filesystem::remove_all(tempHome);
}

TEST(AppCliTests, CallGraphCommandMarksCallGraphMode) {
    const auto tempHome = makeTempDir("tarracsh-app-callgraph-");
    ScopedEnvVar homeGuard("HOME", tempHome.string());
    const auto missingInput = tempHome / "missing.jar";

    std::vector<std::string> args{
        "tarracsh",
        "call-graph",
        "--input", missingInput.string()
    };
    auto argv = makeArgv(args);

    const auto exitCode = App::run(static_cast<int>(argv.size()), argv.data());
    EXPECT_EQ(exitCode, 1);

    const auto &options = App::getGlobalOptions();
    EXPECT_TRUE(options.isCallGraph);
    EXPECT_FALSE(options.isParse);
    EXPECT_FALSE(options.isPublicDigest);
    EXPECT_EQ(options.callGraph.input, missingInput.string());
    const auto expectedOutputDir = (tempHome / "tarracsh" / "output").string();
    EXPECT_EQ(options.outputDir, expectedOutputDir);
    EXPECT_EQ(options.logFile, (std::filesystem::path(expectedOutputDir) / "output.log").string());

    std::filesystem::remove_all(tempHome);
}
