#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <mutex>
#include <stdexcept>
#include <sstream>

#include "domain/db/DigestDb.h"
#include "infrastructure/db/query/Engine.h"
#include "infrastructure/log/Log.h"

using namespace kapa::tarracsh::domain;
using namespace db;
using namespace db::digest;
using namespace db::table;
using namespace kapa::infrastructure::db;
using namespace kapa::infrastructure::db::query;

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

std::filesystem::path makeTempDir(const std::string &prefix) {
    static std::atomic<uint64_t> counter{0};
    const auto base = std::filesystem::temp_directory_path();
    const auto now = std::chrono::steady_clock::now().time_since_epoch().count();
    auto dir = base / (prefix + std::to_string(now) + "-" + std::to_string(counter.fetch_add(1)));
    std::filesystem::create_directories(dir);
    return dir;
}

std::shared_ptr<kapa::infrastructure::log::Log> makeLog(const std::filesystem::path &path) {
    auto log = std::make_shared<kapa::infrastructure::log::Log>();
    std::filesystem::create_directories(path.parent_path());
    log->init(path.string());
    return log;
}

struct DigestDbHandle {
    std::filesystem::path tempDir;
    std::shared_ptr<kapa::infrastructure::log::Log> log;
    std::shared_ptr<DigestDb> db;

    ~DigestDbHandle() {
        if (db) {
            db->stop();
        }
    }
};

DigestDbHandle createDigestDbWithSampleData() {
    DigestDbHandle handle;
    handle.tempDir = makeTempDir("tarracsh-query-");
    handle.log = makeLog(handle.tempDir / "db.log");
    Database::Config config{handle.tempDir.string(), handle.log.get()};

    auto seededDb = DigestDb::create(config, true, false);
    if (!seededDb) {
        throw std::runtime_error("Failed to create seeded digest database");
    }

    auto files = seededDb->getFiles();
    auto classFiles = seededDb->getClassFiles();

    auto *fileRow = static_cast<FileRow *>(files->allocateRow());
    new(fileRow) FileRow();
    fileRow->filename = seededDb->getPoolString("foo.jar");
    fileRow->classfileCount = 1;
    files->add(fileRow);

    auto *classRow = static_cast<ClassFileRow *>(classFiles->allocateRow());
    new(classRow) ClassFileRow(*fileRow);
    classRow->classname = seededDb->getPoolString("pkg/Sample");
    classFiles->add(classRow);

    seededDb->write();
    seededDb->stop();

    handle.db = DigestDb::create(config, false, false);
    if (!handle.db) {
        throw std::runtime_error("Failed to create reloaded digest database");
    }

    return handle;
}

std::string runQuery(const Engine &engine,
                     const std::string &query,
                     const bool displayRaw = false,
                     const bool expectSuccess = true) {
    ScopedStdoutCapture capture;
    const auto result = engine.execute(query, displayRaw);
    const auto output = capture.str();
    EXPECT_EQ(expectSuccess, result) << "Unexpected execution result for query: " << query;
    return output;
}

} // namespace

TEST(QueryEngine, ExecutesSchemaAndListQueries) {
    auto dbHandle = createDigestDbWithSampleData();
    Engine engine(*dbHandle.db);

    EXPECT_TRUE(engine.execute("schema", false));

    const auto listOutput = runQuery(engine, "list files", true);
    EXPECT_NE(std::string::npos, listOutput.find("table: files"));
    EXPECT_NE(std::string::npos, listOutput.find("rows found: 1"));
}

TEST(QueryEngine, AppliesFilterOperatorsAndRegexMatches) {
    auto dbHandle = createDigestDbWithSampleData();
    Engine engine(*dbHandle.db);

    const auto equalOutput = runQuery(engine, "list files where filename = 'foo.jar'");
    EXPECT_NE(std::string::npos, equalOutput.find("rows found: 1"));

    const auto notEqualOutput = runQuery(engine, "list files where filename != 'bar.jar'");
    EXPECT_NE(std::string::npos, notEqualOutput.find("rows found: 1"));

    const auto startsWithOutput = runQuery(engine, "list files where filename ^* 'foo'");
    EXPECT_NE(std::string::npos, startsWithOutput.find("rows found: 1"));

    const auto notStartsWithOutput = runQuery(engine, "list files where filename not ^* 'foo'");
    EXPECT_NE(std::string::npos, notStartsWithOutput.find("rows found: 0"));

    const auto endsWithOutput = runQuery(engine, "list files where filename *^ 'jar'");
    EXPECT_NE(std::string::npos, endsWithOutput.find("rows found: 1"));

    const auto notEndsWithOutput = runQuery(engine, "list files where filename not *^ 'jar'");
    EXPECT_NE(std::string::npos, notEndsWithOutput.find("rows found: 0"));

    const auto regexOutput = runQuery(engine,
                                      "list classfiles where classname *^* 'pkg\\/Sample'");
    EXPECT_NE(std::string::npos, regexOutput.find("rows found: 1"));

    const auto regexCaseInsensitive = runQuery(engine,
                                               "list classfiles where classname *^* 'pkg\\/sample/i'");
    EXPECT_NE(std::string::npos, regexCaseInsensitive.find("rows found: 1"));
}

TEST(QueryEngine, ReportsSemanticAndSyntaxErrors) {
    auto dbHandle = createDigestDbWithSampleData();
    Engine engine(*dbHandle.db);

    const auto syntaxOutput = runQuery(engine, "list", false, false);
    EXPECT_NE(std::string::npos, syntaxOutput.find("Query error"));

    const auto invalidTableOutput = runQuery(engine, "list missing where id = '1'");
    EXPECT_NE(std::string::npos, invalidTableOutput.find("Invalid table name: missing"));

    const auto invalidColumnOutput = runQuery(engine, "list files where bogus = 'foo'");
    EXPECT_NE(std::string::npos, invalidColumnOutput.find("Invalid column name: bogus"));
    EXPECT_EQ(std::string::npos, invalidColumnOutput.find("rows found:"));

    ScopedStdoutCapture capture;
    engine.log("custom log message", true);
    const auto logOutput = capture.str();
    EXPECT_NE(std::string::npos, logOutput.find("custom log message"));
}
