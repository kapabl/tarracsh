#include <gtest/gtest.h>

#include <chrono>
#include <filesystem>
#include <memory>
#include <string>

#include "app/server/digest/ServerCommand.h"

#include "app/Context.h"
#include "domain/Options.h"
#include "domain/stats/Results.h"
#include "infrastructure/log/Log.h"

using kapa::infrastructure::log::Log;
using kapa::tarracsh::app::Context;
using kapa::tarracsh::app::server::digest::ServerCommand;
using kapa::tarracsh::app::server::digest::DiffRequest;
using kapa::tarracsh::app::server::digest::DiffResponse;
using kapa::tarracsh::app::server::digest::Empty;
using kapa::tarracsh::app::server::digest::PublicDigest;
using kapa::tarracsh::domain::Options;
using kapa::tarracsh::domain::stats::Results;

namespace {

std::filesystem::path makeTempLogPath(const std::string &prefix) {
    const auto base = std::filesystem::temp_directory_path();
    const auto now = std::chrono::steady_clock::now().time_since_epoch().count();
    auto path = base / (prefix + "-" + std::to_string(now) + ".log");
    return path;
}

class StubContext : public Context {
public:
    StubContext()
        : results(options) {
        log = std::make_shared<Log>();
        const auto logPath = makeTempLogPath("server-command-tests");
        log->init(logPath.string());
        results.log = log;
        options.logFile = logPath.string();
    }

    Results &getResults() override { return results; }
    Options &getOptions() override { return options; }
    Log &getLog() override { return *log; }

    Options options;
    Results results;
    std::shared_ptr<Log> log;
};

struct FakeDigestStubState {
    grpc::Status nextDiffStatus{grpc::Status::OK};
    grpc::Status nextQuitStatus{grpc::Status::OK};
    DiffResponse templateResponse;
    DiffRequest lastDiffRequest;
    bool diffCalled{false};
    bool quitCalled{false};
};

class FakeDigestStub : public PublicDigest::StubInterface {
public:
    explicit FakeDigestStub(std::shared_ptr<FakeDigestStubState> sharedState)
        : state(std::move(sharedState)) {}

    grpc::Status Quit(grpc::ClientContext *context,
                      const Empty &request,
                      Empty *response) override {
        state->quitCalled = true;
        return state->nextQuitStatus;
    }

