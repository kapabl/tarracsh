#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <memory>
#include <string>

#include "app/commands/digest/DigestAnalyzer.h"

#include "app/Context.h"
#include "domain/Options.h"
#include "domain/classfile/StandaloneClassFileInfo.h"
#include "domain/db/DigestDb.h"
#include "domain/db/table/ClassFiles.h"
#include "domain/db/table/Files.h"
#include "domain/stats/Results.h"
#include "infrastructure/filesystem/Utils.h"
#include "infrastructure/log/Log.h"

using namespace kapa::tarracsh;
using namespace kapa::tarracsh::app;
using namespace kapa::tarracsh::app::commands::digest;
using namespace kapa::tarracsh::domain;
using namespace kapa::tarracsh::domain::classfile;
using namespace kapa::tarracsh::domain::db;
using namespace kapa::tarracsh::domain::db::digest;
using namespace kapa::tarracsh::domain::db::table;
using namespace kapa::tarracsh::domain::stats;
using kapa::infrastructure::log::Log;

namespace {

std::filesystem::path makeTempDir(const std::string &prefix) {
    static std::atomic<uint64_t> counter{0};
    const auto base = std::filesystem::temp_directory_path();
    const auto now = std::chrono::steady_clock::now().time_since_epoch().count();
    auto dir = base / (prefix + std::to_string(now) + "-" + std::to_string(counter.fetch_add(1)));
    std::filesystem::create_directories(dir);
    return dir;
}

struct StubContext : Context {
    StubContext() : options(), results(options) {
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

std::shared_ptr<DigestDb> createDb(const std::filesystem::path &dir, Log *log) {
    DigestDb::Config config{dir.string(), log};
    auto db = DigestDb::create(config, /*doClean=*/true, /*hasSaveThread=*/false);
    return db;
}

} // namespace

class DigestAnalyzerTests : public ::testing::Test {
protected:
    void SetUp() override {
        tempDir = makeTempDir("digest-analyzer-tests-");
        context = std::make_unique<StubContext>();
        context->options.outputDir = tempDir.string();
        context->options.logFile = (tempDir / "digest.log").string();
        context->options.isPublicDigest = true;
    }

    void TearDown() override {
        if (db) {
            db->stop();
        }
        std::error_code ec;
        std::filesystem::remove_all(tempDir, ec);
    }

    std::filesystem::path copyClassFile(const std::string &name) {
        const auto source = std::filesystem::path("test-subjects/org/kapa/reti/ast") / name;
        auto destination = tempDir / name;
        std::filesystem::copy_file(source, destination, std::filesystem::copy_options::overwrite_existing);
        return destination;
    }

    std::filesystem::path copyJar(const std::string &name) {
        const auto source = std::filesystem::path("test-subjects/jars") / name;
        auto destination = tempDir / name;
        std::filesystem::copy_file(source, destination, std::filesystem::copy_options::overwrite_existing);
        return destination;
    }

    std::filesystem::path tempDir;
    std::unique_ptr<StubContext> context;
    std::shared_ptr<DigestDb> db;
};

TEST_F(DigestAnalyzerTests, ProcessesJarAndStoresRows) {
    const auto jarPath = copyJar("AstNode.jar");
    ASSERT_TRUE(std::filesystem::exists(jarPath));

    context->options.digest.input = jarPath.string();
    ASSERT_TRUE(context->options.digest.processInput());

    db = createDb(tempDir, context->log.get());
    ASSERT_NE(db, nullptr);

    DigestAnalyzer analyzer(*context, db);
    analyzer.run();

    EXPECT_EQ(context->results.jarfiles.count.load(), 1u);
    EXPECT_GT(db->getFiles()->size(), 0u);
    EXPECT_GT(db->getClassFiles()->size(), 0u);
    EXPECT_GT(context->results.jarfiles.classfiles.parsedCount.load(), 0u);
}

TEST_F(DigestAnalyzerTests, SkipsParsingWhenFileTimestampMatchesExistingRows) {
    const auto classPath = copyClassFile("AstNode.class");
    ASSERT_TRUE(std::filesystem::exists(classPath));

    const StandaloneClassFileInfo fileInfo(classPath.string());

    db = createDb(tempDir, context->log.get());
    ASSERT_NE(db, nullptr);

    auto files = db->getFiles();
    auto &fileRow = reinterpret_cast<FileRow &>(*files->allocateRow());
    new(&fileRow) FileRow();
    fileRow.filename = db->getPoolString(classPath.string());
    fileRow.fileSize = fileInfo.size;
    fileRow.lastWriteTime = fileInfo.timestamp;
    fileRow.classfileCount = 1;
    files->add(&fileRow);

    auto classFiles = db->getClassFiles();
    auto &classRow = reinterpret_cast<ClassFileRow &>(*classFiles->allocateRow());
    new(&classRow) ClassFileRow(fileRow);
    classRow.classname = db->getPoolString("org/kapa/reti/ast/AstNode");
    classFiles->add(&classRow);

    context->options.digest.input = classPath.string();
    context->options.useFileTimestamp = true;
    ASSERT_TRUE(context->options.digest.processInput());

    DigestAnalyzer analyzer(*context, db);
    analyzer.run();

    EXPECT_EQ(context->results.standaloneClassfiles.parsedCount.load(), 0u);
    EXPECT_EQ(context->results.standaloneClassfiles.taskResult.unchangedCount.load(), 1u);
    EXPECT_TRUE(context->results.report->getClassResults().empty());
}

TEST_F(DigestAnalyzerTests, ProcessesStandaloneClassfileAndUpdatesReportAndDb) {
    const auto classPath = copyClassFile("AstNode.class");
    ASSERT_TRUE(std::filesystem::exists(classPath));

    context->options.digest.input = classPath.string();
    context->options.useFileTimestamp = false;
    context->options.digest.isDiff = true;
    ASSERT_TRUE(context->options.digest.processInput());

    db = createDb(tempDir, context->log.get());
    ASSERT_NE(db, nullptr);

    DigestAnalyzer analyzer(*context, db);
    analyzer.run();

    EXPECT_EQ(context->results.standaloneClassfiles.parsedCount.load(), 1u);
    const auto &classes = context->results.report->getClassResults();
    ASSERT_EQ(classes.size(), 1u);
    EXPECT_TRUE(classes.front().isNew);

    EXPECT_EQ(db->getFiles()->size(), 1u);
    EXPECT_EQ(db->getClassFiles()->size(), 1u);
}
