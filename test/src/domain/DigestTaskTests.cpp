#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <mutex>
#include <vector>

#include <libzippp/libzippp.h>
#include <zip.h>

#include "domain/jar/tasks/DigestTask.h"
#include "domain/db/DigestDb.h"
#include "domain/classfile/reader/MemoryReader.h"
#include "domain/stats/Results.h"
#include "domain/Utils.h"
#include "infrastructure/log/Log.h"

using namespace kapa::tarracsh::domain;
using namespace jar::tasks;
using namespace db;
using namespace db::digest;
using namespace db::table;
using namespace stats;

namespace {

using InfraRow = kapa::infrastructure::db::table::AutoIncrementedRow;

std::vector<char> loadBinary(const std::filesystem::path &path) {
    std::ifstream file(path, std::ios::binary);
    return std::vector<char>(std::istreambuf_iterator<char>(file), {});
}

std::filesystem::path testResource(const std::string &relative) {
    if (const char *srcDir = std::getenv("TEST_SRCDIR")) {
        if (const char *workspace = std::getenv("TEST_WORKSPACE")) {
            return std::filesystem::path(srcDir) / workspace / relative;
        }
    }
    return std::filesystem::path(PROJECT_SOURCE_DIR) / relative;
}

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

class TestDigestTask final : public DigestTask {
public:
    TestDigestTask(Options options, Results &results, DigestDb &db)
        : DigestTask(std::move(options), results, db), _db(db) {}

    kapa::infrastructure::db::Database &getDb() override { return _db; }

protected:
    std::shared_ptr<ClassFiles> getClassFiles() override { return _db.getClassFiles(); }
    std::shared_ptr<Files> getFiles() override { return _db.getFiles(); }

private:
    DigestDb &_db;
};

} // namespace

