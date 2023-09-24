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
using kapa::tarracsh::domain::db::table::ClassFileRow;
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

//void GraphTask::processNewClassfile(const JarEntryInfo &jarEntryInfo) {
//    const auto &jarEntry = jarEntryInfo.jarEntry;
//
//    MemoryReader reader(jarEntry);
//
//    ClassFileParser classFileParser(reader, jarEntry.getName(), _results.log);
//    if (classFileParser.parse()) {
//        auto row = getClassFileRow(addOrUpdateClassfile(jarEntry, classFileParser));
//        auto classFileProcessor = ClassFileProcessor(row, classFileParser, _db );
//        classFileProcessor.extractNodes();
//    } else {
//        _results.report->asFailedJarClass(jarEntryInfo.strongClassname);
//        ++_results.jarfiles.classfiles.errors;
//    }
//
//}

const ClassFileRow *GraphTask::getClassFileRow(uint64_t id) {
    const auto result = reinterpret_cast<const ClassFileRow *>(getClassFiles()->getRow(id));
    return result;
}

void GraphTask::processClassFile(const JarEntryInfo &jarEntryInfo, db::table::ClassFileRow *row) {
    if (row != nullptr) {
        _db.deleteClass(row);
    }
    const auto &jarEntry = jarEntryInfo.jarEntry;

    MemoryReader reader(jarEntry);
    ClassFileParser classFileParser(reader, jarEntry.getName(), _results.log);

    if (classFileParser.parse()) {
        auto upDatedRow = getClassFileRow(addOrUpdateClassfile(jarEntry, classFileParser));
        auto classFileProcessor = ClassFileProcessor(upDatedRow, classFileParser, _db );
        classFileProcessor.extractNodes();
    } else {
        _results.report->asFailedJarClass(jarEntryInfo.strongClassname);
        ++_results.jarfiles.classfiles.errors;
    }
}

bool GraphTask::start() {
    const auto &filename = _options.callGraph.input;
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

    auto classfileRow = static_cast<ClassFileRow *>(_db.getClassFiles()->findByKey(
            jarEntryInfo.strongClassname));

    const auto classExists = nullptr != classfileRow;

    if (!classExists) {
        ++_jarFileRow->classfileCount;
    }

    const auto isUnchanged = classExists && isClassfileUnchanged(jarEntry, classfileRow);
    if (isUnchanged) {
        _results.report->asUnchangedJarClass(jarEntryInfo.strongClassname);
    } else {
        processClassFile(jarEntryInfo, classfileRow);
    }

    ++_results.jarfiles.classfiles.count;

}


void GraphTask::end() {
    _results.jarfiles.classfileCount += _jarFileRow->classfileCount;

    if (_isFileUnchanged) {
        ++_results.jarfiles.taskResult.unchangedCount;
    }
    //TODO maybe update results/stats
    //check DigestTask::end() for inspiration
}


kapa::infrastructure::db::Database &GraphTask::getDb() {
    return _db;
}

auto GraphTask::getClassFiles() -> std::shared_ptr<db::table::ClassFiles> {
    const auto result = _db.getClassFiles();
    return result;
}

auto GraphTask::getFiles() -> std::shared_ptr<Files> {
    const auto result = _db.getFiles();
    return result;
}

