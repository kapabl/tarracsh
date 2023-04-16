#include "Task.h"


using namespace  kapa::tarracsh::domain::jar;
using kapa::tarracsh::domain::db::table::FileRow;
using kapa::tarracsh::domain::db::digest::column::EntryType;

uint32_t Task::getCount() { return _jarFileRow->classfileCount; }

Task::Task(Options options, stats::Results &results):
    _results(results),
    _options(std::move(options)) {
}

Task::~Task() {
    if (_tempFileRow != nullptr) {
        free(_tempFileRow);
        // getFileTable()->freeRow(_tempFileRow);
    }
}

FileRow & Task::getJarFileRow() const { return *_jarFileRow; }

bool Task::isClassfileUnchanged(const JarEntry &jarEntry,
                                const db::table::ClassfileRow *classRow) {
    return classRow->size == jarEntry.getSize() &&
           classRow->crc == jarEntry.getCRC() &&
           classRow->lastWriteTime == jarEntry.getLastWriteTime();
}

FileRow* Task::getFileRow(const std::string& filename) {
    const auto result = static_cast<FileRow*>(getFileTable()->findByKey(filename));
    return result;
}

bool Task::isFileUnchanged() const {
    auto const result = _options.useFileTimestamp &&
        !_isNewJarFile && _jarFileRow->fileSize == _jarSize &&
        _jarFileRow->lastWriteTime == _jarTimestamp;
    return result;
}

FileRow * Task::getOrCreateFileRow(const std::string &filename) {
    auto result = getFileRow(filename);
    _isNewJarFile = result == nullptr;

    if (_isNewJarFile) {
        result = createJarFileRow(filename);
        if (!_options.digest.dryRun) {
            getFileTable()->add(result);
        }
        else {
            _tempFileRow = result;
        }
    }

    return result;
}

FileRow* Task::createJarFileRow(const std::string& filename) {
    auto result = static_cast<FileRow*>(getFileTable()->allocateRow());
    new (result) FileRow();
    result->filename = getDb().getPoolString(filename);
    result->type = EntryType::Jar;
    result->lastWriteTime = _jarTimestamp;
    result->fileSize = _jarSize;
    result->classfileCount = 0;
    return result;

}