TEST(DigestTaskIntegration, ProcessesJarEntryAndPersistsDigest) {
    const auto tempDir = makeTempDir("tarracsh-digest-");
    auto dbLog = makeLog(tempDir / "db.log");

    kapa::infrastructure::db::Database::Config config{tempDir.string(), dbLog.get()};
    const auto dbInstance = DigestDb::create(config, true, false);
    ASSERT_TRUE(dbInstance);

    Options options;
    options.isPublicDigest = true;
    options.digest.input = (tempDir / "sample.jar").string();
    options.outputDir = tempDir.string();

    Results results(options);
    results.log = makeLog(tempDir / "results.log");

    const auto classPath =
        testResource("test-subjects/org/kapa/reti/ast/BoolLiteralNode.class");
    const auto classBytes = loadBinary(classPath);

    {
        int zipError = 0;
        zip_t *archive = zip_open(options.digest.input.c_str(), ZIP_TRUNCATE | ZIP_CREATE, &zipError);
        ASSERT_NE(archive, nullptr);
        zip_source_t *source = zip_source_buffer(archive, classBytes.data(), classBytes.size(), 0);
        ASSERT_NE(source, nullptr);
        const auto addResult = zip_file_add(archive, "org/example/Sample.class", source, ZIP_FL_OVERWRITE);
        ASSERT_GE(addResult, 0);
        ASSERT_EQ(zip_close(archive), 0);
    }

    TestDigestTask task(options, results, *dbInstance);
    ASSERT_TRUE(task.start());

    libzippp::ZipArchive readArchive(options.digest.input);
    readArchive.open(libzippp::ZipArchive::ReadOnly);
    const auto entry = readArchive.getEntry("org/example/Sample.class");
    ASSERT_TRUE(entry.isFile());
    auto *buffer = static_cast<char *>(entry.readAsBinary());
    const auto entrySize = entry.getSize();
    const auto entryCrc = entry.getCRC();
    jar::JarEntry jarEntry(entry, buffer);
    classfile::reader::MemoryReader memReader(jarEntry.getBuffer(),
                                              static_cast<std::streamsize>(jarEntry.getSize()));
    classfile::ClassFileParser classParser(memReader, entry.getName(), results.log);
    ASSERT_TRUE(classParser.parse());
    const auto expectedClassname = classParser.getMainClassname();
    const auto expectedStrongClassname =
        utils::getStrongClassname(options.digest.input.c_str(), expectedClassname.c_str());

    std::mutex mutex;
    task.processEntry(jarEntry, mutex);
    delete[] buffer;
    readArchive.close();

    task.end();
    dbInstance->write();
    dbInstance->stop();

    EXPECT_EQ(results.jarfiles.classfiles.parsedCount.load(), 1U);
    EXPECT_EQ(results.jarfiles.taskResult.newFile.load(), 1U);
    EXPECT_EQ(results.jarfiles.classfiles.taskResult.newFile.load(), 1U);

    auto filesTable = dbInstance->getFiles();
    ASSERT_EQ(filesTable->size(), 1U);
    FileRow *storedFile = nullptr;
    filesTable->forEach([&](InfraRow *row) {
        if (!storedFile) {
            storedFile = static_cast<FileRow *>(row);
        }
    });
    ASSERT_NE(storedFile, nullptr);
    EXPECT_STREQ(filesTable->getFilename(storedFile), options.digest.input.c_str());
    EXPECT_EQ(storedFile->classfileCount, 1U);

    auto classFilesTable = dbInstance->getClassFiles();
    ASSERT_EQ(classFilesTable->size(), 1U);
    ClassFileRow *storedClass = nullptr;
    classFilesTable->forEach([&](InfraRow *row) {
        if (!storedClass) {
            storedClass = static_cast<ClassFileRow *>(row);
        }
    });
    ASSERT_NE(storedClass, nullptr);
    EXPECT_EQ(classFilesTable->getClassname(*storedClass), expectedClassname);
    EXPECT_EQ(classFilesTable->getStrongClassname(*storedClass), expectedStrongClassname);
    EXPECT_EQ(storedClass->file.id, storedFile->id);
    EXPECT_EQ(storedClass->size, entrySize);
    EXPECT_EQ(storedClass->crc, entryCrc);
    bool fileDigestNonZero = false;
    bool classDigestNonZero = false;
    for (int i = 0; i < DIGEST_LENGTH; ++i) {
        fileDigestNonZero |= storedFile->digest.buf[i] != 0;
        classDigestNonZero |= storedClass->digest.buf[i] != 0;
    }
    EXPECT_TRUE(fileDigestNonZero);
    EXPECT_TRUE(classDigestNonZero);
}

