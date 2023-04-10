#include "Parse.h"
#include "app/App.h"
#include "app/Analyzer.h"
#include "infrastructure/profiling/ScopedTimer.h"

using namespace kapa::tarracsh;
using namespace app;
using namespace commands;


using namespace kapa::infrastructure::app::cli::command;
using kapa::infrastructure::profiler::ScopedTimer;
using app::Analyzer;

Parse::Parse(CLI::App *parent)
    : Command(parent), _results(App::getGlobalResults()), _options(App::getGlobalOptions()) {
}

ExitCode Parse::run() {
    ExitCode result = 0;

    ScopedTimer::timeWithPrint(
        "parse-input",
        [this, &result]()-> void {
            if (_options.parse.isValidInput()) {
                Analyzer analyzer(App::getApp());
                analyzer.runWithPrint();
            } else {
                result = 1;
            }
        });

    return result;
}

void Parse::addCommand() {
    _subCommand = _parent->add_subcommand("parse", "Parse class files, jar or directories");
    _subCommand->add_option("--input,-i", _options.parse.input, "Input: directory, jar file or class file")->
                 required();

    const auto printClassParse = _subCommand->add_flag("--print-class-parse", _options.parse.print,
                                                       "Verbose print of classfile parse result to stdout");
    const auto printCPool = _subCommand->add_flag("--print-cpool", _options.parse.printConstantPool,
                                                  "Printing const-pool to stdout. Similar to javap");

    _subCommand->add_flag("--descriptive-cpool", _options.parse.descriptiveCPoolEntries,
                          "Descriptive CPool Entries. e.g: 'UTF8 String' in place of 'utf8'")->default_val(true);

    const auto htmlNav = _subCommand->add_flag("--html-nav", _options.parse.printCPoolHtmlNav,
                                               "Generate Constant pool navigable file");

    htmlNav->excludes(printCPool)
           ->excludes(printClassParse);

}
