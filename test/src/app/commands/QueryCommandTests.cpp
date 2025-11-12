#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
#include <filesystem>
#include <memory>
#include <optional>
#include <string>
#include <utility>

#include "app/Context.h"
#include "app/commands/Query.h"
#include "domain/Options.h"
#include "domain/stats/Results.h"
#include "infrastructure/db/Database.h"
#include "infrastructure/log/Log.h"

using namespace kapa::tarracsh;
using namespace app;
using namespace app::commands;
using kapa::infrastructure::log::Log;
using kapa::infrastructure::db::Database;

namespace {

std::filesystem::path makeTempDir(const std::string &prefix) {
    static std::atomic<uint64_t> counter{0};
    auto base = std::filesystem::temp_directory_path();
    const auto now = std::chrono::steady_clock::now().time_since_epoch().count();
    auto dir = base / (prefix + std::to_string(now) + "-" + std::to_string(counter.fetch_add(1)));
    std::filesystem::create_directories(dir);
    return dir;
}

class StubContext : public Context {
public:
    StubContext()
        : options(), results(options) {
        log = std::make_shared<Log>();
        results.log = log;
    }

    domain::stats::Results &getResults() override { return results; }
    domain::Options &getOptions() override { return options; }
    Log &getLog() override { return *log; }

    domain::Options options;
    domain::stats::Results results;
    std::shared_ptr<Log> log;
};

class FakeDatabase : public Database {
public:
    explicit FakeDatabase(const Config &config)
        : Database(config, false), dataDir(config.dataDir) {
    }

    void init() override { initCalled = true; }

    bool initCalled{false};
    std::string dataDir;
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

} // namespace

TEST(QueryCommandTests, UsesDigestDatabaseWhenFlagSet) {
    StubContext context;
    const auto outputDir = makeTempDir("query-digest-");
    context.options.outputDir = outputDir.string();
    context.options.logFile = (outputDir / "digest.log").string();
    context.options.isPublicDigest = true;
    context.options.digest.queryValue = "list files";
    context.options.digest.displayRaw = true;

    bool factorySawDigest = false;
    FakeDatabase *lastDb = nullptr;

    ScopedFactoryOverride factoryGuard(
        [&](const Database::Config &config, const bool useDigest) {
            factorySawDigest = useDigest;
            auto db = std::make_unique<FakeDatabase>(config);
            lastDb = db.get();
            return db;
        });

    bool executorCalled = false;
    std::optional<std::string> observedDataDir;
    bool observedInitFlag = false;
    ScopedExecutorOverride executorGuard(
        [&](Database &db, const std::string &query, const bool displayRaw) {
            executorCalled = true;
            auto *fake = dynamic_cast<FakeDatabase *>(&db);
            EXPECT_NE(fake, nullptr);
            if (fake == nullptr) {
                return false;
            }
            observedInitFlag = fake->initCalled;
            observedDataDir = fake->dataDir;
            EXPECT_EQ(query, "list files");
            EXPECT_TRUE(displayRaw);
            return true;
        });

    const auto success = Query::run(context);
    EXPECT_TRUE(success);
    EXPECT_TRUE(factorySawDigest);
    EXPECT_TRUE(executorCalled);
    ASSERT_TRUE(observedDataDir.has_value());
    EXPECT_TRUE(observedInitFlag);
    EXPECT_EQ(*observedDataDir, context.options.outputDir);

    std::filesystem::remove_all(outputDir);
}

TEST(QueryCommandTests, UsesCallGraphDatabaseOtherwiseAndPropagatesExecutorResult) {
    StubContext context;
    const auto outputDir = makeTempDir("query-callgraph-");
    context.options.outputDir = outputDir.string();
    context.options.logFile = (outputDir / "callgraph.log").string();
    context.options.isPublicDigest = false;
    context.options.isCallGraph = true;
    context.options.callGraph.queryValue = "list methods";

    bool factorySawDigest = true;
    ScopedFactoryOverride factoryGuard(
        [&](const Database::Config &config, const bool useDigest) {
            factorySawDigest = useDigest;
            auto db = std::make_unique<FakeDatabase>(config);
            return db;
        });

    ScopedExecutorOverride executorGuard(
        [&](Database &db, const std::string &query, const bool displayRaw) {
            auto *fake = dynamic_cast<FakeDatabase *>(&db);
            EXPECT_NE(fake, nullptr);
            if (fake == nullptr) {
                return false;
            }
            EXPECT_EQ(query, "list methods");
            EXPECT_FALSE(displayRaw);
            return false;
        });

    const auto success = Query::run(context);
    EXPECT_FALSE(success);
    EXPECT_FALSE(factorySawDigest);

    std::filesystem::remove_all(outputDir);
}
