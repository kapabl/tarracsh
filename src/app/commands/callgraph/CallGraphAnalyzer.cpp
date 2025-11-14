#include <filesystem>
#include <string>
#include <vector>
#include "CallGraphAnalyzer.h"
#include "domain/Utils.h"
#include "domain/classfile/reader/FileReader.h"
#include "domain/graph/ClassFileProcessor.h"
#include "CallGraphExporter.h"
#include "domain/jar/tasks/GraphTask.h"
#include "domain/jar/Processor.h"
#include "infrastructure/profiling/ScopedTimer.h"

using namespace std;
using namespace kapa::tarracsh::app::commands::callgraph;

using kapa::infrastructure::profiler::ScopedTimer;
using kapa::infrastructure::db::table::AutoIncrementedRow;
using kapa::tarracsh::domain::db::callgraph::CallGraphDb;

using kapa::tarracsh::domain::jar::tasks::GraphTask;
using kapa::tarracsh::domain::jar::Processor;

using kapa::tarracsh::domain::db::table::FileRow;
using kapa::tarracsh::domain::db::table::ClassRefRow;
using kapa::tarracsh::domain::db::table::ClassRefEdgeRow;
using kapa::tarracsh::domain::db::table::ClassRefEdges;

using kapa::tarracsh::domain::db::table::Methods;
using kapa::tarracsh::domain::db::table::MethodRefs;
using kapa::tarracsh::domain::db::table::MethodRefRow;
using kapa::tarracsh::domain::db::table::MethodRefEdges;
using kapa::tarracsh::domain::db::table::MethodRefEdgeRow;

using kapa::tarracsh::domain::db::table::Fields;
using kapa::tarracsh::domain::db::table::FieldRefs;
using kapa::tarracsh::domain::db::table::FieldRow;
using kapa::tarracsh::domain::db::table::FieldRefRow;
using kapa::tarracsh::domain::db::table::FieldRefEdges;
using kapa::tarracsh::domain::db::table::FieldRefEdgeRow;
using kapa::tarracsh::domain::graph::ClassFileProcessor;

CallGraphAnalyzer::CallGraphAnalyzer(Context &config, const std::shared_ptr<CallGraphDb> &db)
        : _callGraphDb(db),
          Analyzer(config, db) {

}

CallGraphAnalyzer::CallGraphAnalyzer(Context &config)
        : Analyzer(config) {
}

void CallGraphAnalyzer::linkClassRefs() {
    auto classRefs = _callGraphDb->getClassRefs();
    _results.log->writeln(fmt::format("{} class-refs, calculating edges...", classRefs->size()), true);

    auto classRefEdges = _callGraphDb->getClassRefEdges();
    classRefs->forEach([this, classRefEdges, classRefs](AutoIncrementedRow *pRow) -> void {
        auto &classRefRow = reinterpret_cast<ClassRefRow &>(*pRow);
        auto it = _classesByName.find(classRefRow.name);
        if (it != _classesByName.end()) {
            classRefs->updateInPlace([&classRefRow, &it]() -> ClassRefRow * {
                classRefRow.edgeCount += it->second.size();
                return &classRefRow;
            });


            for (auto *pClassFileRow: it->second) {
                auto& classRefEdgeRow = reinterpret_cast<ClassRefEdgeRow &>(*classRefEdges->allocateRow());
                new(&classRefEdgeRow) ClassRefEdgeRow();
                classRefEdgeRow.from.id = classRefRow.id;
                classRefEdgeRow.to.id = pClassFileRow->id;
                classRefEdges->add(&classRefEdgeRow);

                _callGraphDb->getClassFiles()->updateInPlace([pClassFileRow]() -> ClassFileRow * {
                    ++pClassFileRow->refCount;
                    return pClassFileRow;
                });
            }
        }
    });
    _results.log->writeln(fmt::format("Class-ref edges: {}", classRefEdges->size()), true);
}


void CallGraphAnalyzer::linkMethodRefs() {
    _results.log->writeln(fmt::format("{} method-refs, calculating edges...",
                                      _callGraphDb->getMethodRefs()->size()), true);
    linkMemberRefs<MethodRefEdges::Types>(
            *_callGraphDb->getMethodRefs(),
            *_callGraphDb->getMethodRefEdges(),
            *_callGraphDb->getMethods(),
            _methodsIndex
    );
    _results.log->writeln(fmt::format("Method-ref edges: {}", _callGraphDb->getMethodRefEdges()->size()), true);
}

void CallGraphAnalyzer::linkFieldRefs() {
    _results.log->writeln(fmt::format("{} field-refs, calculating edges...",
                                      _callGraphDb->getFieldRefs()->size()), true);
    linkMemberRefs<FieldRefEdges::Types>(
            *_callGraphDb->getFieldRefs(),
            *_callGraphDb->getFieldRefEdges(),
            *_callGraphDb->getFields(),
            _fieldsIndex
    );
    _results.log->writeln(fmt::format("Field-ref edges: {}", _callGraphDb->getFieldRefEdges()->size()), true);

}

void CallGraphAnalyzer::createNamedIndexes() {
    createClassnameIndex();
    createMemberIndex<MethodRefEdges::Types>(
            _methodsIndex, *_callGraphDb->getMethods(), "methods");
    createMemberIndex<FieldRefEdges::Types>(
            _fieldsIndex, *_callGraphDb->getFields(), "fields");

}

