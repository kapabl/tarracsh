#include "DbBasedTask.h"
#include "domain/classfile/ClassFileParser.h"


using namespace kapa::tarracsh::domain::jar;
using kapa::tarracsh::domain::db::table::FileRow;
using kapa::tarracsh::domain::db::table::ClassFileRow;

// TODO move EntryType to outside digest namespace
using kapa::tarracsh::domain::db::digest::column::EntryType;

using kapa::tarracsh::domain::classfile::ClassFileParser;
using kapa::infrastructure::db::table::column::DigestCol;


DbBasedTask::DbBasedTask(const Options &options, stats::Results &results)
    : Task(options, results) {
}


DbBasedTask::~DbBasedTask() {
    if (_tempFileRow != nullptr) {
        free(_tempFileRow);
    }
}

FileRow *DbBasedTask::getOrCreateFileRow(const std::string &filename) {
    auto result = getFileRow(filename);
    _isNewJarFile = result == nullptr;

    if (_isNewJarFile) {
        result = createJarFileRow(filename);
        if (!_options.getSubCommandOptions().dryRun) {
            getFiles()->add(result);
        } else {
            _tempFileRow = result;
        }
    }

    return result;
}

FileRow *DbBasedTask::createJarFileRow(const std::string &filename) {
    auto* result = static_cast<FileRow *>(getFiles()->allocateRow());
    new(result) FileRow();
    result->filename = getDb().getPoolString(filename);
    result->type = EntryType::Jar;
    result->lastWriteTime = _jarTimestamp;
    result->fileSize = _jarSize;
    result->classfileCount = 0;
    return result;

}

FileRow *DbBasedTask::getFileRow(const std::string &filename) {
    const auto result = static_cast<FileRow *>(getFiles()->findByKey(filename));
    return result;
}

bool DbBasedTask::isClassfileUnchanged(const JarEntry &jarEntry,
                                       const db::table::ClassFileRow *classRow) {
    return classRow->size == jarEntry.getSize() &&
           classRow->crc == jarEntry.getCRC() &&
           classRow->hasValidFile() &&
           classRow->lastWriteTime == jarEntry.getLastWriteTime();
}

bool DbBasedTask::isFileUnchanged() const {
    auto const result = _options.useFileTimestamp &&
                        !_isNewJarFile && _jarFileRow->fileSize == _jarSize &&
                        _jarFileRow->lastWriteTime == _jarTimestamp;
    return result;
}

FileRow & DbBasedTask::getJarFileRow() const {
    return *_jarFileRow;
}

std::string DbBasedTask::getUniqueClassname(
    const JarEntry& jarEntry,
    const ClassFileParser& classFileParser) {
    auto result = jarEntry.isMultiReleaseEntry()
        ? jarEntry.getClassname()
        : classFileParser.getMainClassname();
    return result;

}

void DbBasedTask::updateFile(const digestUtils::DigestVector& digest) {
    _jarFileRow->digest = digest;
    _jarFileRow->lastWriteTime = _jarTimestamp;
    _jarFileRow->fileSize = _jarSize;
    getFiles()->update(_jarFileRow);
}


uint64_t DbBasedTask::addOrUpdateClassfile(const JarEntry& jarEntry, const ClassFileParser &classFileParser) {
    const DigestCol emptyDigestCol;
    return addOrUpdateClassfile(jarEntry, emptyDigestCol, classFileParser);
}


uint64_t  DbBasedTask::addOrUpdateClassfile(const JarEntry& jarEntry, const DigestCol &digestCol,
                                            const ClassFileParser &classFileParser) {

    const auto classfileTable = getClassFiles();
    const auto classname = getUniqueClassname(jarEntry, classFileParser);
    auto& classfileRow = *static_cast<ClassFileRow*>(classfileTable->allocateRow());
    new(&classfileRow) ClassFileRow(*_jarFileRow);
    classfileRow.lastWriteTime = jarEntry.getLastWriteTime();
    classfileRow.size = jarEntry.getSize();
    classfileRow.classname = getDb().getPoolString(classname);
    classfileRow.digest = digestCol;
    classfileRow.crc = jarEntry.getCRC();
    classfileTable->addOrUpdate(&classfileRow);

    const auto result = classfileRow.id;
    return result;
}