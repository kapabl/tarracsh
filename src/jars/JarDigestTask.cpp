#include "JarDigestTask.h"

#include <ranges>
#include <utility>

#include "../classfile/ClassFileDigest.h"
#include "../classfile/readers/MemoryReader.h"
#include "../utils/DigestUtils.h"
#include "../utils/FilesystemUtils.h"

using namespace org::kapa::tarracsh::jar;
using namespace org::kapa::tarracsh::db;
using namespace org::kapa::tarracsh::stats;
using namespace std;

JarDigestTask::JarDigestTask(
    Options options, Results &results,
    db::DigestDb &digestDb
    )
    : _digestDb(digestDb),
      _results(results),
      _options(move(options)) {
}

string JarDigestTask::getStrongClassname(const JarEntry &jarEntry) const {
    const auto result = _digestDb.getClassfiles()->getStrongClassname(
        *_jarFileRow, jarEntry.getClassname().c_str());
    return result;
}

void JarDigestTask::processEntry(const JarEntry &jarEntry, std::mutex &taskMutex) {
    const DigestEntryInfo digestEntryInfo(*this, jarEntry);
    const auto* classfileRow = _digestDb.getClassfiles()->findByKey(digestEntryInfo.strongClassname);

    const auto classExists = nullptr != classfileRow;

    if (!classExists) {
        ++_jarFileRow->classfileCount;
    }

    const auto isUnchanged = classExists && isClassfileUnchanged(jarEntry, classfileRow);
    const std::optional<tables::columns::DigestCol> classDigest =
        isUnchanged
            ? classfileRow->digest
            : digestEntry(digestEntryInfo, classfileRow);

    ++_results.jarfiles.classfiles.count;
    ++_results.jarfiles.classfiles.digest.count;

    if (isUnchanged) {
        _results.report.asUnchangedClass(digestEntryInfo.strongClassname);
    }

    std::unique_lock lock(taskMutex);
    _digestMap[jarEntry.getClassname()] = classDigest.value();

}

void JarDigestTask::updateFileTableInMemory(const digestUtils::DigestVector digest) const {
    _jarFileRow->digest = digest;
    _jarFileRow->lastWriteTime = _jarTimestamp;
    _jarFileRow->fileSize = _jarSize;
    _digestDb.getFiles()->update(*_jarFileRow);
}

void JarDigestTask::end() {
    ++_results.jarfiles.digest.count;
    _results.jarfiles.classfileCount += _jarFileRow->classfileCount;

    if (_isFileUnchanged) {
        _results.jarfiles.classfiles.digest.count += _jarFileRow->classfileCount;
        _results.jarfiles.classfiles.digest.unchangedCount += _jarFileRow->classfileCount;
        _results.report.asUnchanged(_options.jarFile);
        return;
    }

    digestUtils::DigestBuffer buffer;
    for (auto &[buf] : _digestMap | views::values) {
        buffer.append(buf, DIGEST_LENGTH);
    }
    const auto digest = digestUtils::digest(buffer);

    const auto &filename = _options.jarFile;

    if (_isNewJarFile) {
        _results.report.asNew(filename);
    } else {
        const auto isSameDigest = _jarFileRow->digest == digest;
        _results.report.asModified(filename, isSameDigest);

        updateFileTableInMemory(digest);
        // updateClassfileTableInMemory(jarEntry, result.value(), classFileAnalyzer);
    }
}


bool JarDigestTask::isFileUnchanged() const {
    auto const result = _options.useFileTimestamp &&
                        !_isNewJarFile && _jarFileRow->fileSize == _jarSize &&
                        _jarFileRow->lastWriteTime == _jarTimestamp;
    return result;
}

const tables::FileRow *JarDigestTask::createJarFileRow(const std::string &filename) const {
    tables::FileRow jarFileRow;
    jarFileRow.filename = _digestDb.getPoolString(filename);
    jarFileRow.type = tables::columns::EntryType::Jar;
    jarFileRow.lastWriteTime = _jarTimestamp;
    jarFileRow.fileSize = _jarSize;
    const auto id = _digestDb.getFiles()->add(jarFileRow);
    const auto result = _digestDb.getFiles()->getRow(id);
    return result;

}

const tables::FileRow *JarDigestTask::getOrCreateFileRow(const std::string &filename) {
    auto result = _digestDb.getFiles()->findByKey(filename);
    _isNewJarFile = result == nullptr;

    if (_isNewJarFile) {
        result = createJarFileRow(filename);
    }

    return result;
}

bool JarDigestTask::start() {
    const auto &filename = _options.jarFile;
    _jarSize = filesystem::file_size(filename);
    _jarTimestamp = fsUtils::getLastWriteTimestamp(filename);
    _jarFileRow = const_cast<tables::FileRow *>(getOrCreateFileRow(filename));
    _isFileUnchanged = isFileUnchanged();
    const auto result = !_isFileUnchanged;
    return result;
}

void JarDigestTask::updateClassfileTableInMemory(const JarEntry &jarEntry, const tables::columns::DigestCol &result,
                                     const ClassFileAnalyzer &classFileAnalyzer) const {
    const auto classname = classFileAnalyzer.getMainClassname();
    tables::ClassfileRow classfileRow(*_jarFileRow);
    classfileRow.lastWriteTime = jarEntry.getLastWriteTime();
    classfileRow.size = jarEntry.getSize();
    classfileRow.classname = _digestDb.getPoolString(classname);
    classfileRow.digest = result;
    classfileRow.crc = jarEntry.getCRC();
    classfileRow.id = _digestDb.getClassfiles()->addOrUpdate(classfileRow);
}

optional<tables::columns::DigestCol> JarDigestTask::digestEntry(const DigestEntryInfo& digestEntryInfo,
                                                                const tables::ClassfileRow *row) const {
    optional<tables::columns::DigestCol> result;

    const auto& jarEntry = digestEntryInfo.jarEntry;
    Options options(_options);
    options.classFilePath = jarEntry.getName();
    readers::MemoryReader reader(jarEntry);
    ClassFileAnalyzer classFileAnalyzer(reader, options, _results);

    if (classFileAnalyzer.run()) {
        const ClassFileDigest classFileDigest(classFileAnalyzer);
        result = classFileDigest.digest();

        const bool classExists = row != nullptr;

        if (classExists) {
            const auto isSameDigest = result.value() == row->digest;
            _results.report.asModifiedClass(digestEntryInfo.strongClassname, isSameDigest);
        } else {
            _results.report.asNewClass(digestEntryInfo.strongClassname);
        }

        ++_results.jarfiles.classfiles.parsedCount;

        updateClassfileTableInMemory(jarEntry, result.value(), classFileAnalyzer);

    } else {
        ++_results.jarfiles.classfiles.errors;
    }
    return result;
}


bool JarDigestTask::isClassfileUnchanged(const JarEntry &jarEntry, const tables::ClassfileRow *classRow) {
    return classRow->size == jarEntry.getSize() &&
           classRow->crc == jarEntry.getCRC() &&
           classRow->lastWriteTime == jarEntry.getLastWriteTime();
}
