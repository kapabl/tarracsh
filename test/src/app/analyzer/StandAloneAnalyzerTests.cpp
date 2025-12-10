#include <gtest/gtest.h>

#include <filesystem>
#include <memory>
#include <string>

#include "app/Analyzer.h"
#include "app/Context.h"
#include "domain/Options.h"
#include "domain/stats/Results.h"
#include "infrastructure/db/Database.h"
#include "infrastructure/log/Log.h"

using namespace kapa::tarracsh::app;
using kapa::tarracsh::domain::Options;
using kapa::tarracsh::domain::stats::Results;
using kapa::infrastructure::log::Log;

namespace {

class DummyContext : public Context {
public:
    DummyContext() : options(), results(options) {
        log = std::make_shared<Log>();
        log->forceStdout(true);
        results.log = log;
    }

    Results &getResults() override { return results; }
    Options &getOptions() override { return options; }
    Log &getLog() override { return *log; }

    Options options;
    Results results;
    std::shared_ptr<Log> log;
};

class DummyDatabase : public kapa::infrastructure::db::Database {
public:
    explicit DummyDatabase(const kapa::infrastructure::db::Database::Config &config)
        : Database(config, false) {}

    void init() override {}
};

class AnalyzerAccessor : public Analyzer {
public:
    AnalyzerAccessor(Context &ctx, std::shared_ptr<kapa::infrastructure::db::Database> db)
        : Analyzer(ctx, std::move(db)) {}

    using Analyzer::analyzeInput;
    using Analyzer::processDirInput;
    using Analyzer::processJar;
    using Analyzer::processStandaloneClassFile;
};

} // namespace

TEST(StandaloneAnalyzerTests, AnalyzeInputProcessesClassfile) {
    DummyContext context;
    auto tempDir = std::filesystem::temp_directory_path() / "analyzer-standalone";
    std::filesystem::create_directories(tempDir);
    const auto sampleClass = std::filesystem::path("test-subjects/org/kapa/reti/ast/AstNode.class");
    ASSERT_TRUE(std::filesystem::exists(sampleClass));
    auto standaloneClass = tempDir / "AstNodeCopy.class";
    std::filesystem::copy_file(sampleClass, standaloneClass, std::filesystem::copy_options::overwrite_existing);

    context.options.parse.print = false;
    context.options.getSubCommandOptions().input = standaloneClass.string();
    context.options.getSubCommandOptions().isClassFile = true;

    auto db = std::make_shared<DummyDatabase>(kapa::infrastructure::db::Database::Config{tempDir.string(), context.log.get()});
    AnalyzerAccessor analyzer(context, db);

    analyzer.analyzeInput();

    EXPECT_GE(context.results.standaloneClassfiles.count.load(), 1u);
    std::filesystem::remove_all(tempDir);
}

