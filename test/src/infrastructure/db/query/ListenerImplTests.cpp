#include <gtest/gtest.h>

#include <atomic>
#include <filesystem>
#include <memory>
#include <string>

#include "domain/db/table/Files.h"
#include "infrastructure/db/Database.h"
#include "infrastructure/db/query/Engine.h"
#include "infrastructure/log/Log.h"

using namespace kapa::infrastructure::db;
using namespace kapa::infrastructure::db::query;
using namespace kapa::tarracsh::domain::db::table;

namespace {

class DummyDatabase : public Database {
public:
    DummyDatabase(const Config &config, std::shared_ptr<kapa::infrastructure::log::Log> logStorage)
        : Database(config, false),
          _logStorage(std::move(logStorage)) {}

    void init() override {
        Database::init();
        _files = std::make_shared<Files>(*this, "files");
        _files->init();
        addTable(*_files);
    }

    bool read() override { return true; }
    bool write() override { return true; }
    void clean() override {}
    void backup() override {}
    void outputStats() const override {}

    std::shared_ptr<Files> files() { return _files; }

private:
    std::shared_ptr<Files> _files;
    std::shared_ptr<kapa::infrastructure::log::Log> _logStorage;
};

std::filesystem::path makeTempDir(const std::string &prefix) {
    static std::atomic<uint64_t> counter{0};
    const auto base = std::filesystem::temp_directory_path();
    const auto now = std::chrono::steady_clock::now().time_since_epoch().count();
    auto dir = base / (prefix + std::to_string(now) + "-" + std::to_string(counter.fetch_add(1)));
    std::filesystem::create_directories(dir);
    return dir;
}

std::shared_ptr<DummyDatabase> makeDatabase() {
    auto tempDir = makeTempDir("listener-tests-");
    auto log = std::make_shared<kapa::infrastructure::log::Log>();
    log->init((tempDir / "log.txt").string());
    Database::Config config{tempDir.string(), log.get()};
    auto db = std::make_shared<DummyDatabase>(config, log);
    Database::init(*db, false);
    return db;
}

void addFileRow(DummyDatabase &db, const std::string &filename, const uint32_t classfileCount) {
    auto files = db.files();
    auto *row = static_cast<FileRow *>(files->allocateRow());
    new(row) FileRow();
    row->filename = db.getPoolString(filename);
    row->classfileCount = classfileCount;
    files->add(row);
}

std::string runQuery(const Engine &engine, const std::string &query) {
    testing::internal::CaptureStdout();
    const auto success = engine.execute(query, false);
    auto output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(success) << "Engine execute failed for query: " << query;
    return output;
}

}  // namespace

TEST(ListenerImplTests, EmitsSemanticErrorsForInvalidTableAndColumns) {
    auto db = makeDatabase();
    Engine engine(*db);

    const auto missingTableOutput = runQuery(engine, "list foobar where id = '1'");
    EXPECT_NE(missingTableOutput.find("Invalid table name: foobar"), std::string::npos);

    addFileRow(*db, "alpha.jar", 1);
    const auto invalidColumnOutput = runQuery(engine, "list files where imaginary = 'value'");
    EXPECT_NE(invalidColumnOutput.find("Invalid column name: imaginary"), std::string::npos);
}

TEST(ListenerImplTests, AppliesWhereClauseOnlyWhenAllPredicatesAreTrue) {
    auto db = makeDatabase();
    addFileRow(*db, "alpha.jar", 1);
    addFileRow(*db, "beta.jar", 2);
    Engine engine(*db);

    const auto andOutput = runQuery(
        engine,
        "list files where (filename = 'alpha.jar' and classfileCount = '1')");
    EXPECT_NE(andOutput.find("rows found: 1"), std::string::npos);

    const auto andFalseOutput = runQuery(
        engine,
        "list files where (filename = 'alpha.jar' and classfileCount = '2')");
    EXPECT_NE(andFalseOutput.find("rows found: 0"), std::string::npos);
}

TEST(ListenerImplTests, HandlesNegatedFiltersAndRegexCaching) {
    auto db = makeDatabase();
    addFileRow(*db, "alpha.jar", 1);
    addFileRow(*db, "gamma.LOG", 5);
    Engine engine(*db);

    const auto notEqualOutput = runQuery(
        engine,
        "list files where filename != 'alpha.jar'");
    EXPECT_NE(notEqualOutput.find("rows found: 1"), std::string::npos);

    const auto regexOutput = runQuery(
        engine,
        "list files where filename *^* 'ALPHA/i'");
    EXPECT_NE(regexOutput.find("rows found: 1"), std::string::npos);
}
