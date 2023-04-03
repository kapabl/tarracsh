#include "DigestTask.h"

#include <ranges>
#include <utility>

#include "../../digest/ClassFileDigest.h"
#include "../../classfile/reader/MemoryReader.h"
#include "../../digest/DigestEntryInfo.h"
#include "../../digest/DigestUtils.h"
#include "../../../infrastructure/filesystem/Utils.h"

using namespace kapa::infrastructure::filesystem;
using namespace kapa::tarracsh::domain::digest;
using namespace kapa::tarracsh::domain;

using kapa::infrastructure::db::tables::columns::DigestCol;
using kapa::infrastructure::db::tables::AutoIncrementedRow;

using namespace classfile;
using namespace db;
using namespace db::digest;
using namespace jar::tasks;

using namespace std;

DigestTask::DigestTask(
    Options options, Results &results,
    DigestDb &digestDb
    )
    : _db(digestDb),
      _results(results),
      _options(move(options)) {
}

DigestTask::~DigestTask() {
    if (_tempFileRow != nullptr) {
        _db.getFiles()->freeRow(_tempFileRow);
    }
}

void DigestTask::processEntry(const JarEntry &jarEntry, std::mutex &taskMutex) {
    const auto filename = _db.getFiles()->getFilename(&getJarFileRow());
    const DigestJarEntryInfo digestEntryInfo(filename, jarEntry);

    const auto *classfileRow = static_cast<ClassfileRow *>(_db.getClassfiles()->findByKey(
        digestEntryInfo.strongClassname));

    const auto classExists = nullptr != classfileRow;

    if (!classExists) {
        ++_jarFileRow->classfileCount;
    }

    const auto isUnchanged = classExists && isClassfileUnchanged(jarEntry, classfileRow);
    if (isUnchanged) {
        _results.report->asUnchangedJarClass(digestEntryInfo.strongClassname);
        std::unique_lock lock(taskMutex);
        _digestMap[jarEntry.getClassname()] = classfileRow->digest;
    } else {
        const auto digest = digestEntry(digestEntryInfo, classfileRow);
        if (digest.has_value()) {
            std::unique_lock lock(taskMutex);
            _digestMap[jarEntry.getClassname()] = digest.value();
        }
    }

    ++_results.jarfiles.classfiles.count;
    ++_results.jarfiles.classfiles.digest.count;

}

void DigestTask::updateFileTableInMemory(const digestUtils::DigestVector &digest) const {
    _jarFileRow->digest = digest;
    _jarFileRow->lastWriteTime = _jarTimestamp;
    _jarFileRow->fileSize = _jarSize;
    _db.getFiles()->update(_jarFileRow);
}

bool DigestTask::start() {
    const auto &filename = _options.digest.input;
    _jarSize = filesystem::file_size(filename);
    _jarTimestamp = utils::getLastWriteTimestamp(filename);
    _jarFileRow = getOrCreateFileRow(filename);
    _isFileUnchanged = isFileUnchanged();
    const auto result = !_isFileUnchanged;
    return result;
}

void DigestTask::end() {
    ++_results.jarfiles.digest.count;
    _results.jarfiles.classfileCount.fetch_add(_jarFileRow->classfileCount);

    if (_isFileUnchanged) {
        _results.jarfiles.classfiles.digest.count.fetch_add(_jarFileRow->classfileCount);
        _results.jarfiles.classfiles.digest.unchangedCount.fetch_add(_jarFileRow->classfileCount);
        _results.report->asUnchangedJar(_options.digest.input);
        return;
    }

    digestUtils::DigestBuffer buffer;
    for (auto &[buf] : _digestMap | views::values) {
        buffer.append(buf, DIGEST_LENGTH);
    }
    const auto digest = digestUtils::digest(buffer);

    const auto &filename = _options.digest.input;

    if (_isNewJarFile) {
        _results.report->asNewJar(filename);
    } else {
        const auto isSameDigest = _jarFileRow->digest == digest;
        _results.report->asModifiedJar(filename, isSameDigest);
    }
    if (!_options.digest.dryRun) {
        updateFileTableInMemory(digest);
    }
}


