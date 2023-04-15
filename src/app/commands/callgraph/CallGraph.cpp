#include "CallGraph.h"

#include "CallGraphAnalyzer.h"
#include "app/App.h"
#include "app/Analyzer.h"
#include "app/commands/Query.h"
#include "app/server/digest/ServerCommand.h"
#include "infrastructure/profiling/ScopedTimer.h"


using namespace kapa::infrastructure::app::cli::command;
using namespace kapa::tarracsh::app::commands::callgraph;

using kapa::tarracsh::app::server::digest::ServerCommand;
using kapa::tarracsh::app::commands::Query;
using kapa::tarracsh::domain::ServerOptions;
using kapa::tarracsh::domain::ClientOptions;
using kapa::infrastructure::app::cli::ExitCode;
using kapa::infrastructure::profiler::ScopedTimer;

CallGraph::CallGraph(CLI::App* parent)
    : DbBasedCommand(parent, App::getGlobalOptions().digest) {
}

ExitCode CallGraph::processInput() {
    ExitCode result = 0;

    ScopedTimer::timeWithPrint(
        "digest-input",
        [this, &result]()-> void {
            if (_options.digest.isValidInput()) {
                if (isClientMode()) {
                    result = runAsClient() ? 0 : 1;
                }
                else {
                    result = runAsStandalone() ? 0 : 1;
                }

                if (result == 0 && _options.digest.isDiff) {
                    _results.report->print();
                }
            }
            else {
                result = 1;
            }
        });

    return result;
}

bool CallGraph::runAsServer() {
    const auto result = ServerCommand::run(App::getContext());
    return result;
}

bool CallGraph::runAsClient() {
    const auto result = ServerCommand::run(App::getContext());
    return result;
}

void CallGraph::addMainSubCommand() {
    _subCommand = _parent->add_subcommand("public-digest", "Public digest of jar files and classfiles");
    _subCommand->add_flag("--verbose, -v", _options.verbose, "Verbose output");
}

bool CallGraph::initDb() {

    ScopedTimer timer(&_results.profileData->initDb);
    _db = domain::db::callgraph::CallGraphDb::create({ _options.outputDir, _results.log.get()}, _options.callGraph.rebuild, false );
    const auto result = _db != nullptr;
    return result;

}

ExitCode CallGraph::run() {
    ExitCode result = 0;
    if (initDb()) {
        CallGraphAnalyzer analyzer(App::getApp(), _db);
        analyzer.runWithPrint();
    } else {
        _results.log->writeln("Error initializing call-graph Db", true);
        result = 1;
    }
    return result;
}

bool CallGraph::runAsStandalone() {
    const auto result = initDb();
    if (result) {
        CallGraphAnalyzer analyzer(App::getContext(), _db);
        analyzer.runWithPrint();
    }
    else {
        _results.log->writeln("Error initializing Db", true);
    }
    return result;
}

