#include "GraphTask.h"

#include <ranges>
#include <utility>

// #include "domain/digest/ClassFileDigest.h"
#include "domain/jar/JarEntryInfo.h"
#include "domain/db/table/Classfiles.h"
#include "domain/classfile/constantpool/StructsCommon.h"
#include "domain/classfile/ClassFileParser.h"
#include "infrastructure/filesystem/Utils.h"
#include "domain/classfile/reader/MemoryReader.h"

using namespace kapa::infrastructure::filesystem;
// using namespace kapa::tarracsh::domain::digest;

using namespace kapa::tarracsh::domain;


using db::table::ClassfileRow;

using namespace classfile;
using namespace db;
using namespace db::callgraph;
using namespace db::digest;
using namespace jar::tasks;

using namespace std;

using kapa::tarracsh::domain::classfile::constantpool::u2;
using kapa::tarracsh::domain::classfile::constantpool::u1;
using kapa::tarracsh::domain::classfile::constantpool::u4;

GraphTask::GraphTask(
    Options options, Results &results,
    CallGraphDb &callGraphDb
    )
    : DbBasedTask(std::move(options), results), _db(callGraphDb) {
}

void GraphTask::recordMethod(const classfile::constantpool::MethodInfo &value) {
    //TODO write to method table
}

void GraphTask::recordClassMethods(ClassFileParser &classFileParser) {

    for (auto &method : classFileParser.getMethods()) {
        recordMethod(method);
    }
}

void GraphTask::recordMethodRef(const classfile::constantpool::ConstantPoolRecord &entry) {
    //TODO write to method-ref table
}

void GraphTask::recordClassRef(const classfile::constantpool::ConstantPoolRecord &entry) {
    //TODO write to class-ref table
}

void GraphTask::recordInterfaceMethodRef(const classfile::constantpool::ConstantPoolRecord &entry) {
    //TODO write to table
}

void GraphTask::recordFieldRef(const classfile::constantpool::ConstantPoolRecord &entry) {
    //TODO write to table
}

void GraphTask::recordRefs(ClassFileParser &classFileParser) {
    const auto &constantPool = classFileParser.getConstantPool();

    for (u2 index = 1u; index < constantPool.getPoolSize(); index = constantPool.getNextIndex(index)) {
        const auto &entry = constantPool.getEntry(index);
        if (entry.base.tag == ConstantPoolTag::JVM_CONSTANT_Methodref) {
            recordMethodRef(entry);
        } else if (entry.base.tag == ConstantPoolTag::JVM_CONSTANT_Class) {
            recordClassRef(entry);
        } else if (entry.base.tag == ConstantPoolTag::JVM_CONSTANT_InterfaceMethodref) {
            recordInterfaceMethodRef(entry);
        } else if (entry.base.tag == ConstantPoolTag::JVM_CONSTANT_Fieldref) {
            recordFieldRef(entry);
        }

    }
}

void GraphTask::processClassfile(const JarEntryInfo &jarEntryInfo,
                                 const ClassfileRow *row) {
    const auto &jarEntry = jarEntryInfo.jarEntry;

    Options options(_options);
    reader::MemoryReader reader(jarEntry);

    ClassFileParser classFileParser(reader, options, _results);
    if (classFileParser.parse()) {
        recordClassMethods(classFileParser);
        recordRefs(classFileParser);
        // const ClassFileDigest classFileDigest(classFileParser);
        // result = classFileDigest.digest();
        //
        // const bool classExists = row != nullptr;
        //
        // if (classExists) {
        //     const auto isSamePublicDigest = result.value() == row->digest;
        //     _results.report->asModifiedJarClass(jarEntryInfo.strongClassname, isSamePublicDigest);
        // }
        // else {
        //     _results.report->asNewJarClass(jarEntryInfo.strongClassname);
        // }
        //
        // ++_results.jarfiles.classfiles.parsedCount;
        //
        // if (!_options.digest.dryRun) {
        //     updateClassfileTableInMemory(jarEntry, result.value(), classFileParser);
        // }

    } else {
        _results.report->asFailedJarClass(jarEntryInfo.strongClassname);
        ++_results.jarfiles.classfiles.errors;
    }

}

const db::table::ClassfileRow *GraphTask::getClassfileRow(const JarEntry &jarEntry) const {
    // const auto key = _db.getClassfiles()->getStrongMethodName(
    //     *_jarFileRow,
    //     jarEntry.getClassname().c_str());
    // const tables::ClassfileRow *result = _db.getClassfiles()->findByKey(key);
    //
    // return result;
    //TODO
    return nullptr;
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

    const auto *classfileRow = static_cast<ClassfileRow *>(_db.getClassfiles()->findByKey(
        jarEntryInfo.strongClassname));

    const auto classExists = nullptr != classfileRow;

    if (!classExists) {
        ++_jarFileRow->classfileCount;
    }

    const auto isUnchanged = classExists && isClassfileUnchanged(jarEntry, classfileRow);
    if (isUnchanged) {
        _results.report->asUnchangedJarClass(jarEntryInfo.strongClassname);
        // std::unique_lock lock(taskMutex);
        // _digestMap[jarEntry.getClassname()] = classfileRow->digest;
    } else {
        processClassfile(jarEntryInfo, classfileRow);
        // const auto digest = digestEntry(jarEntryInfo, classfileRow);
        // if (digest.has_value()) {
        //     std::unique_lock lock(taskMutex);
        //     _digestMap[jarEntry.getClassname()] = digest.value();
        // }
    }

    ++_results.jarfiles.classfiles.count;
    //++_results.jarfiles.classfiles.digest.count;

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


kapa::infrastructure::db::Database &GraphTask::getDb() {
    return _db;
}

std::shared_ptr<table::Files> GraphTask::getFileTable() {
    const auto result = _db.getFiles();
    return result;
}
