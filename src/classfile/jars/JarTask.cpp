#include "JarTask.h"

#include <ranges>
#include <utility>
#include "../ClassFileAnalyzer.h"
#include "../readers/MemoryReader.h"
#include "../FilesystemUtils.h"

using namespace org::kapa::tarracsh::jar;
using namespace org::kapa::tarracsh::tables;
using namespace std;

JarDigestTask::JarDigestTask(
    Options options, Results &results,
    std::shared_ptr<DigestTable> digestTable)
    : _digestTable(std::move(digestTable)),

      _results(results),
      _options(move(options)) {
}

DigestRow *JarDigestTask::getClassfileRow(const JarEntry &jarEntry) const {
    const auto key = _digestTable->createKey(
        _options.jarFile.c_str(),
        jarEntry.getClassname().c_str());
    DigestRow *result = _digestTable->findByKey(key);

    return result;
}

void JarDigestTask::processEntry(const JarEntry &jarEntry, std::mutex &taskMutex) {
    const auto classDigest = digestEntry(jarEntry);
    {
        std::unique_lock lock(taskMutex);
        _digestMap[jarEntry.getClassname()] = classDigest.value();
    }
}

void JarDigestTask::end() {
    if (!_isFileUnchanged) {

        Poco::MD5Engine md5Engine;
        Poco::DigestOutputStream stream(md5Engine);

        for (const auto &[buf] : _digestMap | views::values) {
            stream.write(reinterpret_cast<const char *>(buf), MD5_DIGEST_LENGTH);
        }

        stream.close();
        memcpy(_digest.buf, &*md5Engine.digest().begin(), MD5_DIGEST_LENGTH);

        const auto &filename = _options.jarFile;
        const auto isSameDigest = _isExistingJar && _digest == _jarRow->md5;

        if (isSameDigest) {
            _results.resultLog.writeln(std::format("Same public digestEntry of changed jar file:{}", filename));
            _results.jarfiles.digest.same++;
        } else {

            DigestRow jarRow;
            jarRow.filename = _digestTable->getPoolString(filename);
            jarRow.type = EntryType::Jar;
            jarRow.lastWriteTime = _jarTimestamp;
            jarRow.fileSize = _jarSize;
            jarRow.md5 = _digest;
            _digestTable->addOrUpdate(jarRow);

            if (_isExistingJar) {
                _results.jarfiles.digest.differentDigest++;
            } else {
                _results.jarfiles.digest.newFile++;
            }
        }
        _results.jarfiles.digest.count++;

    } else {
        _results.jarfiles.digest.count += static_cast<int>(_jarRow->count);
        _results.jarfiles.digest.unchangedCount += static_cast<int>(_jarRow->count);
    }
}


bool JarDigestTask::isFileUnchanged() const {
    auto const result = _options.useFileTimestamp &&
                        _jarRow != nullptr &&
                        _jarRow->fileSize == _jarSize &&
                        _jarRow->lastWriteTime == _jarTimestamp;
    return result;
}

bool JarDigestTask::start() {
    const auto &filename = _options.jarFile;
    _jarSize = filesystem::file_size(filename);
    _jarTimestamp = fsUtils::getLastWriteTimestamp(filename);
    _jarRow = _digestTable->findByKey(filename);
    _isExistingJar = _jarRow != nullptr;
    _isFileUnchanged = isFileUnchanged();
    const auto result = !_isFileUnchanged;
    return result;
}

std::optional<MD5> JarDigestTask::digestEntry(
    const JarEntry &jarEntry) const {
    std::optional<MD5> result;
    _results.jarfiles.classfiles.digest.count++;
    const auto *row = getClassfileRow(jarEntry);

    if (row != nullptr && isClassfileUnchanged(jarEntry, row)) {
        _results.jarfiles.classfiles.digest.unchangedCount++;
        result = row->md5;
        return result;
    }

    result = parseEntry(jarEntry, row);

    return result;

}

optional<MD5> JarDigestTask::parseEntry(const JarEntry &jarEntry, const DigestRow *row) const {
    optional<MD5> result;
    Options classfileOptions(_options);
    classfileOptions.classFilePath = jarEntry.getName();
    readers::MemoryReader reader(jarEntry);

    ClassFileAnalyzer classFileAnalyzer(reader, classfileOptions, _results);
    const auto digest = classFileAnalyzer.getPublicDigest();

    if (digest.has_value()) {
        const bool alreadyExists = row != nullptr;
        const auto isSameDigest = alreadyExists && digest.value() == row->md5;

        if (isSameDigest) {
            _results.resultLog.writeln(std::format("Same public digestEntry of changed file:{}",
                                                   _digestTable->createKey(*row)));
            _results.jarfiles.classfiles.digest.same++;
            result = row->md5;
        } else {
            const auto classname = classFileAnalyzer.getMainClassname();
            DigestRow newRow;
            newRow.filename = _digestTable->getPoolString(_options.jarFile);
            newRow.type = EntryType::Classfile;
            newRow.lastWriteTime = jarEntry.getLastWriteTime();
            newRow.fileSize = jarEntry.getSize();
            newRow.classname = _digestTable->getPoolString(classname);
            newRow.md5 = digest.value();
            _digestTable->addOrUpdate(newRow);

            result = newRow.md5;

            if (alreadyExists) {
                _results.jarfiles.classfiles.digest.differentDigest++;
            } else {
                _results.jarfiles.classfiles.digest.newFile++;
            }
        }

        _results.jarfiles.classfiles.parsedCount++;

    } else {
        _results.jarfiles.classfiles.errors++;
    }
    return result;
}


bool JarDigestTask::isClassfileUnchanged(const JarEntry &jarEntry, const DigestRow *classRow) const {
    return classRow->fileSize == jarEntry.getSize() &&
           classRow->crc == jarEntry.getCRC() &&
           classRow->lastWriteTime == jarEntry.getLastWriteTime();
}
