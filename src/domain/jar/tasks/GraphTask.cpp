#include "GraphTask.h"

#include <ranges>
#include <utility>

#include "../../digest/ClassFileDigest.h"
#include "../../classfile/ClassFileParser.h"
#include "../../classfile/reader/MemoryReader.h"
#include "../../digest/DigestUtils.h"
#include "infrastructure/filesystem/Utils.h"

using namespace kapa::infrastructure::filesystem;
using namespace kapa::tarracsh::domain::digest;

using namespace kapa::tarracsh::domain;

using kapa::infrastructure::db::table::column::DigestCol;

using namespace classfile;
using namespace db;
using namespace db::callgraph;
using namespace db::digest;
using namespace jar::tasks;

using namespace std;

GraphTask::GraphTask(
    Options options, Results &results,
    CallGraphDb &callGraphDb
    )
    : _callGraphDb(callGraphDb),
      _results(results),
      _options(move(options)) {
}

const db::table::ClassfileRow *GraphTask::getClassfileRow(const JarEntry &jarEntry) const {
    // const auto key = _callGraphDb.getClassfiles()->getStrongMethodName(
    //     *_jarFileRow,
    //     jarEntry.getClassname().c_str());
    // const tables::ClassfileRow *result = _callGraphDb.getClassfiles()->findByKey(key);
    //
    // return result;
    //TODO
    return nullptr;
}

bool GraphTask::start() {
    const auto &filename = _options.digest.input;
    _jarSize = filesystem::file_size(filename);
    _jarTimestamp = utils::getLastWriteTimestamp(filename);
    _jarFileRow = const_cast<table::FileRow *>(getJarFileRow(filename));
    _isFileUnchanged = isFileUnchanged();
    const auto result = !_isFileUnchanged;
    return result;
}

void GraphTask::processEntry(const JarEntry &jarEntry, std::mutex &taskMutex) {
    //
    // const auto *row = getClassfileRow(jarEntry);
    //
    // const auto isUnchanged = row != nullptr && isClassfileUnchanged(jarEntry, row);
    // const std::optional<DigestCol> classDigest =
    //     isUnchanged
    //         ? row->digest
    //         : parseEntry(jarEntry, row);
    //
    // ++_results.jarfiles.classfiles.count;
    // ++_results.jarfiles.classfiles.digest.count;
    //
    // if (isUnchanged) {
    //     ++_results.jarfiles.classfiles.digest.unchangedCount;
    // }
    // ++_jarFileRow->classfileCount;
    // std::unique_lock lock(taskMutex);
    // _digestMap[jarEntry.getClassname()] = classDigest.value();

}


void GraphTask::end() {
    // ++_results.jarfiles.digest.count;
    // _results.jarfiles.classfileCount += _jarFileRow->classfileCount;
    //
    // if (_isFileUnchanged) {
    //     _results.jarfiles.classfiles.digest.count += _jarFileRow->classfileCount;
    //     _results.jarfiles.classfiles.digest.unchangedCount += _jarFileRow->classfileCount;
    //     ++_results.jarfiles.digest.unchangedCount;
    //     return;
    // }
    //
    // digestUtils::DigestBuffer buffer;
    // for (auto &[buf] : _digestMap | views::values) {
    //     buffer.append(buf, DIGEST_LENGTH);
    // }
    // const auto digest = digestUtils::digest(buffer);
    //
    // const auto &filename = _options.digest.input;
    // const auto isSameDigest = !_isNewJarFile && _jarFileRow->digest == digest;
    //
    // if (isSameDigest) {
    //     _results.log->writeln(std::format("Same public digestEntry of changed jar file:{}", filename));
    //     ++_results.jarfiles.digest.same;
    // } else {
    //     _jarFileRow->digest = digest;
    //     if (_isNewJarFile) {
    //         ++_results.jarfiles.digest.newFile;
    //     } else {
    //         ++_results.jarfiles.digest.differentDigest;
    //     }
    // }

}


bool GraphTask::isFileUnchanged() const {
    auto const result = _options.useFileTimestamp &&
                        !_isNewJarFile && _jarFileRow->fileSize == _jarSize &&
                        _jarFileRow->lastWriteTime == _jarTimestamp;
    return result;
}

const table::FileRow *GraphTask::createJarFileRow(const std::string &filename) const {
    //TODO
    // tables::FileRow jarFileRow;
    // jarFileRow.filename = _callGraphDb.getPoolString(filename);
    // jarFileRow.type = tables::EntryType::Jar;
    // jarFileRow.lastWriteTime = _jarTimestamp;
    // jarFileRow.fileSize = _jarSize;
    // const auto id = _callGraphDb.getFiles()->add(jarFileRow);
    // const auto result = _callGraphDb.getFiles()->getRow(id);
    // return result;
    return nullptr;

}

const table::FileRow *GraphTask::getJarFileRow(const std::string &filename) {
    //TODO
    // auto result = _callGraphDb.getFiles()->findByKey(filename);
    // _isNewJarFile = result == nullptr;
    //
    // if (_isNewJarFile) {
    //     result = createJarFileRow(filename);
    // }
    //
    // return result;
    return nullptr;
}


optional<DigestCol> GraphTask::parseEntry(const JarEntry &jarEntry,
                                          const table::ClassfileRow *row) const {
    optional<DigestCol> result;
    Options classfileOptions(_options);
    reader::MemoryReader reader(jarEntry);

    ClassFileParser classFileParser(reader, classfileOptions, _results);
    if (classFileParser.parse()) {
        // const ClassFileDigest classFileDigest(classFileParser);
        // result = classFileDigest.digest();
        //
        // const bool rowAlreadyExists = row != nullptr;
        // const auto isSameDigest = rowAlreadyExists && result.value() == row->digest;
        //
        // if (isSameDigest) {
        //
        //     //TODO
        //     // _results.log->writeln(
        //     //     std::format("Same public digestEntry of changed file:{}",
        //     //                 _callGraphDb.getClassfiles()->getStrongMethodName(*row)));
        //     // ++_results.jarfiles.classfiles.digest.same;
        //     //
        //     // result = row->digest;
        //
        // } else {
        //
        //     // result = digest.value();
        //     //
        //     // if (rowAlreadyExists) {
        //     //     ++_results.jarfiles.classfiles.digest.differentDigest;
        //     // } else {
        //     //     ++_results.jarfiles.classfiles.digest.newFile;
        //     // }
        //
        // }

        // ++_results.jarfiles.classfiles.parsedCount;
        //TODO
        // const auto classname = classFileParser.getMainClassname();
        // tables::ClassfileRow digestRow(*_jarFileRow);
        // digestRow.lastWriteTime = jarEntry.getLastWriteTime();
        // digestRow.size = jarEntry.getSize();
        // digestRow.classname = _callGraphDb.getPoolString(classname);
        // digestRow.digest = digest.value();
        // digestRow.crc = jarEntry.getCRC();
        // digestRow.id = _callGraphDb.getClassfiles()->addOrUpdate(digestRow);

    } else {
        ++_results.jarfiles.classfiles.errors;
    }
    return result;
}


