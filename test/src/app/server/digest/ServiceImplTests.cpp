#include <gtest/gtest.h>

#include "app/server/digest/ServiceImpl.h"
#include "domain/stats/Report.h"
#include "domain/stats/Results.h"

using kapa::tarracsh::app::server::digest::DiffResponse;
using kapa::tarracsh::domain::Options;
using kapa::tarracsh::domain::stats::Results;
using kapa::tarracsh::domain::stats::report::ClassResult;
using kapa::tarracsh::domain::stats::report::FileResult;

namespace {

class ServiceImplTestPeer : public kapa::tarracsh::server::digest::ServiceImpl {
public:
    using ServiceImpl::reportToResponse;
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
