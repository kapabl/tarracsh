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
    return result;
}

int TarracshApp::start(int argc, char *argv[]) {

    const auto exitCode = parseCli(argc, argv);
    if (exitCode != 0) {
        return exitCode;
    }

    init();

    _results.log.setFile(_options.logFile);

    if (_options.isPublicDigest) {
        if (_options.queryOption->empty()) {
            dir::Analyzer analyzer(_options);
            analyzer.run();
        } else {
            query::QueryCommand::run(_options, _results);
        }
    } else if (_options.isCallGraph) {
        //TODO check if we need more
        dir::Analyzer analyzer(_options);
        analyzer.run();
    } else if (_options.isParse) {
        dir::Analyzer analyzer(_options);
        analyzer.run();
    }

    return 0;

}


void TarracshApp::setupCliOptions() {
    set_version_flag("-v,--version", "version " TARRACSH_VERSION);

    const auto digestOptionGroup = add_option_group("Public Digest", "Public digest of jar files and classfiles");
    digestOptionGroup->add_flag("--public-digest", _options.isPublicDigest, "Public digest command");
    digestOptionGroup->add_flag("--rebuild", _options.rebuild, "Rebuild Digest Db");
    digestOptionGroup->add_flag("--check-only", _options.checkOnly,
                                "Check Against Digest Db, default behavior is check and add/update");
    digestOptionGroup->add_flag("--diff", _options.doDiffReport, "Create Diff report");
    digestOptionGroup->add_flag("--print-diff", _options.printDiffReport, "Print Diff report to stdout");
    _options.queryOption = digestOptionGroup->add_option("--query", _options.queryValue, "queryValue");

    const auto callGraphOptionGroup = add_option_group("Call/Class Graph", "Call Graph for jar or classfiles");
    callGraphOptionGroup->add_flag("--call-graph", _options.isCallGraph, "Call Graph");
    // callGraphOptionGroup->add_option(useFileTimeStamp);
    //TODO more options

    const auto classfileOptionGroup = add_option_group("Parse classfile(s)", "");
    classfileOptionGroup->add_flag("--parse", _options.isParse, "Parse jar, dirs or classfiles");
    classfileOptionGroup->add_flag("--print-class-parse", _options.printClassParse,
                                   "Verbose print of classfile parse result to stdout");
    classfileOptionGroup->add_flag("--print-cpool", _options.printConstantPool,
                                   "Printing const-pool to stdout. Similar to javap");

    classfileOptionGroup->excludes(digestOptionGroup);
    classfileOptionGroup->excludes(callGraphOptionGroup);

    digestOptionGroup->excludes(classfileOptionGroup);
    digestOptionGroup->excludes(callGraphOptionGroup);

    callGraphOptionGroup->excludes(classfileOptionGroup);
    callGraphOptionGroup->excludes(digestOptionGroup);

    _options.classfileOption = add_option("--classfile", _options.classFilePath, " Input class file");
    _options.dirOption = add_option("--dir", _options.directory, "Input directory");
    _options.jarOption = add_option("--jar", _options.jarFile, "Input jar file");

    _options.classfileOption->excludes(_options.jarOption);
    _options.classfileOption->excludes(_options.dirOption);
    _options.classfileOption->excludes(_options.queryOption);

    _options.jarOption->excludes(_options.dirOption);
    _options.jarOption->excludes(_options.classfileOption);
    _options.jarOption->excludes(_options.queryOption);

    _options.dirOption->excludes(_options.jarOption);
    _options.dirOption->excludes(_options.classfileOption);
    _options.dirOption->excludes(_options.queryOption);

    _options.queryOption->excludes(_options.jarOption);
    _options.queryOption->excludes(_options.classfileOption);
    _options.queryOption->excludes(_options.dirOption);

    add_flag("--output-dir", _options.outputDir, "Output directory, default './output'");
    add_flag("--output-log-file", _options.logFile, "Log file, default './output/result.log");
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
}
