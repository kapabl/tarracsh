#include <gtest/gtest.h>

#include <CLI/CLI.hpp>
#include <atomic>
#include <chrono>
#include <filesystem>
#include <memory>
#include <string>

#include "app/commands/callgraph/CallGraph.h"
#include "test/src/app/runtime/TestRuntime.h"

#include "app/commands/Query.h"
#include "infrastructure/db/Database.h"
#include "infrastructure/log/Log.h"

using kapa::infrastructure::db::Database;
using kapa::infrastructure::log::Log;
using kapa::tarracsh::app::commands::Query;
using kapa::tarracsh::app::commands::callgraph::CallGraph;
namespace runtime = kapa::tarracsh::app::runtime;
namespace runtime_test = kapa::tarracsh::app::runtime::test;

namespace {

std::filesystem::path makeTempDir(const std::string &prefix) {
    static std::atomic<uint64_t> counter{0};
    const auto base = std::filesystem::temp_directory_path();
    const auto now = std::chrono::steady_clock::now().time_since_epoch().count();
    auto dir = base / (prefix + std::to_string(now) + "-" + std::to_string(counter.fetch_add(1)));
    std::filesystem::create_directories(dir);
    return dir;
}

class FakeDatabase : public Database {
public:
    explicit FakeDatabase(const Database::Config &config)
        : Database(config, false) {}

    void init() override { initCalled = true; }
    bool initCalled{false};
};

class ScopedFactoryOverride {
public:
    explicit ScopedFactoryOverride(Query::DatabaseFactory replacement)
        : previous(Query::setDatabaseFactoryForTests(std::move(replacement))) {}

    ~ScopedFactoryOverride() {
        Query::setDatabaseFactoryForTests(std::move(previous));
    }

private:
    Query::DatabaseFactory previous;
};

class ScopedExecutorOverride {
public:
    explicit ScopedExecutorOverride(Query::QueryExecutor replacement)
        : previous(Query::setQueryExecutorForTests(std::move(replacement))) {}

    ~ScopedExecutorOverride() {
        Query::setQueryExecutorForTests(std::move(previous));
    }

private:
    Query::QueryExecutor previous;
};

class CallGraphCommandTest : public ::testing::Test {
protected:
    void SetUp() override {
        tempDir = makeTempDir("callgraph-command-tests-");
        log = std::make_shared<Log>();
        const auto logFile = (tempDir / "test.log").string();
        log->init(logFile);
        runtime_test::reset(log);
        auto &options = runtime_test::options();
        options.outputDir = tempDir.string();
        options.logFile = (tempDir / "output.log").string();
        runtime_test::results().log = log;
    }

    void TearDown() override {
        runtime_test::reset();
        std::error_code ec;
        std::filesystem::remove_all(tempDir, ec);
    }

    std::filesystem::path tempDir;
    std::shared_ptr<Log> log;
};

} // namespace

TEST_F(CallGraphCommandTest, RunExecutesQueryWhenProvided) {
    CLI::App cli("tarracsh");
    CallGraph command(&cli);
    command.addCommand();

    auto &options = runtime_test::options();
    options.isCallGraph = true;
    options.callGraph.queryValue = "list methods";
    options.callGraph.displayRaw = true;

    bool factoryCalled = false;
    bool executorCalled = false;

    ScopedFactoryOverride factoryGuard(
        [&](const Database::Config &config, bool useDigest) {
            factoryCalled = true;
            EXPECT_FALSE(useDigest);
            auto db = std::make_unique<FakeDatabase>(config);
            return db;
        });

    ScopedExecutorOverride executorGuard(
        [&](Database &db, const std::string &query, bool displayRaw) {
            executorCalled = true;
            EXPECT_EQ(query, "list methods");
            EXPECT_TRUE(displayRaw);
            return true;
        });

    const auto exitCode = command.run();
    EXPECT_EQ(exitCode, 0);
    EXPECT_TRUE(factoryCalled);
    EXPECT_TRUE(executorCalled);
}

TEST_F(CallGraphCommandTest, RunFailsWhenInputIsInvalid) {
    CLI::App cli("tarracsh");
    CallGraph command(&cli);
    command.addCommand();

    auto &options = runtime_test::options();
    options.isCallGraph = true;
    options.callGraph.input = (tempDir / "missing.jar").string();

    const auto exitCode = command.run();
    EXPECT_EQ(exitCode, 1);
}

TEST_F(CallGraphCommandTest, RunProcessesInputWhenClientModeDisabled) {
    CLI::App cli("tarracsh");
    CallGraph command(&cli);
    command.addCommand();

    auto &options = runtime_test::options();
    options.isCallGraph = true;
    auto inputDir = tempDir / "input-dir";
    std::filesystem::create_directories(inputDir);
    options.callGraph.input = inputDir.string();
    options.callGraph.client.isClientMode = false;

    const auto exitCode = command.run();
    EXPECT_EQ(exitCode, 0);
}

TEST_F(CallGraphCommandTest, RunFallsBackToClientModeWhenClientModeEnabled) {
    CLI::App cli("tarracsh");
    CallGraph command(&cli);
    command.addCommand();

    auto &options = runtime_test::options();
    options.isCallGraph = true;
    auto inputDir = tempDir / "client-input";
    std::filesystem::create_directories(inputDir);
    options.callGraph.input = inputDir.string();
    options.callGraph.client.isClientMode = true;

    const auto exitCode = command.run();
    EXPECT_EQ(exitCode, 1);
}

TEST_F(CallGraphCommandTest, RunUsesServerModeWhenServerFlagSet) {
    CLI::App cli("tarracsh");
    CallGraph command(&cli);
    command.addCommand();

    auto &options = runtime_test::options();
    options.isCallGraph = true;
    options.callGraph.server.isServerMode = true;

    const auto exitCode = command.run();
    EXPECT_EQ(exitCode, 0);
}