TEST(DbBasedTaskHelpers, UniqueClassnameHandlesMultiReleaseEntries) {
    const auto tempDir = makeTempDir("tarracsh-digest-unique-");
    auto dbLog = makeLog(tempDir / "db.log");

    kapa::infrastructure::db::Database::Config config{tempDir.string(), dbLog.get()};
    const auto dbInstance = DigestDb::create(config, true, false);
    ASSERT_TRUE(dbInstance);

    Options options;
    options.isPublicDigest = true;
    options.digest.input = (tempDir / "sample.jar").string();
    options.outputDir = tempDir.string();

    Results results(options);
    results.log = makeLog(tempDir / "results.log");

    const auto classPath =
        testResource("test-subjects/org/kapa/reti/ast/BoolLiteralNode.class");
    const auto classBytes = loadBinary(classPath);

    {
        int zipError = 0;
        zip_t *archive = zip_open(options.digest.input.c_str(), ZIP_TRUNCATE | ZIP_CREATE, &zipError);
        ASSERT_NE(archive, nullptr);
        zip_source_t *source1 = zip_source_buffer(archive, classBytes.data(), classBytes.size(), 0);
        ASSERT_NE(source1, nullptr);
        ASSERT_GE(zip_file_add(archive, "org/example/Sample.class", source1, ZIP_FL_OVERWRITE), 0);

        zip_source_t *source2 = zip_source_buffer(archive, classBytes.data(), classBytes.size(), 0);
        ASSERT_NE(source2, nullptr);
        ASSERT_GE(zip_file_add(archive, "META-INF/versions/9/org/example/Sample.class", source2, ZIP_FL_OVERWRITE), 0);
        ASSERT_EQ(zip_close(archive), 0);
    }

    TestDigestTask task(options, results, *dbInstance);

    libzippp::ZipArchive readArchive(options.digest.input);
    readArchive.open(libzippp::ZipArchive::ReadOnly);

    {
        const auto baseEntry = readArchive.getEntry("org/example/Sample.class");
        ASSERT_TRUE(baseEntry.isFile());
        auto *buffer = static_cast<char *>(baseEntry.readAsBinary());
        jar::JarEntry jarEntry(baseEntry, buffer);
        classfile::reader::MemoryReader reader(jarEntry.getBuffer(),
                                               static_cast<std::streamsize>(jarEntry.getSize()));
        classfile::ClassFileParser parser(reader, baseEntry.getName(), results.log);
        ASSERT_TRUE(parser.parse());

        EXPECT_EQ(task.getUniqueClassname(jarEntry, parser), parser.getMainClassname());
        delete[] buffer;
    }

    {
        const auto multiEntry = readArchive.getEntry("META-INF/versions/9/org/example/Sample.class");
        ASSERT_TRUE(multiEntry.isFile());
        auto *buffer = static_cast<char *>(multiEntry.readAsBinary());
        jar::JarEntry jarEntry(multiEntry, buffer);
        classfile::reader::MemoryReader reader(jarEntry.getBuffer(),
                                               static_cast<std::streamsize>(jarEntry.getSize()));
        classfile::ClassFileParser parser(reader, multiEntry.getName(), results.log);
        ASSERT_TRUE(parser.parse());

        EXPECT_EQ(task.getUniqueClassname(jarEntry, parser), jarEntry.getClassname());
        delete[] buffer;
    }

    readArchive.close();
    dbInstance->stop();
}

TEST(FilesTable, GeneratesKeysAndStrongClassnames) {
    const auto tempDir = makeTempDir("tarracsh-files-");
    auto dbLog = makeLog(tempDir / "db.log");

    kapa::infrastructure::db::Database::Config config{tempDir.string(), dbLog.get()};
    const auto dbInstance = DigestDb::create(config, true, false);
    ASSERT_TRUE(dbInstance);

    auto files = dbInstance->getFiles();
    auto classFiles = dbInstance->getClassFiles();

    auto *fileRow = static_cast<FileRow *>(files->allocateRow());
    new(fileRow) FileRow();
    fileRow->filename = dbInstance->getPoolString("foo.jar");
    fileRow->classfileCount = 0;
    files->add(fileRow);

    FileRow *storedFile = nullptr;
    files->forEach([&](InfraRow *row) {
        storedFile = static_cast<FileRow *>(row);
    });
    ASSERT_NE(storedFile, nullptr);
    EXPECT_EQ(files->createKey(storedFile), "foo.jar");
    EXPECT_STREQ(files->getFilename(storedFile), "foo.jar");

    auto *classRow = static_cast<ClassFileRow *>(classFiles->allocateRow());
    new(classRow) ClassFileRow(*storedFile);
    classRow->classname = dbInstance->getPoolString("pkg/Sample");
    classFiles->add(classRow);

    ClassFileRow *storedClass = nullptr;
    classFiles->forEach([&](InfraRow *row) {
        storedClass = static_cast<ClassFileRow *>(row);
    });
    ASSERT_NE(storedClass, nullptr);
    EXPECT_EQ(classFiles->getClassname(*storedClass), "pkg/Sample");
    EXPECT_EQ(classFiles->getStrongClassname(*storedClass),
              utils::getStrongClassname(files->getFilename(storedFile), "pkg/Sample"));

    dbInstance->stop();
}
