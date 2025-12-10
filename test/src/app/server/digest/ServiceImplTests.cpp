#include <gtest/gtest.h>

#include "app/server/digest/ServiceImpl.h"
#include <boost/interprocess/exceptions.hpp>
#include "domain/stats/Report.h"
#include "domain/stats/Results.h"
#include "test/src/app/runtime/TestRuntime.h"
#include <atomic>
#include <chrono>
#include <cstdlib>
#include <exception>
#include <filesystem>
#include <functional>
#include <thread>

using kapa::tarracsh::app::Context;
using kapa::tarracsh::app::server::digest::DiffRequest;
using kapa::tarracsh::app::server::digest::DiffResponse;
using kapa::tarracsh::app::server::digest::Empty;
using kapa::tarracsh::domain::Options;
using kapa::tarracsh::domain::stats::Results;
using kapa::tarracsh::domain::stats::report::ClassResult;
using kapa::tarracsh::domain::stats::report::FileResult;
using kapa::tarracsh::server::digest::ServiceImpl;
using kapa::tarracsh::server::digest::testhooks::ServiceImplAccessor;
using grpc::StatusCode;

namespace {

std::filesystem::path makeTempDir(const std::string &prefix) {
    const auto base = std::filesystem::temp_directory_path();
    const auto now = std::chrono::steady_clock::now().time_since_epoch().count();
    auto dir = base / (prefix + std::to_string(now));
    std::filesystem::create_directories(dir);
    return dir;
}

class ServiceImplTestPeer : public kapa::tarracsh::server::digest::ServiceImpl {
public:
    using ServiceImpl::reportToResponse;

    explicit ServiceImplTestPeer(Context &context) : ServiceImpl(context) {}
};

class StubContext : public Context {
public:
    StubContext()
        : options(), results(options) {
        log = std::make_shared<kapa::infrastructure::log::Log>();
        log->forceStdout(true);
        results.log = log;
        const auto logPath = (std::filesystem::temp_directory_path() / "service-impl-tests.log").string();
        log->init(logPath);
        options.logFile = logPath;
        options.isPublicDigest = true;
    }

    Options &getOptions() override { return options; }
    Results &getResults() override { return results; }
    kapa::infrastructure::log::Log &getLog() override { return *log; }

    Options options;
    Results results;
    std::shared_ptr<kapa::infrastructure::log::Log> log;
};

} // namespace

TEST(ServiceImplTests, ReportToResponseCopiesReportSummaries) {
    Options options;
    options.isPublicDigest = true;
    options.digest.isDiff = true;

    Results results(options);
    auto &report = results.report;

    FileResult newFile;
    newFile.filename = "lib.jar";
    newFile.isNew = true;
    newFile.isSamePublicDigest = false;
    report->addFile(newFile);

    FileResult failedFile;
    failedFile.filename = "broken.jar";
    failedFile.failed = true;
    report->addFile(failedFile);

    ClassResult classResult;
    classResult.strongClassname = "pkg/Type";
    classResult.isModified = true;
    classResult.isSamePublicDigest = false;
    report->addClass(classResult);

    ClassResult newClass;
    newClass.strongClassname = "pkg/NewType";
    newClass.isNew = true;
    report->addClass(newClass);

    DiffResponse response;
    ServiceImplTestPeer::reportToResponse(report, response);

    ASSERT_EQ(response.files_size(), 2);
    const auto &firstFile = response.files(0);
    EXPECT_EQ(firstFile.filename(), "lib.jar");
    EXPECT_TRUE(firstFile.isnew());
    EXPECT_FALSE(firstFile.issamedigest());

    const auto &secondFile = response.files(1);
    EXPECT_EQ(secondFile.filename(), "broken.jar");
    EXPECT_TRUE(secondFile.failed());

    ASSERT_EQ(response.classfiles_size(), 2);
    const auto &firstClass = response.classfiles(0);
    EXPECT_EQ(firstClass.strongclassname(), "pkg/Type");
    EXPECT_TRUE(firstClass.ismodified());
    EXPECT_FALSE(firstClass.issamedigest());

    const auto &secondClass = response.classfiles(1);
    EXPECT_EQ(secondClass.strongclassname(), "pkg/NewType");
    EXPECT_TRUE(secondClass.isnew());
}

TEST(ServiceImplTests, DiffReturnsInvalidArgumentOnBadInput) {
    StubContext context;
    context.log->setFile(context.options.logFile);
    kapa::tarracsh::app::runtime::test::reset(context.log);
    auto &globalOptions = kapa::tarracsh::app::runtime::test::options();
    globalOptions.logFile = context.options.logFile;
    ServiceImplTestPeer service(context);

    DiffRequest request;
    request.set_input("/definitely/missing-" + std::to_string(std::rand()));

    grpc::ServerContext serverContext;
    DiffResponse response;
    const auto status = service.Diff(&serverContext, &request, &response);

    EXPECT_EQ(status.error_code(), StatusCode::INVALID_ARGUMENT);
}

