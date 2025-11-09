#include <gtest/gtest.h>

#include <memory>

#include "domain/stats/Results.h"
#include "domain/stats/Report.h"

using namespace kapa::tarracsh::domain;
using namespace stats;
using namespace stats::report;

namespace {

Options makeDiffOptions() {
    Options options;
    options.isPublicDigest = true;
    options.outputDir = ".";
    options.digest.isDiff = true;
    return options;
}

std::shared_ptr<Results> makeResults(Options &options) {
    auto log = std::make_shared<kapa::infrastructure::log::Log>();
    auto results = std::make_shared<Results>(options);
    results->log = log;
    return results;
}

} // namespace

TEST(ReportTests, AddsNewJarAndTracksFileResultWhenDiffEnabled) {
    auto options = makeDiffOptions();
    auto results = makeResults(options);
    auto &report = *results->report;

    report.asNewJar("foo.jar");

    EXPECT_EQ(results->jarfiles.taskResult.newFile.load(), 1u);
    ASSERT_EQ(report.getFileResults().size(), 1u);
    const auto &entry = report.getFileResults().front();
    EXPECT_EQ(entry.filename, "foo.jar");
    EXPECT_TRUE(entry.isNew);
}

TEST(ReportTests, RecordsModifiedClassFileAndFileSummary) {
    auto options = makeDiffOptions();
    auto results = makeResults(options);
    auto &report = *results->report;

    report.asModifiedClassFile(false, "foo.jar#pkg/Clazz");

    EXPECT_EQ(results->standaloneClassfiles.taskResult.differentDigest.load(), 1u);
    ASSERT_EQ(report.getClassResults().size(), 1u);
    const auto &classEntry = report.getClassResults().front();
    EXPECT_EQ(classEntry.strongClassname, "foo.jar#pkg/Clazz");
    EXPECT_TRUE(classEntry.isModified);
    EXPECT_FALSE(classEntry.isSamePublicDigest);

    ASSERT_EQ(report.getFileResults().size(), 1u);
    const auto &fileEntry = report.getFileResults().front();
    EXPECT_EQ(fileEntry.filename, "foo.jar#pkg/Clazz");
    EXPECT_TRUE(fileEntry.isModified);
}

TEST(ReportTests, MarksFailedJarClass) {
    auto options = makeDiffOptions();
    auto results = makeResults(options);
    auto &report = *results->report;

    report.asFailedJarClass("foo.jar#pkg/Clazz");

    EXPECT_EQ(results->jarfiles.classfiles.errors.load(), 1u);
    ASSERT_EQ(report.getClassResults().size(), 1u);
    const auto &classEntry = report.getClassResults().front();
    EXPECT_EQ(classEntry.strongClassname, "foo.jar#pkg/Clazz");
    EXPECT_FALSE(classEntry.failed);
    EXPECT_FALSE(classEntry.isModified);
}