bool DigestTask::isFileUnchanged() const {
    auto const result = _options.useFileTimestamp &&
                        !_isNewJarFile && _jarFileRow->fileSize == _jarSize &&
                        _jarFileRow->lastWriteTime == _jarTimestamp;
    return result;
}

FileRow *DigestTask::createJarFileRow(const std::string &filename) const {
    auto result = static_cast<FileRow *>(_db.getFiles()->allocateRow());
    new (result) FileRow();
    result->filename = _db.getPoolString(filename);
    result->type = columns::EntryType::Jar;
    result->lastWriteTime = _jarTimestamp;
    result->fileSize = _jarSize;
    result->classfileCount = 0;
    return result;

}

FileRow *DigestTask::getOrCreateFileRow(const std::string &filename) {
    auto result = static_cast<FileRow *>(_db.getFiles()->findByKey(filename));
    _isNewJarFile = result == nullptr;

    if (_isNewJarFile) {
        result = createJarFileRow(filename);
        if (!_options.digest.dryRun) {
            _db.getFiles()->add(result);
        } else {
            _tempFileRow = result;
        }
    }

    return result;
}

string DigestTask::getUniqueClassname(
    const JarEntry &jarEntry,
    const ClassFileParser &classFileParser) {
    auto result = jarEntry.isMultiReleaseEntry()
                      ? jarEntry.getClassname()
                      : classFileParser.getMainClassname();
    return result;

}

void DigestTask::updateClassfileTableInMemory(const JarEntry &jarEntry, const DigestCol &result,
                                              const ClassFileParser &classFileParser) const {
    const auto classname = getUniqueClassname(jarEntry, classFileParser);
    auto &classfileRow = *static_cast<ClassfileRow *>(_db.getClassfiles()->allocateRow());
    new (&classfileRow) ClassfileRow(*_jarFileRow);
    classfileRow.lastWriteTime = jarEntry.getLastWriteTime();
    classfileRow.size = jarEntry.getSize();
    classfileRow.classname = _db.getPoolString(classname);
    classfileRow.digest = result;
    classfileRow.crc = jarEntry.getCRC();
    _db.getClassfiles()->addOrUpdate(&classfileRow);
}

optional<DigestCol> DigestTask::digestEntry(const DigestJarEntryInfo &digestEntryInfo,
                                            const ClassfileRow *row) const {
    const auto &jarEntry = digestEntryInfo.jarEntry;

    optional<DigestCol> result;

    Options options(_options);
    reader::MemoryReader reader(jarEntry);

    ClassFileParser classFileParser(reader, options, _results);
    if (classFileParser.parse()) {
        const ClassFileDigest classFileDigest(classFileParser);
        result = classFileDigest.digest();

        const bool classExists = row != nullptr;

        if (classExists) {
            const auto isSamePublicDigest = result.value() == row->digest;
            _results.report->asModifiedJarClass(digestEntryInfo.strongClassname, isSamePublicDigest);
        } else {
            _results.report->asNewJarClass(digestEntryInfo.strongClassname);
        }

        ++_results.jarfiles.classfiles.parsedCount;

        if (!_options.digest.dryRun) {
            updateClassfileTableInMemory(jarEntry, result.value(), classFileParser);
        }

    } else {
        _results.report->asFailedJarClass(digestEntryInfo.strongClassname);
        ++_results.jarfiles.classfiles.errors;
    }
    return result;
}


bool DigestTask::isClassfileUnchanged(const JarEntry &jarEntry, const ClassfileRow *classRow) {
    return classRow->size == jarEntry.getSize() &&
           classRow->crc == jarEntry.getCRC() &&
           classRow->lastWriteTime == jarEntry.getLastWriteTime();
}
