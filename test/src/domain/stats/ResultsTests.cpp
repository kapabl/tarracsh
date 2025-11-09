#include <gtest/gtest.h>

#include <memory>
#include <string>
#include <filesystem>

#include "domain/stats/Results.h"
#include "infrastructure/log/Log.h"

using namespace kapa::tarracsh::domain;
using namespace stats;

namespace {

std::shared_ptr<Results> makeResults() {
    Options options;
    options.outputDir = ".";
    auto log = std::make_shared<kapa::infrastructure::log::Log>();
    const auto logPath = (std::filesystem::temp_directory_path() / "results-test.log").string();
    log->init(logPath);
    auto results = std::make_shared<Results>(options);
    results->log = log;
    return results;
}

} // namespace

TEST(ResultsTests, PrintProgressEmitsCountersOnceThresholdReached) {
    auto results = makeResults();
    results->standaloneClassfiles.count.store(2);
    results->jarfiles.classfiles.count.store(3);
    results->jarfiles.count.store(1);

    testing::internal::CaptureStdout();
    results->printProgress();
    const auto output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("classfiles: 5"), std::string::npos);
    EXPECT_NE(output.find("jars: 1"), std::string::npos);
}
