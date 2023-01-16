#include "App.h"
#include "../infrastructure/filesystem/Utils.h"
#include "Tarracsh.h"
#include "nav/HtmlGen.h"

#ifdef _WIN32
#include <Windows.h>
#include <processenv.h>
#include <consoleapi.h>
#endif

using namespace kapa;
using namespace infrastructure::app::cli;
using namespace infrastructure::log;
using namespace infrastructure::filesystem::utils;
using namespace tarracsh;
using namespace app;
using namespace std;


std::unique_ptr<App> App::_app;



App::App(const std::string &description, const std::string &name, std::shared_ptr<Log> log)
    : CliApp(description, name), _results(_options), _log(log) {

    _results.log = log;
}

ExitCode App::run(int argc, char *argv[]) {

    const auto log = make_shared<Log>();
    _app.reset(new App("", "Tarracsh", log));

    const auto result = _app->start(argc, argv);

    if (_app->_options.pause) {
        cin.get();
    }

    return result;
}

ExitCode App::start(int argc, char *argv[]) {

    auto exitCode = parseCli(argc, argv);
    if (exitCode != 0) {
        return exitCode;
    }

    init();

    if (_options.isPublicDigest) {
        exitCode = _digestCommand->run();
    } else if (_options.isCallGraph) {
        exitCode = _callGraphCommand->run();
    } else if (_options.isParse) {
        exitCode = _parseCommand->run();
    }

    _results.profileData->output(_options);

    return exitCode;

}


void App::setupCliOptions() {
    set_version_flag("--version", "version " TARRACSH_VERSION);
    set_help_all_flag("--help-all");

    _digestCommand = make_unique<commands::PublicDigest>(this);
    _digestCommand->addCommand();

    _parseCommand = make_unique<commands::Parse>(this);
    _parseCommand->addCommand();

    _callGraphCommand = make_unique<commands::CallGraph>(this);
    _callGraphCommand->addCommand();

    _parseCommand->getSubCommand()->excludes(_digestCommand->getSubCommand());
    _parseCommand->getSubCommand()->excludes(_callGraphCommand->getSubCommand());
    _digestCommand->getSubCommand()->excludes(_callGraphCommand->getSubCommand());

    add_flag("--pause", _options.pause, "Pause and wait for enter before finishing process. Useful when debugging");
    add_flag("--output-dir", _options.outputDir, "Output directory, default '%HOME%/tarracsh/output'");
    add_flag("--output-log-file", _options.logFile, "Log file, default '[output]/result.log");
    add_flag("--print-profiler", _options.printProfiler, "Print '[output]/profiler.txt");
    add_flag("--use-file-timestamp", _options.useFileTimestamp,
             "yes/no - Default 'yes'. Use file timestamp and size to check if a file was modified");

    // const auto workers = add_flag("--workers", _options.workers, "Number of workers, default 4");
    // workers->default_val(_options.workers);

}

bool App::isValidInput(Options &options) {
    const auto result = options.processInput();
    if (!result) {
        cout << std::format("Input should be a directory, jar or class file. Invalid input:{}",
                            options.input) << endl;
    }
    return result;
}

int App::parseCli(int argc, char **argv) {
    auto result = 0;
    setupCliOptions();
    try {
        parse(argc, argv);

        if (_digestCommand->isSelected()) {
            _options.isPublicDigest = true;
        } else if (_callGraphCommand->isSelected()) {
            _options.isCallGraph = true;
        } else if (_parseCommand->isSelected()) {
            _options.isParse = true;
        } else {
            cout << std::format("Invalid sub-command") << endl;
            result = 1;
        }

        if (_options.outputDir.empty()) {
            _options.outputDir = (getUserHomeDir() / "tarracsh" / "output").string();
        }

        if (_options.logFile.empty()) {
            const auto logFile = std::filesystem::path(_options.outputDir) / "output.log";
            _options.logFile = logFile.string();
        }

        if (!_options.input.empty() && !isValidInput(_options)) {
            result = 1;
        }

    } catch (const CLI::ParseError &e) {
        result = exit(e);
    }

    return result;
}

#ifdef _WIN32
void App::prepareConsoleForUTF8() {
    SetConsoleOutputCP(CP_UTF8);
    setvbuf(stdout, nullptr, _IOFBF, 1000);
}

void App::prepareConsoleForVT100() {
    const auto stdOutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleMode(stdOutHandle, ENABLE_VIRTUAL_TERMINAL_PROCESSING | ENABLE_PROCESSED_OUTPUT);
}
#endif


bool App::isCPoolPrinterNeeded() const {
    const auto result = _options.printConstantPool || _options.printCPoolHtmlNav;
    return result;
}

void App::init() {

    auto result = true;

#ifdef _WIN32
    prepareConsoleForVT100();
    prepareConsoleForUTF8();
#endif

    ensureDir(_options.outputDir);
    _log->init(_options.logFile);

    if (isCPoolPrinterNeeded()) {
        ConstantPoolPrinter::init();
        if (_options.printCPoolHtmlNav) {
            nav::HtmlGen::init();
        }
    }

}
