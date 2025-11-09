#include <gtest/gtest.h>

#include <filesystem>
#include <chrono>

#include "domain/db/CallGraphDb.h"
#include "infrastructure/log/Log.h"

using namespace kapa::tarracsh::domain;
using namespace db;
using namespace db::callgraph;

namespace {

std::filesystem::path makeTempDir(const std::string &suffix) {
    const auto base = std::filesystem::temp_directory_path();
    const auto dir = base / ("tarracsh-db-" + suffix + "-" +
                             std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()));
    std::filesystem::create_directories(dir);
    return dir;
}

std::shared_ptr<CallGraphDb> makeDb(const std::filesystem::path &dir,
                                    const std::shared_ptr<kapa::infrastructure::log::Log> &log) {
    kapa::infrastructure::db::Database::Config config{dir.string(), log.get()};
    return CallGraphDb::create(config, /*doClean=*/true, /*hasSaveThread=*/false);
}

} // namespace

TEST(DbTablesTests, GeneratesStrongClassnameAndKeys) {
    const auto dir = makeTempDir("tables");
    auto log = std::make_shared<kapa::infrastructure::log::Log>();
    log->init((dir / "db.log").string());

    auto db = makeDb(dir, log);
    ASSERT_NE(db, nullptr);

    auto files = db->getFiles();
    auto &fileRow = *reinterpret_cast<table::FileRow *>(files->allocateRow());
    new(&fileRow) table::FileRow();
    fileRow.filename = db->getPoolString("foo.jar");
    files->addOrUpdate(&fileRow);

    auto classFiles = db->getClassFiles();
    auto &classRow = *reinterpret_cast<table::ClassFileRow *>(classFiles->allocateRow());
    new(&classRow) table::ClassFileRow();
    classRow.file.id = fileRow.id;
    classRow.classname = db->getPoolString("pkg/Clazz");
    classFiles->addOrUpdate(&classRow);

    const auto strongName = classFiles->getStrongClassname(classRow);
    EXPECT_EQ(strongName, "foo.jar@pkg/Clazz");
    EXPECT_EQ(classFiles->getClassname(classRow), std::string("pkg/Clazz"));
    EXPECT_STREQ(files->getFilename(&fileRow), "foo.jar");
    EXPECT_EQ(files->getKey(&fileRow), files->createKey(fileRow.filename));
}
