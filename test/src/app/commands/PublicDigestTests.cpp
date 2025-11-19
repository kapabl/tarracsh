#include <gtest/gtest.h>

#include <CLI/CLI.hpp>
#include <atomic>
#include <chrono>
#include <filesystem>
#include <memory>
#include <string>

#include "app/commands/digest/PublicDigest.h"
#include "app/server/digest/ServerCommand.h"
#include "test/src/app/runtime/TestRuntime.h"

#include "app/commands/Query.h"
#include "infrastructure/db/Database.h"
#include "infrastructure/log/Log.h"

using kapa::infrastructure::db::Database;
using kapa::infrastructure::log::Log;
using kapa::tarracsh::app::commands::digest::PublicDigest;
using kapa::tarracsh::app::commands::Query;
using kapa::tarracsh::app::server::digest::DiffRequest;
using kapa::tarracsh::app::server::digest::DiffResponse;
using kapa::tarracsh::app::server::digest::Empty;
using ServerPublicDigest = kapa::tarracsh::app::server::digest::PublicDigest;
using kapa::tarracsh::app::server::digest::ServerCommand;
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

struct FakeDigestStubState {
    grpc::Status nextDiffStatus{grpc::Status::OK};
    DiffRequest lastDiffRequest;
    bool diffCalled{false};
};

class FakeDigestStub : public ServerPublicDigest::StubInterface {
public:
    explicit FakeDigestStub(std::shared_ptr<FakeDigestStubState> sharedState)
        : state(std::move(sharedState)) {}

    grpc::Status Quit(grpc::ClientContext *context,
                      const Empty &request,
                      Empty *response) override {
        return grpc::Status(grpc::StatusCode::UNIMPLEMENTED, "not used");
    }

    grpc::Status Diff(grpc::ClientContext *context,
                      const DiffRequest &request,
                      DiffResponse *response) override {
        state->diffCalled = true;
        state->lastDiffRequest = request;
        return state->nextDiffStatus;
    }

private:
    std::shared_ptr<FakeDigestStubState> state;

    ::grpc::ClientAsyncResponseReaderInterface<Empty> *AsyncQuitRaw(
        ::grpc::ClientContext *, const Empty &, ::grpc::CompletionQueue *) override {
        return nullptr;
    }
    ::grpc::ClientAsyncResponseReaderInterface<Empty> *PrepareAsyncQuitRaw(
        ::grpc::ClientContext *, const Empty &, ::grpc::CompletionQueue *) override {
        return nullptr;
    }
    ::grpc::ClientAsyncResponseReaderInterface<DiffResponse> *AsyncDiffRaw(
        ::grpc::ClientContext *, const DiffRequest &, ::grpc::CompletionQueue *) override {
        return nullptr;
    }
    ::grpc::ClientAsyncResponseReaderInterface<DiffResponse> *PrepareAsyncDiffRaw(
        ::grpc::ClientContext *, const DiffRequest &, ::grpc::CompletionQueue *) override {
        return nullptr;
    }
};

class ScopedStubFactoryOverride {
public:
    explicit ScopedStubFactoryOverride(ServerCommand::StubFactory replacement)
        : previous(ServerCommand::setStubFactoryForTests(std::move(replacement))) {}

    ~ScopedStubFactoryOverride() {
        ServerCommand::setStubFactoryForTests(std::move(previous));
    }

private:
    ServerCommand::StubFactory previous;
};

class PublicDigestTest : public ::testing::Test {
protected:
    void SetUp() override {
        tempDir = makeTempDir("public-digest-tests-");
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

TEST_F(PublicDigestTest, RunExecutesQueryWhenQueryProvided) {
    CLI::App cli("tarracsh");
    PublicDigest command(&cli);
    command.addCommand();

    auto &options = runtime_test::options();
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

    auto &options = runtime_test::options();
    options.isPublicDigest = true;
    options.digest.input = (tempDir / "missing-dir").string();

    const auto exitCode = command.run();
    EXPECT_EQ(exitCode, 1);
}

TEST_F(PublicDigestTest, RunUsesClientModeWhenFlagEnabled) {
    CLI::App cli("tarracsh");
    PublicDigest command(&cli);
    command.addCommand();

    auto &options = runtime_test::options();
    options.isPublicDigest = true;
    options.digest.input = (tempDir / "valid-input").string();
    std::filesystem::create_directories(options.digest.input);
    options.digest.client.isClientMode = true;
    options.digest.dryRun = true;

    auto stubState = std::make_shared<FakeDigestStubState>();
    ScopedStubFactoryOverride stubGuard(
        [&](kapa::tarracsh::app::Context &) {
            return std::make_unique<FakeDigestStub>(stubState);
        });

    const auto exitCode = command.run();
    EXPECT_EQ(exitCode, 0);
    EXPECT_TRUE(stubState->diffCalled);
    EXPECT_EQ(stubState->lastDiffRequest.input(), options.digest.input);
    EXPECT_TRUE(stubState->lastDiffRequest.dryrun());
}

TEST_F(PublicDigestTest, RunProcessesInputWithLocalAnalyzer) {
    CLI::App cli("tarracsh");
    PublicDigest command(&cli);
    command.addCommand();

    auto &options = runtime_test::options();
    options.isPublicDigest = true;
    options.digest.input = (tempDir / "AstNodeRun.jar").string();
    std::filesystem::copy_file("test-subjects/jars/AstNode.jar",
                               options.digest.input,
                               std::filesystem::copy_options::overwrite_existing);
    options.digest.isDiff = true;

    auto &results = runtime_test::results();
    const auto initialJarCount = results.jarfiles.count.load();

    const auto exitCode = command.run();

    EXPECT_EQ(exitCode, 0);
    EXPECT_GT(results.jarfiles.count.load(), initialJarCount);
    EXPECT_FALSE(results.report->getClassResults().empty());
}
