#include <gtest/gtest.h>

#include <chrono>
#include <filesystem>
#include <iostream>
#include <memory>
#include <sstream>

#include "domain/stats/ProfileData.h"
#include "domain/stats/Results.h"
#include "infrastructure/filesystem/Utils.h"

using namespace kapa::tarracsh::domain;
using namespace stats;
using namespace stats::profiler;

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
    std::streambuf *_oldBuf{nullptr};
};

std::filesystem::path makeTempDir(const std::string &name) {
    const auto base = std::filesystem::temp_directory_path();
    const auto dir = base / ("tarracsh-profiler-" + name + "-" +
                             std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()));
    std::filesystem::create_directories(dir);
    return dir;
}

std::shared_ptr<kapa::infrastructure::log::Log> makeLog(const std::filesystem::path &dir) {
    auto log = std::make_shared<kapa::infrastructure::log::Log>();
    log->init((dir / "output.log").string());
    return log;
}

std::shared_ptr<Results> makeResults(Options &options, const std::shared_ptr<kapa::infrastructure::log::Log> &log) {
    auto results = std::make_shared<Results>(options);
    results->log = log;
    return results;
}

} // namespace

TEST(ProfileDataTests, WritesProfilerFileWithDurations) {
    auto tempDir = makeTempDir("write");
    Options options;
    options.outputDir = tempDir.string();
    auto log = makeLog(tempDir);
    auto results = makeResults(options, log);

    ProfileData profiler(*results);
    profiler.analyzerTime = MillisecondDuration(123);
    profiler.initAnalyzer = MillisecondDuration(5);
    profiler.initDb = MillisecondDuration(7);

    profiler.output(options);

    const auto profilerPath = tempDir / "profiler.txt";
    ASSERT_TRUE(std::filesystem::exists(profilerPath));
    const auto content = kapa::infrastructure::filesystem::utils::readFileContent(profilerPath.string());
    EXPECT_NE(content.find("Analyzer total time"), std::string::npos);
    EXPECT_NE(content.find("Init Db"), std::string::npos);
}

TEST(ProfileDataTests, PrintsProfilerWhenFlagSet) {
    auto tempDir = makeTempDir("print");
    Options options;
    options.outputDir = tempDir.string();
    options.printProfiler = true;
    auto log = makeLog(tempDir);
    auto results = makeResults(options, log);

    ProfileData profiler(*results);
    profiler.analyzerTime = MillisecondDuration(42);

    ScopedStdoutCapture capture;
    profiler.output(options);
    const auto stdoutOutput = capture.str();
    EXPECT_NE(stdoutOutput.find("Analyzer total time"), std::string::npos);
}
