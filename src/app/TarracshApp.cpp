#include "TarracshApp.h"
#include "../classfile/constpool/ConstantPoolPrinter.h"
#include "../classfile/ClassFileAnalyzer.h"
#include "../classfile/Analyzer.h"
#include "../jars/JarAnalyzerTask.h"
#include "../classfile/readers/FileReader.h"
#include "../tables/Query.h"

#ifdef _WIN32
#include <Windows.h>
#include <processenv.h>
#include <consoleapi.h>
#endif

using namespace org::kapa::tarracsh;
using namespace db;
using namespace std;


stats::Results TarracshApp::_results;
Options TarracshApp::_options;

int TarracshApp::run(int argc, char *argv[]) {

    TarracshApp app("", "Tarracsh");
    const auto result = app.start(argc, argv);

    if (_options.pause) {
        cin.get();
    }

    return result;
}

int TarracshApp::start(int argc, char *argv[]) {

    const auto exitCode = parseCli(argc, argv);
    if (exitCode != 0) {
        return exitCode;
    }

    init();

    if (_options.isPublicDigest) {
        if (_options.queryValue.empty()) {
            dir::Analyzer analyzer(_options, _results);
            analyzer.run();
        } else {
            query::QueryCommand::run(_options, _results);
        }
    } else if (_options.isCallGraph) {
        dir::Analyzer analyzer(_options, _results);
        analyzer.run();
    } else if (_options.isParse) {
        dir::Analyzer analyzer(_options, _results);
        analyzer.run();
    }

    _results.profileData->output(_options);

    return 0;

}


CLI::Option_group *TarracshApp::addParseOptions(CLI::Option *const inputOption) {
    const auto result = add_option_group("Parse classfile(s)", "");
    result->needs(inputOption);
    const auto parse = result->add_flag("--parse", _options.isParse, "Parse jar, dirs or classfiles")->
                               required();

    const auto printParse = result->add_flag("--print-class-parse", _options.printClassParse,
                                             "Verbose print of classfile parse result to stdout");
    printParse->needs(parse);

    const auto printCPool = result->add_flag("--print-cpool", _options.printConstantPool,
                                             "Printing const-pool to stdout. Similar to javap");

    printCPool->needs(parse);
    return result;
}

CLI::Option_group* TarracshApp::addCallGraphOptions(CLI::Option * const inputOption) {
    const auto result = add_option_group("Call/Class Graph", "Call Graph for jar or classfiles");
    result->needs(inputOption);
    result->add_flag("--call-graph", _options.isCallGraph, "Call Graph");
    return result;
}

//CLI::Option_group * TarracshApp::addPublicDigestOptions(CLI::Option *inputOption) {
CLI::App * TarracshApp::addPublicDigestOptions(CLI::Option *inputOption) {
    //const auto result = add_subcommand("Public Digest", "Public digest of jar files and classfiles");
    const auto result = add_subcommand("public-digest", "Public digest of jar files and classfiles");
    result->needs(inputOption);

    const auto digestFlag = result->add_flag("--public-digest", _options.isPublicDigest,
        "Public digest command")->required();
    const auto rebuild = result->add_flag("--rebuild", _options.rebuild, "Rebuild Digest Db");
    const auto dryRun = result->add_flag("--dry-run", _options.dryRun,
        "Check Against Digest Db, default behavior is check and add/update");

    rebuild->excludes(dryRun);

    const auto diff = result->add_flag("--diff", _options.doDiffReport, "Create Diff report");
    result->add_flag("--print-diff", _options.printDiffReport,
        "Print Diff report to stdout")->needs(diff);

    result->add_option("--query", _options.queryValue, "TODO Query Help - schema");
    return result;
}

void TarracshApp::setupCliOptions() {
    set_version_flag("-v,--version", "version " TARRACSH_VERSION);

    const auto inputOption = add_option("--input,-i", _options.input, "Input: directory, jar file or class file");

    const auto digestOptionGroup = addPublicDigestOptions(inputOption);
    const auto callGraphOptionGroup = addCallGraphOptions(inputOption);
    const auto classfileOptionGroup = addParseOptions(inputOption);

    classfileOptionGroup->excludes(digestOptionGroup);
    classfileOptionGroup->excludes(callGraphOptionGroup);
    digestOptionGroup->excludes(callGraphOptionGroup);

    add_flag("--pause", _options.pause, "Pause and wait for enter before finishing process. Useful when debugging");
    add_flag("--output-dir", _options.outputDir, "Output directory, default './output'");
    add_flag("--output-log-file", _options.logFile, "Log file, default './output/result.log");
    add_flag("--print-profiler", _options.printProfiler, "Print '[output]/profiler.txt");
    add_flag("--use-file-timestamp", _options.useFileTimestamp,
             "yes/no - Default 'yes'. Use file timestamp and size to check if a file was modified");

    // const auto workers = add_flag("--workers", _options.workers, "Number of workers, default 4");
    // workers->default_val(_options.workers);

}

int TarracshApp::parseCli(int argc, char **argv) {
    auto result = 0;
    setupCliOptions();
    try {
        parse(argc, argv);
    } catch (const CLI::ParseError &e) {
        result = exit(e);
    }

    return result;
}

#ifdef _WIN32
void TarracshApp::prepareForUTF8() {
    SetConsoleOutputCP(CP_UTF8);
    setvbuf(stdout, nullptr, _IOFBF, 1000);
}

void TarracshApp::prepareConsoleForVT100() {
    const auto stdOutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleMode(stdOutHandle, ENABLE_VIRTUAL_TERMINAL_PROCESSING | ENABLE_PROCESSED_OUTPUT);
}
#endif


void TarracshApp::init() const {

#ifdef _WIN32
    prepareConsoleForVT100();
    prepareForUTF8();
#endif

    filesystem::create_directories(_options.outputDir);
    Log::emptyLogFile(_options.logFile);
    ConstantPoolPrinter::init();

    _results.log.setFile(_options.logFile);
    _options.processInput();
}
