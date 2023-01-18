#include "ParseCommand.h"
#include "../App.h"
#include "../Analyzer.h"

using namespace kapa::tarracsh;
using namespace app;
using namespace commands;


using namespace kapa::infrastructure::app::cli::command;

ParseCommand::ParseCommand(CLI::App* parent)
    : Command(parent), _results(App::getGlobalResults()), _options(App::getGlobalOptions()) {
}

ExitCode ParseCommand::run() {
    ExitCode result = 0;
    Analyzer analyzer(App::getApp());
    analyzer.run();
    return result;
}

void ParseCommand::addCommand() {
    _subCommand = _parent->add_subcommand("parse", "Parse class files, jar or directories");
    _subCommand->add_option("--input,-i", _options.input, "Input: directory, jar file or class file")->required();

    const auto printClassParse = _subCommand->add_flag("--print-class-parse", _options.printClassParse,
        "Verbose print of classfile parse result to stdout");
    const auto printCPool = _subCommand->add_flag("--print-cpool", _options.printConstantPool,
        "Printing const-pool to stdout. Similar to javap");

    _subCommand->add_flag("--descriptive-cpool", _options.descriptiveCPoolEntries,
        "Descriptive CPool Entries. e.g: 'UTF8 String' in place of 'utf8'")->default_val(true);

    const auto htmlNav = _subCommand->add_flag("--html-nav", _options.printCPoolHtmlNav,
        "Generate Constant pool navigable file");

    htmlNav->excludes(printCPool)
        ->excludes(printClassParse);
    
}
