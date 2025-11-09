#include "App.h"
#include "app/AppRuntime.h"
#include "infrastructure/filesystem/Utils.h"
#include "app/Tarracsh.h"

#include "app/classfile/constantpool/printer/ConstantPoolPrinter.h"
#include "app/classfile/constantpool/printer/nav/HtmlGen.h"
#include "app/server/digest/ServiceImpl.h"

using kapa::tarracsh::app::classfile::constantpool::printer::ConstantPoolPrinter;
using kapa::tarracsh::app::classfile::constantpool::printer::nav::HtmlGen;
using kapa::infrastructure::log::Log;
using kapa::infrastructure::filesystem::utils::getUserHomeDir;
using kapa::infrastructure::filesystem::utils::ensureDir;

using namespace kapa::tarracsh::app;


std::unique_ptr<App> App::_app;

App::App(const std::string &description, const std::string &name, const std::shared_ptr<Log> log)
        : CliApp(description, name), _log(log), _results(_options) {

    _results.log = log;
}

ExitCode App::run(const int argc, char *argv[]) {

    const auto log = std::make_shared<Log>();
    _app.reset(new App("", "Tarracsh", log));

    const auto result = _app->start(argc, argv);

    if (_app->_options.pause) {
        std::cin.get();
    }

    return result;
}

ExitCode App::start(const int argc, char *argv[]) {

    auto exitCode = parseCli(argc, argv);
    if (exitCode != 0  || _isHelp) {
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

    _digestCommand = std::make_unique<commands::digest::PublicDigest>(this);
    _digestCommand->addCommand();

    _parseCommand = std::make_unique<commands::Parse>(this);
    _parseCommand->addCommand();

    _callGraphCommand = std::make_unique<commands::callgraph::CallGraph>(this);
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
            std::cout << fmt::format("Invalid sub-command") << std::endl;
            result = 1;
        }

        if (_options.outputDir.empty()) {
            _options.outputDir = (getUserHomeDir() / "tarracsh" / "output").string();
        }

        if (_options.logFile.empty()) {
            const auto logFile = std::filesystem::path(_options.outputDir) / "output.log";
            _options.logFile = logFile.string();
        }

    } catch (const CLI::CallForHelp &e) {
        _isHelp = true;
        result = exit(e);
    } catch (const CLI::CallForAllHelp &e) {
        _isHelp = true;
        result = exit(e);
    } catch (const CLI::ParseError &e) {
        result = exit(e);
    }

    return result;
}


bool App::isCPoolPrinterNeeded() const {
    const auto result = _options.parse.printConstantPool || _options.parse.printCPoolHtmlNav;
    return result;
}



void App::init() const {

    ensureDir(_options.outputDir);
    _log->init(_options.logFile);

    if (isCPoolPrinterNeeded()) {
        ConstantPoolPrinter::init(App::getContext());
        if (_options.parse.printCPoolHtmlNav) {
            HtmlGen::init();
        }
    }

}

namespace kapa::tarracsh::app::runtime {

domain::stats::Results &global_results() {
    return App::getGlobalResults();
}

domain::Options &global_options() {
    return App::getGlobalOptions();
}

Context &context() {
    return App::getContext();
}

App &app() {
    return App::getApp();
}

} // namespace kapa::tarracsh::app::runtime
