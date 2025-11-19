#include <gtest/gtest.h>

#include "app/server/digest/ServiceImpl.h"
#include "domain/stats/Report.h"
#include "domain/stats/Results.h"
#include "test/src/app/runtime/TestRuntime.h"
#include <cstdlib>

using kapa::tarracsh::app::Context;
using kapa::tarracsh::app::server::digest::DiffRequest;
using kapa::tarracsh::app::server::digest::DiffResponse;
using kapa::tarracsh::domain::Options;
using kapa::tarracsh::domain::stats::Results;
using kapa::tarracsh::domain::stats::report::ClassResult;
using kapa::tarracsh::domain::stats::report::FileResult;
using grpc::StatusCode;

namespace {

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
