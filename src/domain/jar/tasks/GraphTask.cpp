#include "GraphTask.h"

#include <ranges>
//#include <utility>

#include "domain/db/table/ClassRefs.h"
#include "domain/db/table/Methods.h"
#include "domain/db/table/MethodRefs.h"
#include "domain/db/table/Fields.h"
#include "domain/db/table/FieldRefs.h"
#include "infrastructure/filesystem/Utils.h"
#include "domain/classfile/reader/MemoryReader.h"
#include "domain/graph/ClassFileProcessor.h"

using namespace kapa::infrastructure::filesystem;
using namespace std;

using kapa::tarracsh::domain::classfile::reader::MemoryReader;
using kapa::tarracsh::domain::classfile::ClassFileParser;
using kapa::tarracsh::domain::db::table::MethodRow;
using kapa::tarracsh::domain::db::table::MethodRefRow;
using kapa::tarracsh::domain::db::table::Files;
using kapa::tarracsh::domain::db::table::FieldRow;
using kapa::tarracsh::domain::db::table::FieldRefRow;
using kapa::tarracsh::domain::db::table::ClassRefRow;
using kapa::tarracsh::domain::db::table::ClassfileRow;
using kapa::tarracsh::domain::db::callgraph::CallGraphDb;
using kapa::tarracsh::domain::graph::ClassFileProcessor;

using kapa::tarracsh::domain::classfile::constantpool::u2;
using kapa::tarracsh::domain::classfile::constantpool::u1;
using kapa::tarracsh::domain::classfile::constantpool::u4;
using kapa::tarracsh::domain::classfile::constantpool::ConstantPoolRecord;

using namespace kapa::tarracsh::domain::jar::tasks;

GraphTask::GraphTask(const Options &options, Results &results,
                     CallGraphDb &callGraphDb)
        : DbBasedTask(options, results), _db(callGraphDb) {
}


void GraphTask::updateIncompleteRefs(const ClassfileRow *row) {
    //TODO check if pending method-refs, class-refs, a field-refs can match
    //this class and its methods
}

void GraphTask::processNewClassfile(const JarEntryInfo &jarEntryInfo) {
    const auto &jarEntry = jarEntryInfo.jarEntry;

    MemoryReader reader(jarEntry);

    ClassFileParser classFileParser(reader, jarEntry.getName(), _results.log);
    if (classFileParser.parse()) {
        auto row = getClassfileRow(addOrUpdateClassfile(jarEntry, classFileParser));
        auto classFileProcessor = ClassFileProcessor(row, classFileParser, _db );
        classFileProcessor.extractNodes();
//        updateIncompleteRefs(row);

    } else {
        _results.report->asFailedJarClass(jarEntryInfo.strongClassname);
        ++_results.jarfiles.classfiles.errors;
    }

}

const ClassfileRow *GraphTask::getClassfileRow(uint64_t id) {
    const auto result = reinterpret_cast<const ClassfileRow *>(getClassfiles()->getRow(id));
    return result;
}

void GraphTask::processIncompleteClassfile(const JarEntryInfo &jarEntryInfo, ClassfileRow *row) {
    const auto &jarEntry = jarEntryInfo.jarEntry;

    //TODO update file row
    row->file.id = _jarFileRow->id;

    MemoryReader reader(jarEntry);

    ClassFileParser classFileParser(reader, jarEntry.getName(), _results.log);
    if (classFileParser.parse()) {
        auto classFileProcessor = ClassFileProcessor(row, classFileParser, _db );
        classFileProcessor.extractNodes();
    } else {
        _results.report->asFailedJarClass(jarEntryInfo.strongClassname);
        ++_results.jarfiles.classfiles.errors;
    }
    updateIncompleteRefs(row);
}

void GraphTask::markMemberRefsAsIncomplete(ClassfileRow *row) {
    //TODO
}

void GraphTask::deleteMembers(ClassfileRow *row) {
    //TODO
}

void GraphTask::reProcessClassfile(const JarEntryInfo &jarEntryInfo, db::table::ClassfileRow *row) {

    markMemberRefsAsIncomplete(row);
    deleteMembers(row);
    processNewClassfile(jarEntryInfo);

    //TODO check all method refs to this class:
    //parse and check methods/fields that were deleted and mark refs as incomplete
    //delete methods -> check refs to it and mark as "incomplete" or set class RefCol to Invalid


    //try to complete incompleteMethods

}


void GraphTask::processClassfile(const JarEntryInfo &jarEntryInfo, ClassfileRow *row) {

    if (row == nullptr) {
        processNewClassfile(jarEntryInfo);
    } else if (isIncompleteClassfileRow(row)) {
        processIncompleteClassfile(jarEntryInfo, row);
    } else {
        reProcessClassfile(jarEntryInfo, row);
    }
}

bool GraphTask::isIncompleteClassfileRow(const db::table::ClassfileRow *classfileRow) {
    const bool result = !classfileRow->hasValidFile();
    return result;
}

bool GraphTask::start() {
    const auto &filename = _options.digest.input;
    _jarSize = filesystem::file_size(filename);
    _jarTimestamp = kapa::infrastructure::filesystem::utils::getLastWriteTimestamp(filename);
    _jarFileRow = getOrCreateFileRow(filename);
    _isFileUnchanged = isFileUnchanged();
    const auto result = !_isFileUnchanged;
    return result;
}

void GraphTask::processEntry(const JarEntry &jarEntry, std::mutex &taskMutex) {

    const auto filename = _db.getFiles()->getFilename(&getJarFileRow());
    const JarEntryInfo jarEntryInfo(filename, jarEntry);

    auto classfileRow = static_cast<ClassfileRow *>(_db.getClassfiles()->findByKey(
            jarEntryInfo.strongClassname));

    const auto classExists = nullptr != classfileRow;
    //const bool isIncomplete = isIncompleteClassfileRow(classfileRow);

    if (!classExists) {
        ++_jarFileRow->classfileCount;
    }

    const auto isUnchanged = classExists && isClassfileUnchanged(jarEntry, classfileRow);
    if (isUnchanged) {
        _results.report->asUnchangedJarClass(jarEntryInfo.strongClassname);
    } else {
        if (classfileRow == nullptr) {
            classfileRow = findIncompleteClass(jarEntryInfo);
        }
        processClassfile(jarEntryInfo, classfileRow);
    }

    ++_results.jarfiles.classfiles.count;

}


void GraphTask::end() {
    // ++_results.jarfiles.digest.count;
    _results.jarfiles.classfileCount += _jarFileRow->classfileCount;

    if (_isFileUnchanged) {
        // _results.jarfiles.classfiles.digest.count += _jarFileRow->classfileCount;
        // _results.jarfiles.classfiles.digest.unchangedCount += _jarFileRow->classfileCount;
        ++_results.jarfiles.taskResult.unchangedCount;
        return;
    }

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
    //     _results.log->writeln(fmt::format("Same public digestEntry of changed jar file:{}", filename));
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


kapa::infrastructure::db::Database &GraphTask::getDb() {
    return _db;
}

auto GraphTask::getClassfiles() -> std::shared_ptr<db::table::Classfiles> {
    const auto result = _db.getClassfiles();
    return result;
}

auto GraphTask::getFiles() -> std::shared_ptr<Files> {
    const auto result = _db.getFiles();
    return result;
}


auto GraphTask::findIncompleteClass(const JarEntryInfo &jarEntryInfo) -> ClassfileRow * {
    //TODO find incomplete class with the same FQN
    const auto result = nullptr;
    //TODO
    return result;
}

