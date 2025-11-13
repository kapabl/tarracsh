#include <gtest/gtest.h>

#include <chrono>
#include <filesystem>
#include <memory>
#include <string>

#define private public
#define protected public
#include "app/server/digest/ServerCommand.h"
#undef private
#undef protected

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

class FakeDigestStub : public PublicDigest::StubInterface {
public:
    grpc::Status nextDiffStatus{grpc::Status::OK};
    grpc::Status nextQuitStatus{grpc::Status::OK};
    DiffResponse templateResponse;
    DiffRequest lastDiffRequest;
    bool diffCalled{false};
    bool quitCalled{false};

    grpc::Status Quit(grpc::ClientContext *context,
                      const Empty &request,
                      Empty *response) override {
        quitCalled = true;
        return nextQuitStatus;
    }

    grpc::Status Diff(grpc::ClientContext *context,
                      const DiffRequest &request,
                      DiffResponse *response) override {
        diffCalled = true;
        lastDiffRequest = request;
        *response = templateResponse;
        return nextDiffStatus;
    }

private:
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

TEST(ServerCommandTests, OptionsToRequestCopiesDigestFields) {
    StubContext context;
    context.options.digest.input = "/tmp/sample.jar";
    context.options.digest.dryRun = true;

    ServerCommand command(context);
    DiffRequest request;
    command.optionsToRequest(request);

    EXPECT_EQ(request.input(), context.options.digest.input);
    EXPECT_TRUE(request.dryrun());
}

TEST(ServerCommandTests, ResponseToReportTransfersFileAndClassSummaries) {
    StubContext context;
    context.options.isPublicDigest = true;

    ServerCommand command(context);
    DiffResponse response;
    auto *file = response.add_files();
    file->set_filename("lib.jar");
    file->set_isnew(true);
    file->set_ismodified(false);
    file->set_issamedigest(false);
    file->set_failed(true);

    auto *clazz = response.add_classfiles();
    clazz->set_strongclassname("pkg/Type");
    clazz->set_ismodified(true);
    clazz->set_isnew(false);
    clazz->set_issamedigest(true);
    clazz->set_failed(false);

    command.responseToReport(response);

    const auto &files = context.results.report->getFileResults();
    ASSERT_EQ(files.size(), 1);
    EXPECT_EQ(files[0].filename, "lib.jar");
    EXPECT_TRUE(files[0].isNew);
    EXPECT_TRUE(files[0].failed);
    EXPECT_FALSE(files[0].isSamePublicDigest);

    const auto &classes = context.results.report->getClassResults();
    ASSERT_EQ(classes.size(), 1);
    EXPECT_EQ(classes[0].strongClassname, "pkg/Type");
    EXPECT_TRUE(classes[0].isModified);
    EXPECT_TRUE(classes[0].isSamePublicDigest);
    EXPECT_FALSE(classes[0].failed);
}

TEST(ServerCommandTests, DiffPopulatesReportOnSuccessfulResponse) {
    StubContext context;
    context.options.digest.input = "/tmp/input.jar";
    context.options.digest.dryRun = true;

    ServerCommand command(context);
    auto fakeStub = std::make_unique<FakeDigestStub>();
    auto *stubPtr = fakeStub.get();
    auto *file = stubPtr->templateResponse.add_files();
    file->set_filename("lib.jar");
    file->set_isnew(true);
    file->set_issamedigest(false);
    file->set_failed(false);
    command._stub = std::move(fakeStub);

    EXPECT_TRUE(command.diff());
    EXPECT_TRUE(stubPtr->diffCalled);
    EXPECT_EQ(stubPtr->lastDiffRequest.input(), context.options.digest.input);
    EXPECT_TRUE(stubPtr->lastDiffRequest.dryrun());

    const auto &files = context.results.report->getFileResults();
    ASSERT_EQ(files.size(), 1);
    EXPECT_EQ(files[0].filename, "lib.jar");
    EXPECT_TRUE(files[0].isNew);
    EXPECT_FALSE(files[0].isSamePublicDigest);
}

TEST(ServerCommandTests, DiffReturnsFalseWhenRequestFails) {
    StubContext context;

    ServerCommand command(context);
    auto fakeStub = std::make_unique<FakeDigestStub>();
    fakeStub->nextDiffStatus = grpc::Status(grpc::StatusCode::UNAVAILABLE, "offline");
    auto *stubPtr = fakeStub.get();
    command._stub = std::move(fakeStub);

    EXPECT_FALSE(command.diff());
    EXPECT_TRUE(stubPtr->diffCalled);
    EXPECT_TRUE(context.results.report->getFileResults().empty());
}

TEST(ServerCommandTests, StopReturnsTrueWhenServerAcknowledgesQuit) {
    StubContext context;
    ServerCommand command(context);

    auto fakeStub = std::make_unique<FakeDigestStub>();
    auto *stubPtr = fakeStub.get();
    command._stub = std::move(fakeStub);

    EXPECT_TRUE(command.stop());
    EXPECT_TRUE(stubPtr->quitCalled);
}

TEST(ServerCommandTests, StopReturnsFalseWhenQuitFails) {
    StubContext context;
    ServerCommand command(context);

    auto fakeStub = std::make_unique<FakeDigestStub>();
    fakeStub->nextQuitStatus = grpc::Status(grpc::StatusCode::UNAVAILABLE, "busy");
    auto *stubPtr = fakeStub.get();
    command._stub = std::move(fakeStub);

    EXPECT_FALSE(command.stop());
    EXPECT_TRUE(stubPtr->quitCalled);
}

TEST(ServerCommandTests, RunUsesStopWhenServerStopFlagSet) {
    StubContext context;
    context.options.digest.server.stopServer = true;

    FakeDigestStub *createdStub = nullptr;
    ScopedStubFactoryOverride stubGuard(
        [&](Context &) {
            auto stub = std::make_unique<FakeDigestStub>();
            createdStub = stub.get();
            return stub;
        });

    const auto result = ServerCommand::run(context);
    EXPECT_TRUE(result);
    ASSERT_NE(createdStub, nullptr);
    EXPECT_TRUE(createdStub->quitCalled);
}

TEST(ServerCommandTests, RunUsesDiffWhenClientModeEnabled) {
    StubContext context;
    context.options.digest.client.isClientMode = true;
    context.options.digest.input = "/tmp/input.jar";

    FakeDigestStub *createdStub = nullptr;
    ScopedStubFactoryOverride stubGuard(
        [&](Context &) {
            auto stub = std::make_unique<FakeDigestStub>();
            createdStub = stub.get();
            auto *file = stub->templateResponse.add_files();
            file->set_filename("lib.jar");
            file->set_isnew(true);
            return stub;
        });

    const auto result = ServerCommand::run(context);
    EXPECT_TRUE(result);
    ASSERT_NE(createdStub, nullptr);
    EXPECT_TRUE(createdStub->diffCalled);
}

TEST(ServerCommandTests, RunStartsServerWhenNoClientOrStopFlags) {
    StubContext context;
    bool starterCalled = false;

    ScopedStubFactoryOverride stubGuard(
        [&](Context &) {
            return std::make_unique<FakeDigestStub>();
        });

    ScopedServerStarterOverride starterGuard(
        [&](Context &) {
            starterCalled = true;
            return true;
        });

    const auto result = ServerCommand::run(context);
    EXPECT_TRUE(result);
    EXPECT_TRUE(starterCalled);
}
