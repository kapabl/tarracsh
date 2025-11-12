#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <memory>
#include <string>
#include <utility>

#include "app/Analyzer.h"
#include "app/Context.h"
#include "domain/Options.h"
#include "domain/stats/Results.h"
#include "infrastructure/log/Log.h"

using namespace kapa::tarracsh;
using namespace app;
using kapa::infrastructure::log::Log;

namespace {

std::filesystem::path makeTempDir(const std::string &prefix) {
    static std::atomic<uint64_t> counter{0};
    auto base = std::filesystem::temp_directory_path();
    const auto now = std::chrono::steady_clock::now().time_since_epoch().count();
    auto dir = base / (prefix + std::to_string(now) + "-" + std::to_string(counter.fetch_add(1)));
    std::filesystem::create_directories(dir);
    return dir;
}

class StubContext : public Context {
public:
    StubContext()
        : options(), results(options) {
        log = std::make_shared<Log>();
        results.log = log;
    }

    domain::stats::Results &getResults() override { return results; }
    domain::Options &getOptions() override { return options; }
    Log &getLog() override { return *log; }

    domain::Options options;
    domain::stats::Results results;
    std::shared_ptr<Log> log;
};

class AnalyzerAccessor : public Analyzer {
public:
    using Analyzer::Analyzer;
    using Analyzer::initAnalyzer;
};

class AnalyzerTestDouble : public Analyzer {
public:
    explicit AnalyzerTestDouble(Context &ctx)
        : Analyzer(ctx) {}

    bool initReturnValue{true};
    bool analyzeCalled{false};
    bool printAllCalled{false};
    int initCallCount{0};

protected:
    bool initAnalyzer() override {
        ++initCallCount;
        return initReturnValue;
    }

    void analyzeInput() override {
        analyzeCalled = true;
    }

    void printAllResults() override {
        printAllCalled = true;
    }
};

} // namespace

TEST(AnalyzerOrchestrationTests, InitAnalyzerSetsConfiguredLogFile) {
    StubContext context;
    const auto tempDir = makeTempDir("analyzer-log-");
    const auto logFile = tempDir / "analyzer.log";
    context.options.logFile = logFile.string();

    AnalyzerAccessor analyzer(context);
    EXPECT_TRUE(analyzer.initAnalyzer());

    const std::string message = "init analyzer log test";
    context.results.log->writeln(message, false);

    std::ifstream input(logFile);
    ASSERT_TRUE(input.is_open());
    std::string line;
    std::getline(input, line);
    EXPECT_EQ(line, message);

    std::filesystem::remove_all(tempDir);
}

TEST(AnalyzerOrchestrationTests, RunSkipsAnalysisWhenInitFails) {
    StubContext context;
    context.options.parse.print = true; // Disable progress thread for this test.

    AnalyzerTestDouble analyzer(context);
    analyzer.initReturnValue = false;

    analyzer.run();

    EXPECT_EQ(analyzer.initCallCount, 1);
    EXPECT_FALSE(analyzer.analyzeCalled);
    EXPECT_FALSE(analyzer.printAllCalled);
}

TEST(AnalyzerOrchestrationTests, RunWithPrintInvokesPrintAllWhenProgressAllowed) {
    StubContext context;
    AnalyzerTestDouble analyzer(context);

    analyzer.runWithPrint();

    EXPECT_TRUE(analyzer.analyzeCalled);
    EXPECT_TRUE(analyzer.printAllCalled);
}

TEST(AnalyzerOrchestrationTests, RunWithPrintSkipsPrintAllWhenProgressDisabled) {
    StubContext context;
    context.options.parse.print = true; // Makes Options::canPrintProgress return false.
    AnalyzerTestDouble analyzer(context);

    analyzer.runWithPrint();

    EXPECT_TRUE(analyzer.analyzeCalled);
    EXPECT_FALSE(analyzer.printAllCalled);
}
