#include "TarracshApp.h"
#include "../nav/HtmlGen.h"
#include "Analyzer.h"
#include "../jars/JarParserTask.h"
#include "../tables/Query.h"
#include "../server/PublicDigestServiceImpl.h"

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

    auto exitCode = parseCli(argc, argv);
    if (exitCode != 0) {
        return exitCode;
    }

    init();

    if (_options.isPublicDigest) {
        if (_options.server) {
            server::PublicDigestServiceImpl::start(_options);
        } else if (!_options.queryValue.empty()) {
            query::QueryCommand::run(_options, _results);
        } else {
            if (isValidInput()) {
                dir::Analyzer analyzer(_options, _results);
                analyzer.run();
            } else {
                exitCode = 1;
            }
        }

    } else if (_options.isCallGraph) {
        dir::Analyzer analyzer(_options, _results);
        analyzer.run();
    } else if (_options.isParse) {
        dir::Analyzer analyzer(_options, _results);
        analyzer.run();
    }

    _results.profileData->output(_options);

    return exitCode;

}


CLI::App *TarracshApp::addParseSubCommand() {
    const auto result = add_subcommand("parse", "Parse class files, jar or directories");
    result->add_option("--input,-i", _options.input, "Input: directory, jar file or class file")->required();

    const auto printClassParse = result->add_flag("--print-class-parse", _options.printClassParse,
                                                  "Verbose print of classfile parse result to stdout");
    const auto printCPool = result->add_flag("--print-cpool", _options.printConstantPool,
                                             "Printing const-pool to stdout. Similar to javap");

    result->add_flag("--descriptive-cpool", _options.descriptiveCPoolEntries,
                     "Descriptive CPool Entries. e.g: 'UTF8 String' in place of 'utf8'");

    const auto htmlNav = result->add_flag("--html-nav", _options.printCPoolHtmlNav,
                                          "Generate Constant pool navigable file");

    htmlNav->excludes(printCPool)
           ->excludes(printClassParse);

    return result;
}

CLI::App *TarracshApp::addCallGraphSubCommand() {
    const auto result = add_subcommand("call-graph", "Call Graph for jar or classfiles");
    result->add_option("--input,-i", _options.input, "Input: directory, jar file or class file")->required();
    return result;
}

CLI::App *TarracshApp::addPublicDigestSubCommand() {

    const auto result = add_subcommand("public-digest", "Public digest of jar files and classfiles");
    const auto input = result->add_option("--input,-i", _options.input, "Input: directory, jar file or class file");

    const auto rebuild = result->add_flag("--rebuild", _options.rebuild, "Rebuild Digest Db")
                               ->needs(input);

    const auto dryRun = result->add_flag("--dry-run", _options.dryRun,
                                         "Check Against Digest Db, default behavior is check and add/update")
                              ->needs(input);

    rebuild->excludes(dryRun);

    const auto diff = result->add_flag("--diff", _options.doDiffReport, "Create Diff report")
                            ->needs(input);
    result->add_flag("--print-diff", _options.printDiffReport,
                     "Print Diff report to stdout")->needs(diff)
          ->needs(input);

    result->add_flag("--verbose, -v", _options.verbose, "Verbose output");

    const auto query = result->add_option("--query", _options.queryValue, "TODO Query Help - schema")
                             ->needs(input);

    const auto server = result->add_flag("--server", _options.server, "Server mode");
    result->add_option("--port", _options.port, "port")->needs(server)->default_val(_options.port);

    server->excludes(input);

    return result;
}

void TarracshApp::setupCliOptions() {
    set_version_flag("--version", "version " TARRACSH_VERSION);
    set_help_all_flag("--help-all");

    _digest = addPublicDigestSubCommand();
    _callGraph = addCallGraphSubCommand();
    _parse = addParseSubCommand();

    _parse->excludes(_digest);
    _parse->excludes(_callGraph);
    _digest->excludes(_callGraph);

    add_flag("--pause", _options.pause, "Pause and wait for enter before finishing process. Useful when debugging");
    add_flag("--output-dir", _options.outputDir, "Output directory, default '%HOME%/tarracsh/output'");
    add_flag("--output-log-file", _options.logFile, "Log file, default '[output]/result.log");
    add_flag("--print-profiler", _options.printProfiler, "Print '[output]/profiler.txt");
    add_flag("--use-file-timestamp", _options.useFileTimestamp,
             "yes/no - Default 'yes'. Use file timestamp and size to check if a file was modified");

    // const auto workers = add_flag("--workers", _options.workers, "Number of workers, default 4");
    // workers->default_val(_options.workers);

}

bool TarracshApp::isValidInput() const {
    auto result = true;
    if (!_options.processInput()) {
        cout << std::format("Input should be a directory, jar or class file. Invalid input:{}",
                            _options.input) << endl;
        result = false;

    }
    return result;
}

int TarracshApp::parseCli(int argc, char **argv) {
    auto result = 0;
    setupCliOptions();
    try {
        parse(argc, argv);

        if (got_subcommand(_digest)) {
            _options.isPublicDigest = true;
        } else if (got_subcommand(_callGraph)) {
            _options.isCallGraph = true;
        } else if (got_subcommand(_parse)) {
            _options.isParse = true;
        } else {
            cout << std::format("Invalid sub-command") << endl;
            result = 1;
        }

        if (_options.outputDir.empty()) {
            _options.outputDir = (fsUtils::getUserHomeDir() / "tarracsh" / "output").string();
        }

        if (_options.logFile.empty()) {
            const auto logFile = std::filesystem::path(_options.outputDir) / "output.log";
            _options.logFile = logFile.string();
        }

        if (!_options.input.empty() && !isValidInput()) {
            result = 1;
        }

    } catch (const CLI::ParseError &e) {
        result = exit(e);
    }

    return result;
}

#ifdef _WIN32
void TarracshApp::prepareConsoleForUTF8() {
    SetConsoleOutputCP(CP_UTF8);
    setvbuf(stdout, nullptr, _IOFBF, 1000);
}

void TarracshApp::prepareConsoleForVT100() {
    const auto stdOutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleMode(stdOutHandle, ENABLE_VIRTUAL_TERMINAL_PROCESSING | ENABLE_PROCESSED_OUTPUT);
}
#endif


bool TarracshApp::isCPoolPrinterNeeded() {
    const auto result = _options.printConstantPool || _options.printCPoolHtmlNav;
    return result;
}

void TarracshApp::init() const {

    auto result = true;

#ifdef _WIN32
    prepareConsoleForVT100();
    prepareConsoleForUTF8();
#endif

    fsUtils::ensureDir(_options.outputDir);
    _results.log.init(_options.logFile);

    if (isCPoolPrinterNeeded()) {
        ConstantPoolPrinter::init();
        if (_options.printCPoolHtmlNav) {
            nav::HtmlGen::init();
        }
    }

}
