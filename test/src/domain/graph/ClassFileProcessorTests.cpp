#include <gtest/gtest.h>

#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <memory>
#include <vector>
#include <cstring>

#include "domain/graph/ClassFileProcessor.h"
#include "domain/db/CallGraphDb.h"
#include "domain/classfile/ClassFileParser.h"
#include "domain/classfile/reader/ClassFileReader.h"
#include "infrastructure/log/Log.h"

using namespace kapa::tarracsh::domain;
using namespace graph;
using namespace classfile;
using namespace classfile::reader;

namespace {

std::vector<char> loadBinary(const std::filesystem::path &path) {
    std::ifstream file(path, std::ios::binary);
    return std::vector<char>(std::istreambuf_iterator<char>(file), {});
}

class BufferReader final : public ClassFileReader {
public:
    explicit BufferReader(std::vector<char> data)
        : _data(std::move(data)) {
        _size = static_cast<std::streamsize>(_data.size());
        readHeader();
    }

    void readBytes(char *buffer, unsigned int count) override {
        if (_position + count > _data.size()) {
            throw std::runtime_error("Read past end of buffer");
        }
        std::memcpy(buffer, _data.data() + _position, count);
        _position += count;
        _lastReadCount = count;
    }

private:
    std::vector<char> _data;
    size_t _position{0};
};

std::filesystem::path makeTempDir(const std::string &suffix) {
    const auto base = std::filesystem::temp_directory_path();
    const auto dir = base / ("tarracsh-classfileprocessor-" + suffix + "-" +
                             std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()));
    std::filesystem::create_directories(dir);
    return dir;
}

std::shared_ptr<kapa::infrastructure::log::Log> makeLog(const std::filesystem::path &dir) {
    auto log = std::make_shared<kapa::infrastructure::log::Log>();
    log->init((dir / "processor.log").string());
    return log;
}

std::filesystem::path testResource(const std::string &relative) {
    if (const char *srcDir = std::getenv("TEST_SRCDIR")) {
        if (const char *workspace = std::getenv("TEST_WORKSPACE")) {
            return std::filesystem::path(srcDir) / workspace / relative;
        }
    }
    return std::filesystem::path(PROJECT_SOURCE_DIR) / relative;
}

} // namespace

TEST(ClassFileProcessorTests, ExtractsMethodsFieldsAndReferences) {
    const auto tempDir = makeTempDir("db");
    auto log = makeLog(tempDir);
    kapa::infrastructure::db::Database::Config config{tempDir.string(), log.get()};
    auto db = db::callgraph::CallGraphDb::create(config, true, false);
    ASSERT_NE(db, nullptr);

    auto filesTable = db->getFiles();
    auto &fileRow = *reinterpret_cast<db::table::FileRow *>(filesTable->allocateRow());
    new(&fileRow) db::table::FileRow();
    fileRow.filename = db->getPoolString("sample.jar");
    filesTable->addOrUpdate(&fileRow);

    auto classFiles = db->getClassFiles();
    auto &classRow = *reinterpret_cast<db::table::ClassFileRow *>(classFiles->allocateRow());
    new(&classRow) db::table::ClassFileRow();
    classRow.file.id = fileRow.id;
    classRow.classname = db->getPoolString("org/kapa/reti/ast/BoolLiteralNode");
    classFiles->addOrUpdate(&classRow);

    const auto classPath =
        testResource("test-subjects/org/kapa/reti/ast/BoolLiteralNode.class");
    BufferReader reader(loadBinary(classPath));
    ClassFileParser parser(reader, classPath.string(), log);
    ASSERT_TRUE(parser.parse());

    auto processor = ClassFileProcessor(classFiles->getClassFileRow(classRow.id), parser, *db);
    processor.extractNodes();

    EXPECT_GT(db->getMethods()->size(), 0u);
    EXPECT_GE(db->getFields()->size(), 0u);
    EXPECT_GT(db->getMethodRefs()->size(), 0u);
    EXPECT_GT(db->getClassRefs()->size(), 0u);
}
