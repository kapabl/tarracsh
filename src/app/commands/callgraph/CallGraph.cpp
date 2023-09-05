#include "CallGraph.h"

#include "CallGraphAnalyzer.h"
#include "app/App.h"
#include "app/Analyzer.h"
#include "app/commands/Query.h"
#include "domain/db/CallGraphDb.h"
#include "infrastructure/profiling/ScopedTimer.h"
#include "infrastructure/db/columns/Columns.h"
#include "domain/classfile/AccessModifiers.h"


using namespace kapa::infrastructure::app::cli::command;
using namespace kapa::tarracsh::app::commands::callgraph;

using kapa::tarracsh::app::commands::Query;
using kapa::tarracsh::domain::ServerOptions;
using kapa::tarracsh::domain::ClientOptions;
using kapa::infrastructure::app::cli::ExitCode;
using kapa::infrastructure::profiler::ScopedTimer;
using kapa::infrastructure::db::table::column::DisplayAs;
using kapa::infrastructure::db::table::column::Properties;
using kapa::tarracsh::domain::classfile::constantpool::u2;


CallGraph::CallGraph(CLI::App *parent)
        : DbBasedCommand(parent, App::getGlobalOptions().callGraph) {
}

ExitCode CallGraph::processInput() {
    ExitCode result = 0;

    ScopedTimer::timeWithPrint(
            "callgraph-input",
            [this, &result]() -> void {
                if (_options.callGraph.isValidInput()) {
                    if (isClientMode()) {
                        result = runAsClient() ? 0 : 1;
                    } else {
                        result = runAsStandalone() ? 0 : 1;
                    }

                    if (result == 0 /*&& _options.digest.isDiff*/) {
                        _results.report->print();
                    }
                } else {
                    result = 1;
                }
            });

    return result;
}

bool CallGraph::runAsServer() {
    // const auto result = ServerCommand::run(App::getContext());
    const auto result = false;
    //TODO
    return result;
}

bool CallGraph::runAsClient() {
    // const auto result = ServerCommand::run(App::getContext());
    const auto result = false;
    //TODO
    return result;
}

void CallGraph::addMainSubCommand() {
    _subCommand = _parent->add_subcommand("call-graph", "Creates call-graph");
    _subCommand->add_flag("--verbose, -v", _options.verbose, "Verbose output");
}

bool CallGraph::initDb() {

    registerColumns();

    ScopedTimer timer(&_results.profileData->initDb);
    _db = domain::db::callgraph::CallGraphDb::create({_options.outputDir, _results.log.get()},
                                                     _options.callGraph.rebuild, false);
    const auto result = _db != nullptr;
    return result;

}

ExitCode CallGraph::run() {
    ExitCode result = processInput();
    //TODO implement server-mode and client-mode
    return result;
}

bool CallGraph::runAsStandalone() {
    const auto result = initDb();
    if (result) {
        CallGraphAnalyzer analyzer(App::getContext(), _db);
        analyzer.runWithPrint();
    } else {
        _results.log->writeln("Error initializing Db", true);
    }
    return result;
}

auto CallGraph::registerColumns() -> bool {
    Properties::registerColumn(
            DisplayAs::AsAccessFlags,
            [](char *pValue, const Properties &properties, kapa::infrastructure::db::Database &db,
               bool displayRaw) -> std::string {
                auto value = static_cast<u2>(*pValue);
                auto result = kapa::tarracsh::domain::classfile::accessmodifier::toString(value);
                return result;
            });
    return true;
}

//static bool registerColumnsResult = registerColumns();


