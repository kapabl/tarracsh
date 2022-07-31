#include "Tarracsh.h"
#include "TarracshApp.h"

#include "ClassFileAnalyzer.h"
#include "CLI11.hpp"
#include "DirAnalyzer.h"
#include "JarAnalyzer.h"

#ifdef _WIN32
#include <Windows.h>
#include <processenv.h>
#include <consoleapi.h>
#endif

using namespace org::kapa::tarracsh;
using namespace std;

int TarracshApp::run(int argc, char *argv[]) {

    TarracshApp app("", "Tarracsh");
    const auto result = app.start(argc, argv);
    return result;
}

int TarracshApp::start(int argc, char *argv[]) {


    const auto exitCode = parseCli(argc, argv);
    if (exitCode != 0) return exitCode;

    init();

    Results results;
    results.resultLog.setFile(_options.logFile);

    if (!_options.classfileOption->empty()) {
        ClassFileAnalyzer classFileAnalyzer(_options, results);
        classFileAnalyzer.run();
    } else if (!_options.jarOption->empty()) {
        jar::JarAnalyzer jarAnalyzer(_options);
        jarAnalyzer.run();
    } else if (!_options.dirOption->empty()) {
        dir::DirAnalyzer dirAnalyzer(_options);
        dirAnalyzer.run();
    } else {
        std::cout << help();
    }

    return 0;

}

void TarracshApp::setupCliOptions() {
    set_version_flag("-v,--version", "version " TARRACSH_VERSION);

    _options.classfileOption = add_option("--classfile", _options.classFile, " Input class file");
    _options.dirOption = add_option("--dir", _options.directory, " directory");
    _options.jarOption = add_option("--jar", _options.jarFile, "Input jar file");

    _options.classfileOption->excludes(_options.jarOption);
    _options.classfileOption->excludes(_options.dirOption);

    _options.jarOption->excludes(_options.dirOption);
    _options.jarOption->excludes(_options.classfileOption);

    _options.dirOption->excludes(_options.jarOption);
    _options.dirOption->excludes(_options.classfileOption);

    add_flag("-c,--classpath", _options.classPath, "Class paths to look into.");
    add_flag("--generate-public-sha", _options.generatePublicSha, "yes/no");
    add_flag("--output-class-parse", _options.outputClassParse, "yes/no");
    add_flag("--output-dir", _options.outputDir, "Output directory, default './output'");
    add_flag("--output-log-file", _options.logFile, "Log file, default './output/result.log");
    const auto workers = add_flag("--workers", _options.workers, "Number of workers, default 4");
    workers->default_val(_options.workers);
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
void TarracshApp::prepareConsoleForVT100() {
    const auto stdOutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleMode(stdOutHandle, ENABLE_VIRTUAL_TERMINAL_PROCESSING | ENABLE_PROCESSED_OUTPUT);
}
#endif



void TarracshApp::init() const {

#ifdef _WIN32
    prepareConsoleForVT100();
#endif

    filesystem::create_directories(_options.outputDir);
    Log::emptyLogFile( _options.logFile);
}