void CallGraphAnalyzer::createClassnameIndex() {
    auto classFiles = _callGraphDb->getClassFiles();
    _results.log->writeln(fmt::format("Indexing {} classes", classFiles->size()));
    classFiles->forEach([this](AutoIncrementedRow *pRow) -> void {
        auto classFileRow = reinterpret_cast<ClassFileRow *>(pRow);

        if (_classesByName.contains(classFileRow->classname)) {
            _classesByName[classFileRow->classname].insert(classFileRow);
        } else {
            _classesByName[classFileRow->classname] = {classFileRow};
        }
    });
}

void CallGraphAnalyzer::linkRefNodes() {
    createNamedIndexes();
    linkClassRefs();
    linkMethodRefs();
    linkFieldRefs();
}


void CallGraphAnalyzer::endAnalysis() {
    linkRefNodes();
    const bool wantsDot = _options.callGraph.exportDot;
    const bool wantsGml = _options.callGraph.exportGml;
    if (_callGraphDb != nullptr && (wantsDot || wantsGml)) {
        CallGraphExporter exporter(_callGraphDb, _results, _options.outputDir, wantsDot, wantsGml);
        exporter.run();
    }
    if (!_options.callGraph.dryRun) {
        if (!_options.callGraph.server.isServerMode) {
            ScopedTimer timer(&_results.profileData->writeCallGraphDb);
            _db->stop();
            //_db->write();

        }
    }
}

void CallGraphAnalyzer::doClassFile(const std::string &filename) {
    if (_callGraphDb == nullptr) {
        _results.log->writeln("Call graph database is not initialized", true);
        return;
    }

    StandaloneClassFileInfo fileInfo(filename);
    auto filesTable = _callGraphDb->getFiles();
    auto classFiles = _callGraphDb->getClassFiles();

    auto *existingFileRow = reinterpret_cast<FileRow *>(filesTable->findByKey(fileInfo.filename));
    const bool fileExists = existingFileRow != nullptr;
    const bool isUnchanged = _options.useFileTimestamp &&
                             fileExists &&
                             existingFileRow->fileSize == fileInfo.size &&
                             existingFileRow->lastWriteTime == fileInfo.timestamp;

    ++_results.standaloneClassfiles.taskResult.count;

    if (isUnchanged) {
        _results.report->asUnchangedClassFile(filename);
        return;
    }

    kapa::tarracsh::domain::classfile::reader::FileReader reader(filename);
    ClassFileParser parser(reader, filename, _results.log);

    if (!parser.parse()) {
        _results.report->asFailedClassFile(filename);
        return;
    }

    ++_results.standaloneClassfiles.parsedCount;

    auto &fileRow = reinterpret_cast<FileRow &>(*filesTable->allocateRow());
    new(&fileRow) FileRow();
    fileRow.filename = _callGraphDb->getPoolString(fileInfo.filename);
    fileRow.lastWriteTime = fileInfo.timestamp;
    fileRow.fileSize = fileInfo.size;
    fileRow.classfileCount = 1;
    filesTable->addOrUpdate(&fileRow);

    auto *storedFileRow = reinterpret_cast<FileRow *>(filesTable->findByKey(fileInfo.filename));
    if (storedFileRow == nullptr) {
        _results.log->writeln(fmt::format("Failed to upsert file row for {}", filename), true);
        return;
    }

    const auto classname = parser.getMainClassname();
    const auto strongClassname =
            kapa::tarracsh::domain::utils::getStrongClassname(filename.c_str(), classname.c_str());

    auto *existingClassRow = reinterpret_cast<ClassFileRow *>(classFiles->findByKey(strongClassname));
    if (existingClassRow != nullptr) {
        _callGraphDb->deleteClass(existingClassRow);
        classFiles->archiveRow(*existingClassRow, strongClassname);
        classFiles->deleteRow(existingClassRow->id);
    }

    auto &classRow = reinterpret_cast<ClassFileRow &>(*classFiles->allocateRow());
    new(&classRow) ClassFileRow(*storedFileRow);
    classRow.classname = _callGraphDb->getPoolString(classname);
    classRow.lastWriteTime = fileInfo.timestamp;
    classRow.size = fileInfo.size;
    classRow.crc = 0;
    classFiles->add(&classRow);

    ClassFileProcessor processor(&classRow, parser, *_callGraphDb);
    processor.extractNodes();

    if (existingClassRow != nullptr) {
        _results.report->asModifiedClassFile(false, strongClassname);
    } else {
        _results.report->asNewClassFile(strongClassname);
    }
}

void CallGraphAnalyzer::processStandaloneClassFile(const std::string &filename) {
    Analyzer::processStandaloneClassFile(filename);
}

void CallGraphAnalyzer::processJar(const std::string &filename) {
    _fileThreadPool.detach_task([this, filename] {
        domain::Options jarOptions(_options);
        jarOptions.isCallGraph = true;
        auto &jarInputs = jarOptions.callGraph;
        jarInputs.input = filename;
        jarInputs.isJar = true;
        (void)jarInputs.processInput();

        ++_results.jarfiles.count;
        {
            GraphTask jarGraphTask(jarOptions, _results, reinterpret_cast<CallGraphDb &>(*_db));
            Processor jarProcessor(jarOptions, _results, jarGraphTask);
            jarProcessor.run();
        }

    });
}