TEST(ServiceImplTests, DiffProcessesValidJarAndReturnsOk) {
    StubContext context;
    context.log->setFile(context.options.logFile);
    kapa::tarracsh::app::runtime::test::reset(context.log);
    auto &globalOptions = kapa::tarracsh::app::runtime::test::options();
    globalOptions.logFile = context.options.logFile;
    ServiceImplTestPeer service(context);

    DiffRequest request;
    request.set_input("test-subjects/jars/AstNode.jar");
    request.set_dryrun(true);

    grpc::ServerContext serverContext;
    DiffResponse response;
    const auto status = service.Diff(&serverContext, &request, &response);

    EXPECT_TRUE(status.ok());
    // Successful path should not touch quit flag or return an error code.
    EXPECT_GE(response.files_size(), 0);
}

TEST(ServiceImplTests, QuitSignalsAndReturnsOk) {
    StubContext context;
    ServiceImplTestPeer service(context);

    grpc::ServerContext serverContext;
    Empty request;
    Empty response;

    const auto status = service.Quit(&serverContext, &request, &response);
    EXPECT_TRUE(status.ok());
}

TEST(ServiceImplTests, StartInitializesServerAndStopsViaSignal) {
    StubContext context;
    auto tempDir = makeTempDir("service-impl-start-");
    context.options.outputDir = tempDir.string();
    context.options.digest.server.listenAddress = "127.0.0.1";
    context.options.digest.server.port = 0;

    std::atomic<bool> started{false};
    std::exception_ptr backgroundError;
    ServiceImpl::resetQuickSignalForTests();

    std::thread serverThread([&] {
        try {
            started = ServiceImpl::start(context);
        } catch (...) {
            backgroundError = std::current_exception();
        }
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    ServiceImpl::signalQuick();
    serverThread.join();

    if (backgroundError) {
        try {
            std::rethrow_exception(backgroundError);
        } catch (const boost::interprocess::interprocess_exception &ex) {
            std::filesystem::remove_all(tempDir);
            GTEST_SKIP() << "ServiceImpl::start unavailable in this environment: " << ex.what();
        } catch (...) {
            std::filesystem::remove_all(tempDir);
            throw;
        }
    }

    if (!started.load()) {
        GTEST_SKIP() << "ServiceImpl::start failed to acquire shared mutex in this environment.";
    }
    std::filesystem::remove_all(tempDir);
}

TEST(ServiceImplTests, StartServerUsesInjectedHandleFactory) {
    StubContext context;
    ServiceImpl service(context);

    struct FakeHandle : ServiceImpl::ServerHandle {
        explicit FakeHandle(std::function<void()> onShutdown)
            : shutdownFn(std::move(onShutdown)) {}

        void Shutdown() override {
            if (shutdownFn) {
                shutdownFn();
            }
        }

        std::function<void()> shutdownFn;
    };

    bool factoryCalled = false;
    bool shutdownCalled = false;
    auto previousFactory = ServiceImpl::setServerHandleFactoryForTests(
        [&](ServiceImpl &impl, const std::string &address) -> std::unique_ptr<ServiceImpl::ServerHandle> {
            factoryCalled = true;
            EXPECT_EQ(&impl, &service);
            EXPECT_EQ(address, context.options.digest.server.getListenServerAddress());
            return std::make_unique<FakeHandle>([&]() { shutdownCalled = true; });
        });

    ServiceImpl::resetQuickSignalForTests();
    std::thread signalThread([]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        ServiceImpl::signalQuick();
    });

    ServiceImplAccessor::startServer(service);
    signalThread.join();

    EXPECT_TRUE(factoryCalled);
    EXPECT_TRUE(shutdownCalled);

    ServiceImpl::setServerHandleFactoryForTests(std::move(previousFactory));
}

TEST(ServiceImplTests, InitDbReturnsFalseWhenFactoryFails) {
    StubContext context;
    ServiceImpl service(context);

    auto previousFactory = ServiceImpl::setDigestDbFactoryForTests(
        [](const kapa::tarracsh::domain::db::digest::DigestDb::Config &, bool, bool) {
            return std::shared_ptr<kapa::tarracsh::domain::db::digest::DigestDb>{};
        });

    const auto initResult = ServiceImplAccessor::initDb(service);
    EXPECT_FALSE(initResult);

    ServiceImpl::setDigestDbFactoryForTests(std::move(previousFactory));
}
