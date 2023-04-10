#include <filesystem>
#include <string>
#include "CallGraphAnalyzer.h"
#include "infrastructure/filesystem/Utils.h"
#include "infrastructure/db/Database.h"

#include "app/classfile/constantpool/printer/nav/HtmlGen.h"
#include "domain/jar/tasks/ParserTask.h"
#include "domain/jar/tasks/DigestTask.h"
#include "domain/stats/Results.h"
#include "domain/db/CallGraphDb.h"
#include "infrastructure/profiling/ScopedTimer.h"

using namespace std;


using kapa::infrastructure::profiler::ScopedTimer;
using kapa::tarracsh::domain::db::callgraph::CallGraphDb;


using namespace kapa::tarracsh::app::commands::callgraph;

CallGraphAnalyzer::CallGraphAnalyzer(Context &config, const std::shared_ptr<infrastructure::db::Database> db)
    : Analyzer(config, db) {
}

CallGraphAnalyzer::CallGraphAnalyzer(Context &config)
    : Analyzer(config) {
}

void CallGraphAnalyzer::endAnalysis() {
    if (!_options.callGraph.dryRun) {
        if (!_options.callGraph.server.isServerMode) {
            ScopedTimer timer(&_results.profileData->writeCallGraphDb);
            _db->stop();
            //_db->write();

        }
    }
}

void CallGraphAnalyzer::analyzeStandaloneClassfile(const std::string &filename) {

    //TODO
    //callGraph(filename);
}


void CallGraphAnalyzer::processJar(const std::string &filename) {
    _fileThreadPool.push_task([this, filename] {
        domain::Options jarOptions(_options);
        jarOptions.getBaseOptions().input = filename;

        ++_results.jarfiles.count;
        {
            //TODO
            // GraphTask jarGraphTask(jarOptions, _results, _callGraphDb);
            // Processor jarProcessor(jarOptions, _results, jarGraphTask);
            // jarProcessor.run();
        }

    });
}

CallGraphDb& CallGraphAnalyzer::getCallGraphDb() const {
    auto& result = reinterpret_cast<CallGraphDb&>(*_db);
    return result;
}