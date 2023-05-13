#include "DigestTask.h"

#include <ranges>
#include <utility>

#include "domain/digest/ClassFileDigest.h"
#include "domain/classfile/reader/MemoryReader.h"
#include "domain/jar/JarEntryInfo.h"
#include "domain/digest/DigestUtils.h"
#include "infrastructure/filesystem/Utils.h"

using namespace kapa::infrastructure::filesystem;
using namespace kapa::tarracsh::domain::digest;
using namespace kapa::tarracsh::domain::db::table;
using namespace kapa::tarracsh::domain;

using kapa::infrastructure::db::table::column::DigestCol;

using namespace classfile;
using namespace db;
using namespace db::digest;
using namespace jar::tasks;

using namespace std;

DigestTask::DigestTask(
    Options options, Results &results,
    DigestDb &digestDb
    )
    : DbBasedTask(std::move(options), results), _db(digestDb) {
}

DigestTask::~DigestTask() {
}

void DigestTask::processEntry(const JarEntry &jarEntry, std::mutex &taskMutex) {
    const auto filename = _db.getFiles()->getFilename(&getJarFileRow());
    const JarEntryInfo jarEntryInfo(filename, jarEntry);

    const auto *classfileRow = static_cast<ClassfileRow *>(getClassfiles()->findByKey(
        jarEntryInfo.strongClassname));

    const auto classExists = nullptr != classfileRow;

    if (!classExists) {
        ++_jarFileRow->classfileCount;
    }

    const auto isUnchanged = classExists && isClassfileUnchanged(jarEntry, classfileRow);
    if (isUnchanged) {
        _results.report->asUnchangedJarClass(jarEntryInfo.strongClassname);
        std::unique_lock lock(taskMutex);
        _digestMap[jarEntry.getClassname()] = classfileRow->digest;
    } else {
        const auto digest = digestEntry(jarEntryInfo, classfileRow);
        if (digest.has_value()) {
            std::unique_lock lock(taskMutex);
            _digestMap[jarEntry.getClassname()] = digest.value();
        }
    }

    ++_results.jarfiles.classfiles.count;
    ++_results.jarfiles.classfiles.taskResult.count;

}

bool DigestTask::start() {
    const auto &filename = _options.digest.input;
    _jarSize = filesystem::file_size(filename);
    _jarTimestamp = kapa::infrastructure::filesystem::utils::getLastWriteTimestamp(filename);
    _jarFileRow = getOrCreateFileRow(filename);
    _isFileUnchanged = isFileUnchanged();
    const auto result = !_isFileUnchanged;
    return result;
}

void DigestTask::end() {
    ++_results.jarfiles.taskResult.count;
    _results.jarfiles.classfileCount.fetch_add(_jarFileRow->classfileCount);

    if (_isFileUnchanged) {
        _results.jarfiles.classfiles.taskResult.count.fetch_add(_jarFileRow->classfileCount);
        _results.jarfiles.classfiles.taskResult.unchangedCount.fetch_add(_jarFileRow->classfileCount);
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

std::shared_ptr<Files> DigestTask::getFiles() {
    const auto result = _db.getFiles();
    return result;
}

kapa::infrastructure::db::Database &DigestTask::getDb() {
    return _db;
}

auto DigestTask::getClassfiles() -> std::shared_ptr<db::table::Classfiles> {
    return _db.getClassfiles();
}

optional<DigestCol> DigestTask::digestEntry(const JarEntryInfo &digestEntryInfo,
                                            const ClassfileRow *row) {
    const auto &jarEntry = digestEntryInfo.jarEntry;

    optional<DigestCol> result;

    reader::MemoryReader reader(jarEntry);
    ClassFileParser classFileParser(reader, jarEntry.getName(), _results.log);
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
