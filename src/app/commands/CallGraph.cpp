#include "CallGraph.h"
#include "../Analyzer.h"


using namespace org::kapa::infrastructure::cli::command;
using namespace org::kapa::tarracsh::app::commands;

CallGraph::CallGraph(CLI::App* parent)
    : Command(parent), _results(App::getGlobalResults()), _options(App::getGlobalOptions()) {
}

ExitCode CallGraph::run() const {
    ExitCode result = 0;
    Analyzer analyzer(App::getApp());
    analyzer.run();
    return result;
}

void CallGraph::addCommand() {
    _subCommand = _parent->add_subcommand("call-graph", "Call Graph for jar or classfiles");
    _subCommand->add_option("--input,-i", _options.input, "Input: directory, jar file or class file")->required();
}
