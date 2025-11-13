#include <gtest/gtest.h>

#include <CLI/CLI.hpp>
#include <atomic>
#include <chrono>
#include <filesystem>
#include <memory>
#include <string>

#define private public
#define protected public
#include "interfaces/tarracsh/App.h"
#include "app/commands/digest/PublicDigest.h"
#undef private
#undef protected

#include "app/commands/Query.h"
#include "infrastructure/db/Database.h"
#include "infrastructure/log/Log.h"

using kapa::infrastructure::db::Database;
using kapa::infrastructure::log::Log;
using kapa::tarracsh::app::App;
using kapa::tarracsh::app::commands::digest::PublicDigest;
using kapa::tarracsh::app::commands::Query;

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

class PublicDigestTest : public ::testing::Test {
protected:
    void SetUp() override {
        tempDir = makeTempDir("public-digest-tests-");
        log = std::make_shared<Log>();
        const auto logFile = (tempDir / "test.log").string();
        log->init(logFile);
        App::_app = std::make_unique<App>("", "tarracsh", log);
        auto &options = App::_app->_options;
        options.outputDir = tempDir.string();
        options.logFile = (tempDir / "output.log").string();
        App::_app->_results.log = log;
    }

    void TearDown() override {
        App::_app.reset();
        std::error_code ec;
        std::filesystem::remove_all(tempDir, ec);
    }

    std::filesystem::path tempDir;
    std::shared_ptr<Log> log;
};

} // namespace

TEST_F(PublicDigestTest, RunExecutesQueryWhenQueryProvided) {
    CLI::App cli("tarracsh");
    PublicDigest command(&cli);
    command.addCommand();

    auto &options = App::_app->_options;
    options.isPublicDigest = true;
    options.digest.queryValue = "list files";
    options.digest.displayRaw = true;

    bool factoryCalled = false;
    bool executorCalled = false;

    ScopedFactoryOverride factoryGuard(
        [&](const Database::Config &config, bool useDigest) {
            factoryCalled = true;
            EXPECT_TRUE(useDigest);
            auto db = std::make_unique<FakeDatabase>(config);
            return db;
        });

    ScopedExecutorOverride executorGuard(
        [&](Database &db, const std::string &query, bool displayRaw) {
            executorCalled = true;
            EXPECT_EQ(query, "list files");
            EXPECT_TRUE(displayRaw);
            return true;
        });

    const auto exitCode = command.run();
    EXPECT_EQ(exitCode, 0);
    EXPECT_TRUE(factoryCalled);
    EXPECT_TRUE(executorCalled);
}

TEST_F(PublicDigestTest, DigestInputFailsForInvalidInputPath) {
    CLI::App cli("tarracsh");
    PublicDigest command(&cli);
    command.addCommand();

    auto &options = App::_app->_options;
    options.isPublicDigest = true;
    options.digest.input = (tempDir / "missing-dir").string();

    const auto exitCode = command.digestInput();
    EXPECT_EQ(exitCode, 1);
}
