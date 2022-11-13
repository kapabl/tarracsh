#include "JarDigestTask.h"

#include <ranges>
#include <utility>

#include "../ClassFileAnalyzer.h"
#include "../readers/MemoryReader.h"
#include "../FilesystemUtils.h"

using namespace org::kapa::tarracsh::jar;
using namespace org::kapa::tarracsh::tables;
using namespace org::kapa::tarracsh::stats;
using namespace std;

JarDigestTask::JarDigestTask(
    Options options, Results &results,
    std::shared_ptr<ClassfilesTable> digestTable,
    std::shared_ptr<FilesTable> filesTable
    )
    : _digestTable(std::move(digestTable)),
      _filesTable(std::move(filesTable)),
      _results(results),
      _options(move(options)) {
}

const ClassfileRow *JarDigestTask::getClassfileRow(const JarEntry &jarEntry) const {
    const auto key = _digestTable->createKey(
        *_jarFileRow,
        jarEntry.getClassname().c_str());
    const ClassfileRow *result = _digestTable->findByKey(key);

    return result;
}

void JarDigestTask::processEntry(const JarEntry &jarEntry, std::mutex &taskMutex) {

    const auto *row = getClassfileRow(jarEntry);

    const auto isUnchanged = row != nullptr && isClassfileUnchanged(jarEntry, row);
    const std::optional<Md5Column> classDigest =
        isUnchanged
            ? row->md5
            : parseEntry(jarEntry, row);

    ++_results.jarfiles.classfiles.count;
    ++_results.jarfiles.classfiles.digest.count;

    if (isUnchanged) {
        ++_results.jarfiles.classfiles.digest.unchangedCount;
    }
    ++_jarFileRow->classfileCount;
    std::unique_lock lock(taskMutex);
    _digestMap[jarEntry.getClassname()] = classDigest.value();

}

void JarDigestTask::end() {
    ++_results.jarfiles.digest.count;
    _results.jarfiles.classfileCount += _jarFileRow->classfileCount;

    if (_isFileUnchanged) {
        _results.jarfiles.classfiles.digest.count += _jarFileRow->classfileCount;
        _results.jarfiles.classfiles.digest.unchangedCount += _jarFileRow->classfileCount;
        ++_results.jarfiles.digest.unchangedCount;
        return;
    }

    Poco::MD5Engine md5Engine;
    Poco::DigestOutputStream stream(md5Engine);

    for (const auto &[buf] : _digestMap | views::values) {
        stream.write(reinterpret_cast<const char *>(buf), MD5_DIGEST_LENGTH);
    }

    stream.close();
    Md5Column digest;
    memcpy(digest.buf, &*md5Engine.digest().begin(), MD5_DIGEST_LENGTH);

    const auto &filename = _options.jarFile;
    const auto isSameDigest = !_isNewJarFile && digest == _jarFileRow->md5;

    if (isSameDigest) {
        _results.resultLog.writeln(std::format("Same public digestEntry of changed jar file:{}", filename));
        ++_results.jarfiles.digest.same;
    } else {
        _jarFileRow->md5 = digest;
        if (_isNewJarFile) {
            ++_results.jarfiles.digest.newFile;
        } else {
            ++_results.jarfiles.digest.differentDigest;
        }
    }

}


bool JarDigestTask::isFileUnchanged() const {
    auto const result = _options.useFileTimestamp &&
                        !_isNewJarFile && _jarFileRow->fileSize == _jarSize &&
                        _jarFileRow->lastWriteTime == _jarTimestamp;
    return result;
}

const FileRow *JarDigestTask::createJarFileRow(const std::string &filename) const {
    FileRow jarFileRow;
    jarFileRow.filename = _filesTable->getPoolString(filename);
    jarFileRow.type = Jar;
    jarFileRow.lastWriteTime = _jarTimestamp;
    jarFileRow.fileSize = _jarSize;
    const auto id = _filesTable->add(jarFileRow);
    const auto result = _filesTable->getRow(id);
    return result;

}

const FileRow *JarDigestTask::getJarFileRow(const std::string &filename) {
    auto result = _filesTable->findByKey(filename);
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
    _jarFileRow = const_cast<FileRow *>(getJarFileRow(filename));
    _isFileUnchanged = isFileUnchanged();
    const auto result = !_isFileUnchanged;
    return result;
}

optional<Md5Column> JarDigestTask::parseEntry(const JarEntry &jarEntry,
                                              const ClassfileRow *row) const {
    optional<Md5Column> result;
    Options classfileOptions(_options);
    classfileOptions.classFilePath = jarEntry.getName();
    readers::MemoryReader reader(jarEntry);

    ClassFileAnalyzer classFileAnalyzer(reader, classfileOptions, _results);
    const auto digest = classFileAnalyzer.getPublicDigest();

    if (digest.has_value()) {
        const bool rowAlreadyExists = row != nullptr;
        const auto isSameDigest = rowAlreadyExists && digest.value() == row->md5;

        if (isSameDigest) {

            _results.resultLog.writeln(
                std::format("Same public digestEntry of changed file:{}",
                            _digestTable->createKey(*row)));
            ++_results.jarfiles.classfiles.digest.same;

            result = row->md5;
        } else {
            const auto classname = classFileAnalyzer.getMainClassname();

            ClassfileRow digestRow(*_jarFileRow);
            digestRow.lastWriteTime = jarEntry.getLastWriteTime();
            digestRow.size = jarEntry.getSize();
            digestRow.classname = _digestTable->getPoolString(classname);
            digestRow.md5 = digest.value();
            digestRow.id = _digestTable->addOrUpdate(digestRow);

            result = digestRow.md5;

            if (rowAlreadyExists) {
                ++_results.jarfiles.classfiles.digest.differentDigest;
            } else {
                ++_results.jarfiles.classfiles.digest.newFile;
            }

        }

        ++_results.jarfiles.classfiles.parsedCount;

    } else {
        ++_results.jarfiles.classfiles.errors;

    }
    return result;
}


bool JarDigestTask::isClassfileUnchanged(const JarEntry &jarEntry, const ClassfileRow *classRow) {
    return classRow->size == jarEntry.getSize() &&
           classRow->crc == jarEntry.getCRC() &&
           classRow->lastWriteTime == jarEntry.getLastWriteTime();
}
