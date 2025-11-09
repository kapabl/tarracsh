#include <gtest/gtest.h>

#include <memory>
#include <string>
#include <filesystem>
#include <iostream>
#include <sstream>

#include "domain/stats/Results.h"
#include "infrastructure/log/Log.h"

using namespace kapa::tarracsh::domain;
using namespace stats;

namespace {

class ScopedStdoutCapture {
public:
    ScopedStdoutCapture()
        : _oldBuf(std::cout.rdbuf(_buffer.rdbuf())) {}
    ScopedStdoutCapture(const ScopedStdoutCapture &) = delete;
    ScopedStdoutCapture &operator=(const ScopedStdoutCapture &) = delete;
    ~ScopedStdoutCapture() { std::cout.rdbuf(_oldBuf); }

    [[nodiscard]] std::string str() const { return _buffer.str(); }

private:
    std::ostringstream _buffer;
    std::streambuf *_oldBuf;
};

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

    ScopedStdoutCapture capture;
    results->printProgress();
    const auto output = capture.str();
    EXPECT_NE(output.find("classfiles: 5"), std::string::npos);
    EXPECT_NE(output.find("jars: 1"), std::string::npos);
}
