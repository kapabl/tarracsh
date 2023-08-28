#include <filesystem>
#include <string>
#include "CallGraphAnalyzer.h"
#include "infrastructure/db/Database.h"
#include "domain/jar/tasks/GraphTask.h"
#include "domain/jar/Processor.h"
#include "domain/db/CallGraphDb.h"
#include "infrastructure/profiling/ScopedTimer.h"

using namespace std;


using kapa::infrastructure::profiler::ScopedTimer;
using kapa::tarracsh::domain::db::callgraph::CallGraphDb;
using namespace kapa::tarracsh::app::commands::callgraph;

using kapa::tarracsh::domain::jar::tasks::GraphTask;
using kapa::tarracsh::domain::jar::Processor;

CallGraphAnalyzer::CallGraphAnalyzer(Context &config, const std::shared_ptr<infrastructure::db::Database>& db)
    : Analyzer(config, db) {
}

CallGraphAnalyzer::CallGraphAnalyzer(Context &config)
    : Analyzer(config) {
}

void CallGraphAnalyzer::endAnalysis() {
    //TODO link nodes? and create report of linking errors (missing symbols) ?
    //
    if (!_options.callGraph.dryRun) {
        if (!_options.callGraph.server.isServerMode) {
            ScopedTimer timer(&_results.profileData->writeCallGraphDb);
            _db->stop();
            //_db->write();

        }
    }
}

void CallGraphAnalyzer::doClassfile(const std::string &filename) {
    //TODO
}

void CallGraphAnalyzer::processStandaloneClassfile(const std::string &filename) {
    _results.log->writeln(fmt::format("Standalone class file not supported yet: {}", filename));
}

void CallGraphAnalyzer::processJar(const std::string &filename) {
    _fileThreadPool.push_task([this, filename] {
        domain::Options jarOptions(_options);
        jarOptions.getBaseOptions().input = filename;

        ++_results.jarfiles.count;
        {
             GraphTask jarGraphTask(jarOptions, _results, reinterpret_cast<CallGraphDb &>(*_db));
             Processor jarProcessor(jarOptions, _results, jarGraphTask);
             jarProcessor.run();
        }

    });
}

CallGraphDb& CallGraphAnalyzer::getCallGraphDb() const {
    auto& result = reinterpret_cast<CallGraphDb&>(*_db);
    return result;
}