    grpc::Status Diff(grpc::ClientContext *context,
                      const DiffRequest &request,
                      DiffResponse *response) override {
        state->diffCalled = true;
        state->lastDiffRequest = request;
        *response = state->templateResponse;
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

class ScopedServerStarterOverride {
public:
    explicit ScopedServerStarterOverride(ServerCommand::ServerStarter replacement)
        : previous(ServerCommand::setServerStarterForTests(std::move(replacement))) {}

    ~ScopedServerStarterOverride() {
        ServerCommand::setServerStarterForTests(std::move(previous));
    }

private:
    ServerCommand::ServerStarter previous;
};

} // namespace

TEST(ServerCommandTests, DiffPopulatesReportOnSuccessfulResponse) {
    StubContext context;
    context.options.digest.client.isClientMode = true;
    context.options.digest.input = (std::filesystem::temp_directory_path() / "server-command-diff.jar").string();
    context.options.digest.dryRun = true;

    auto stubState = std::make_shared<FakeDigestStubState>();
    ScopedStubFactoryOverride stubGuard(
        [&](Context &) {
            auto stub = std::make_unique<FakeDigestStub>(stubState);
            auto *file = stubState->templateResponse.add_files();
            file->set_filename("lib.jar");
            file->set_isnew(true);
            file->set_issamedigest(false);
            file->set_failed(false);
            return stub;
        });

    const auto result = ServerCommand::run(context);

    EXPECT_TRUE(result);
    EXPECT_TRUE(stubState->diffCalled);
    EXPECT_EQ(stubState->lastDiffRequest.input(), context.options.digest.input);
    EXPECT_TRUE(stubState->lastDiffRequest.dryrun());

    const auto &files = context.results.report->getFileResults();
    ASSERT_EQ(files.size(), 1);
    EXPECT_EQ(files[0].filename, "lib.jar");
    EXPECT_TRUE(files[0].isNew);
    EXPECT_FALSE(files[0].isSamePublicDigest);
}

TEST(ServerCommandTests, DiffReturnsFalseWhenRequestFails) {
    StubContext context;
    context.options.digest.client.isClientMode = true;
    context.options.digest.input = (std::filesystem::temp_directory_path() / "server-command-diff-fail.jar").string();

    auto stubState = std::make_shared<FakeDigestStubState>();
    stubState->nextDiffStatus = grpc::Status(grpc::StatusCode::UNAVAILABLE, "offline");
    ScopedStubFactoryOverride stubGuard(
        [&](Context &) {
            auto stub = std::make_unique<FakeDigestStub>(stubState);
            return stub;
        });

    const auto result = ServerCommand::run(context);

    EXPECT_FALSE(result);
    EXPECT_TRUE(stubState->diffCalled);
    EXPECT_TRUE(context.results.report->getFileResults().empty());
}

TEST(ServerCommandTests, DiffPropagatesDryRunFlag) {
    StubContext context;
    context.options.digest.client.isClientMode = true;
    context.options.digest.input = (std::filesystem::temp_directory_path() / "server-command-dryrun.jar").string();
    context.options.digest.dryRun = true;

    auto stubState = std::make_shared<FakeDigestStubState>();
    ScopedStubFactoryOverride stubGuard(
        [&](Context &) {
            return std::make_unique<FakeDigestStub>(stubState);
        });

    const auto result = ServerCommand::run(context);

    EXPECT_TRUE(result);
    EXPECT_TRUE(stubState->diffCalled);
    EXPECT_TRUE(stubState->lastDiffRequest.dryrun());
}

TEST(ServerCommandTests, StopReturnsTrueWhenServerAcknowledgesQuit) {
    StubContext context;
    context.options.digest.server.stopServer = true;

    auto stubState = std::make_shared<FakeDigestStubState>();
    ScopedStubFactoryOverride stubGuard(
        [&](Context &) {
            return std::make_unique<FakeDigestStub>(stubState);
        });

    const auto result = ServerCommand::run(context);

    EXPECT_TRUE(result);
    EXPECT_TRUE(stubState->quitCalled);
}

TEST(ServerCommandTests, StopReturnsFalseWhenQuitFails) {
    StubContext context;
    context.options.digest.server.stopServer = true;

    auto stubState = std::make_shared<FakeDigestStubState>();
    stubState->nextQuitStatus = grpc::Status(grpc::StatusCode::UNAVAILABLE, "busy");
    ScopedStubFactoryOverride stubGuard(
        [&](Context &) {
            return std::make_unique<FakeDigestStub>(stubState);
        });

    const auto result = ServerCommand::run(context);

    EXPECT_FALSE(result);
    EXPECT_TRUE(stubState->quitCalled);
}

TEST(ServerCommandTests, RunUsesStopWhenServerStopFlagSet) {
    StubContext context;
    context.options.digest.server.stopServer = true;

    auto stubState = std::make_shared<FakeDigestStubState>();
    ScopedStubFactoryOverride stubGuard(
        [&](Context &) {
            return std::make_unique<FakeDigestStub>(stubState);
        });

    const auto result = ServerCommand::run(context);
    EXPECT_TRUE(result);
    EXPECT_TRUE(stubState->quitCalled);
}

TEST(ServerCommandTests, RunPrefersStopOverClientWhenBothFlagsSet) {
    StubContext context;
    context.options.digest.server.stopServer = true;
    context.options.digest.client.isClientMode = true;

    auto stubState = std::make_shared<FakeDigestStubState>();
    ScopedStubFactoryOverride stubGuard(
        [&](Context &) {
            return std::make_unique<FakeDigestStub>(stubState);
        });

    const auto result = ServerCommand::run(context);
    EXPECT_TRUE(result);
    EXPECT_TRUE(stubState->quitCalled);
    EXPECT_FALSE(stubState->diffCalled);
}

TEST(ServerCommandTests, RunUsesDiffWhenClientModeEnabled) {
    StubContext context;
    context.options.digest.client.isClientMode = true;
    context.options.digest.input = "/tmp/input.jar";

    auto stubState = std::make_shared<FakeDigestStubState>();
    ScopedStubFactoryOverride stubGuard(
        [&](Context &) {
            auto stub = std::make_unique<FakeDigestStub>(stubState);
            auto *file = stubState->templateResponse.add_files();
            file->set_filename("lib.jar");
            file->set_isnew(true);
            return stub;
        });

    const auto result = ServerCommand::run(context);
    EXPECT_TRUE(result);
    EXPECT_TRUE(stubState->diffCalled);
    EXPECT_FALSE(stubState->lastDiffRequest.dryrun());
}

TEST(ServerCommandTests, RunStartsServerWhenNoClientOrStopFlags) {
    StubContext context;
    bool starterCalled = false;

    auto stubState = std::make_shared<FakeDigestStubState>();
    ScopedStubFactoryOverride stubGuard(
        [&](Context &) {
            return std::make_unique<FakeDigestStub>(stubState);
        });

    ScopedServerStarterOverride starterGuard(
        [&](Context &) {
            starterCalled = true;
            return true;
        });

    const auto result = ServerCommand::run(context);
    EXPECT_TRUE(result);
    EXPECT_TRUE(starterCalled);
    EXPECT_FALSE(stubState->diffCalled);
    EXPECT_FALSE(stubState->quitCalled);
}

TEST(ServerCommandTests, DiffPopulatesClassResults) {
    StubContext context;
    context.options.digest.client.isClientMode = true;
    context.options.digest.input = (std::filesystem::temp_directory_path() / "class-results-input.jar").string();

    auto stubState = std::make_shared<FakeDigestStubState>();
    auto *clazz = stubState->templateResponse.add_classfiles();
    clazz->set_strongclassname("com.example.Foo");
    clazz->set_isnew(false);
    clazz->set_issamedigest(false);
    clazz->set_ismodified(true);

    ScopedStubFactoryOverride stubGuard(
        [&](Context &) {
            return std::make_unique<FakeDigestStub>(stubState);
        });

    const auto result = ServerCommand::run(context);
    EXPECT_TRUE(result);
    EXPECT_TRUE(stubState->diffCalled);

    const auto &classes = context.results.report->getClassResults();
    ASSERT_EQ(classes.size(), 1);
    EXPECT_EQ(classes[0].strongClassname, "com.example.Foo");
    EXPECT_TRUE(classes[0].isModified);
    EXPECT_FALSE(classes[0].isSamePublicDigest);
}

TEST(ServerCommandTests, RunReturnsFalseWhenServerStarterFails) {
    StubContext context;
    bool starterCalled = false;

    auto stubState = std::make_shared<FakeDigestStubState>();
    ScopedStubFactoryOverride stubGuard(
        [&](Context &) {
            return std::make_unique<FakeDigestStub>(stubState);
        });

    ScopedServerStarterOverride starterGuard(
        [&](Context &) {
            starterCalled = true;
            return false;
        });

    const auto result = ServerCommand::run(context);
    EXPECT_FALSE(result);
    EXPECT_TRUE(starterCalled);
}
