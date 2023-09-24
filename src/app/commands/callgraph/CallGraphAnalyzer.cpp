#include <filesystem>
#include <string>
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

using kapa::tarracsh::domain::db::table::MethodRefRow;
using kapa::tarracsh::domain::db::table::MethodRefEdges;

using kapa::tarracsh::domain::db::table::FieldRow;
using kapa::tarracsh::domain::db::table::FieldRefRow;
using kapa::tarracsh::domain::db::table::FieldRefEdges;

CallGraphAnalyzer::CallGraphAnalyzer(Context &config, const std::shared_ptr<CallGraphDb> &db)
        : Analyzer(config, db) {
}

CallGraphAnalyzer::CallGraphAnalyzer(Context &config)
        : Analyzer(config) {
}

void CallGraphAnalyzer::linkClassRefs() {
    //TODO

    auto classRefs = _callGraphDb->getClassRefs();
    auto classRefEdges = _callGraphDb->getClassRefEdges();
    classRefs->forEach([this, classRefEdges, classRefs](AutoIncrementedRow *pRow) -> void {

        auto classRefRow = reinterpret_cast<ClassRefRow &>(*pRow);
        auto it = _classesByName.find(classRefRow.name);
        if (it != _classesByName.end()) {
            classRefRow.refCount += it->second.size();
            classRefs->update(&classRefRow);
        }

        for( auto* pClassFileRow: it->second) {
            auto classRefEdgeRow = reinterpret_cast<ClassRefEdgeRow &>(*classRefEdges->allocateRow());
            classRefEdgeRow.from.id = classRefRow.id;
            classRefEdgeRow.to.id = pClassFileRow->id;
            classRefEdges->add(&classRefEdgeRow);
        }
    });
}


void CallGraphAnalyzer::linkMethodRefs() {
    auto methodRefs = _callGraphDb->getMethodRefs();
//TODO
}

void CallGraphAnalyzer::linkFieldRefs() {
//TODO
}

void CallGraphAnalyzer::createNamedIndexes() {
    createClassnameIndex();
    createMemberNameIndex(_methodsIndex);
    createMemberNameIndex(_fieldsIndex);
}

void CallGraphAnalyzer::createClassnameIndex() {
    auto classFiles = _callGraphDb->getClassFiles();
    classFiles->forEach([this](AutoIncrementedRow *pRow) -> void {
        auto classFileRow = reinterpret_cast<ClassFileRow *>(pRow);

        if (_classesByName.contains(classFileRow->classname)) {
            _classesByName[classFileRow->classname].insert(classFileRow);
        } else {
            _classesByName[classFileRow->classname] = {classFileRow};
        }
    });
}

void CallGraphAnalyzer::createMemberNameIndex(MembersIndex &membersIndex) {
    auto fields = _callGraphDb->getFields();
    fields->forEach([this, &membersIndex](AutoIncrementedRow *pRow) -> void {
        auto fieldRow = reinterpret_cast<FieldRow *>(pRow);
        MemberInfo methodInfo;
        methodInfo.method.id = fieldRow->id;

        auto name = fieldRow->name;

        if (!membersIndex.contains(name)) {
            membersIndex[name] = MemberIndexEntry();
        }
        auto &entry = _methodsIndex[name];

        if (!entry.contains(fieldRow->ownerClass.id)) {
            entry[fieldRow->ownerClass.id] = {methodInfo};
        } else {
            entry[fieldRow->ownerClass.id].insert(methodInfo);
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

