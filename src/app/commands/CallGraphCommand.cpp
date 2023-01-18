#include "CallGraphCommand.h"
#include "../App.h"
#include "../Analyzer.h"
#include "../../domain/stats/ScopedTimer.h"


using namespace kapa::infrastructure::app::cli::command;
using namespace kapa::tarracsh::app::commands;

CallGraphCommand::CallGraphCommand(CLI::App* parent)
    : Command(parent), _results(App::getGlobalResults()), _options(App::getGlobalOptions()) {
}

bool CallGraphCommand::initDb() {

    domain::stats::profiler::ScopedTimer timer(&_results.profileData->initDb);
    _db = domain::db::callgraph::CallGraphDb::create(_options.outputDir, *_results.log, _options.rebuild);
    const auto result = _db.get() != nullptr;
    return result;

}

kapa::infrastructure::app::cli::ExitCode CallGraphCommand::run() {
    ExitCode result = 0;
    if (initDb()) {
        Analyzer analyzer(App::getApp(), _db);
        analyzer.run();
    } else {
        _results.log->writeln("Error initializing call-graph Db", true);
        result = 1;
    }
    return result;
}

void CallGraphCommand::addCommand() {
    _subCommand = _parent->add_subcommand("call-graph", "Call Graph for jar or classfiles");
    _subCommand->add_option("--input,-i", _options.input, "Input: directory, jar file or class file")->required();
}
