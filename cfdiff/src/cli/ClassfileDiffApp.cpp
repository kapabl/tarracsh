#include "CfDiff.h"
#include "ClassfileDiffApp.h"
#include "infrastructure/filesystem/Utils.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

using kapa::infrastructure::app::cli::CliApp;
using kapa::infrastructure::app::cli::ExitCode;
using kapa::tarracsh::domain::Options;
using kapa::infrastructure::log::Log;

using kapa::infrastructure::filesystem::utils::ensureDir;


using namespace kapa::cldiff::app;

std::unique_ptr<ClassfileDiffApp> ClassfileDiffApp::_app;


void ClassfileDiffApp::setupCliOptions() {
    set_version_flag("--version", "version " CF_DIFF_VERSION);
    set_help_all_flag("--help-all");

    add_flag("--ignore-classname", _ignoreClassname, "Ignore classname during comparison");
    add_flag("-p,--public-only", _publicOnly, "yes/no, default yes");

    // add_option("--left-file", _leftFile, "left file to compare")->required();
    // add_option("--right-file", _rightFile, "right file to compare")->required();

    allow_extras();

    callback([this]() {
        const auto &args = remaining();
        if (args.size() != 2) {
            throw CLI::ValidationError("Exactly two files are required.");
        }
        _leftFile = args[0];
        _rightFile = args[1];
    });
}


void ClassfileDiffApp::validateFile(const std::string &file) const {
    if (!std::filesystem::exists(file)) {
        throw CLI::ValidationError(std::format("File does not exist: {}", file), CLI::ExitCodes::FileError);
    }
}

ExitCode ClassfileDiffApp::parseCli(int argc, char **argv) {
    ExitCode result = 0;
    try {
        parse(argc, argv);

        validateFile(_leftFile);
        validateFile(_rightFile);

    } catch (const CLI::ParseError &e) {
        result = exit(e);
    }
    return result;
}

ExitCode ClassfileDiffApp::start(int argc, char *argv[]) {
    setupCliOptions();
    auto exitCode = parseCli(argc, argv);
    if (exitCode != 0) {
        return exitCode;
    }
    //TODO run app
    return exitCode;
}

ExitCode ClassfileDiffApp::run(int argc, char **argv) {
    const auto log = std::make_shared<Log>();
    _app.reset(new ClassfileDiffApp("", "cfdiff", log));

    const auto result = _app->start(argc, argv);

    if (_app->_options.pause) {
        std::cin.get();
    }

    return result;

}

ClassfileDiffApp::ClassfileDiffApp(const std::string &description, const std::string &name, std::shared_ptr<Log> log)
    : CliApp(description, name), _log(log), _results(_options) {

    _results.log = log;
}


void ClassfileDiffApp::init() const {

    auto result = true;

#ifdef _WIN32
    prepareConsoleForVT100();
    prepareConsoleForUTF8();
#endif

    ensureDir(_options.outputDir);
    _log->init(_options.logFile);

}
