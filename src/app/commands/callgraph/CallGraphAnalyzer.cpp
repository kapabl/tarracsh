#include <filesystem>
#include <string>
#include <vector>
#include "CallGraphAnalyzer.h"
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

CallGraphAnalyzer::CallGraphAnalyzer(Context &config, const std::shared_ptr<CallGraphDb> &db)
        : _callGraphDb(db),
          Analyzer(config, db) {

}

CallGraphAnalyzer::CallGraphAnalyzer(Context &config)
        : Analyzer(config) {
}

void CallGraphAnalyzer::linkClassRefs() {
    auto classRefs = _callGraphDb->getClassRefs();
    _results.log->writeln(fmt::format("Calculating {} class-ref edges...", classRefs->size()), true);

    auto classRefEdges = _callGraphDb->getClassRefEdges();
    classRefs->forEach([this, classRefEdges, classRefs](AutoIncrementedRow *pRow) -> void {

        auto classRefRow = reinterpret_cast<ClassRefRow &>(*pRow);
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
}


void CallGraphAnalyzer::linkMethodRefs() {
    linkMemberRefs<MethodRefEdges::Types>(
            *_callGraphDb->getMethodRefs(),
            *_callGraphDb->getMethodRefEdges(),
            *_callGraphDb->getMethods(),
            _methodsIndex
    );
}

void CallGraphAnalyzer::linkFieldRefs() {
    linkMemberRefs<FieldRefEdges::Types>(
            *_callGraphDb->getFieldRefs(),
            *_callGraphDb->getFieldRefEdges(),
            *_callGraphDb->getFields(),
            _fieldsIndex
    );
}

void CallGraphAnalyzer::createNamedIndexes() {
    createClassnameIndex();
    createMemberIndex<std::shared_ptr<Methods>, MethodRow>(
            _methodsIndex, _callGraphDb->getMethods(), "methods");
    createMemberIndex<std::shared_ptr<Fields>, FieldRow>(
            _fieldsIndex, _callGraphDb->getFields(), "fields");

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

    _results.log->writeln(fmt::format("Calculating {} method-ref edges...",
                                        _callGraphDb->getMethodRefs()->size()), true);
    linkMethodRefs();

    _results.log->writeln(fmt::format("Calculating {} field-ref edges...",
                                      _callGraphDb->getFieldRefs()->size()), true);
    linkFieldRefs();
}


void CallGraphAnalyzer::endAnalysis() {
    linkRefNodes();
    if (!_options.callGraph.dryRun) {
        if (!_options.callGraph.server.isServerMode) {
            ScopedTimer timer(&_results.profileData->writeCallGraphDb);
            _db->stop();
            //_db->write();

        }
    }
}

void CallGraphAnalyzer::doClassFile(const std::string &filename) {
    //TODO
}

void CallGraphAnalyzer::processStandaloneClassFile(const std::string &filename) {
    _results.log->writeln(fmt::format("Standalone class file not supported yet: {}", filename));
}

void CallGraphAnalyzer::processJar(const std::string &filename) {
    _fileThreadPool.push_task([this, filename] {
        domain::Options jarOptions(_options);
        jarOptions.getSubCommandOptions().input = filename;

        ++_results.jarfiles.count;
        {
            GraphTask jarGraphTask(jarOptions, _results, reinterpret_cast<CallGraphDb &>(*_db));
            Processor jarProcessor(jarOptions, _results, jarGraphTask);
            jarProcessor.run();
        }

    });
}


