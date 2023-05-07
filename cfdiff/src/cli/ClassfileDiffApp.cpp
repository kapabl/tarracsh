#include "ClassfileDiffApp.h"
#include "infrastructure/filesystem/Utils.h"

// #ifdef _WIN32
// // #define WIN32_LEAN_AND_MEAN
// // #include <Windows.h>
// #include <processenv.h>
// #include <consoleapi.h>
// #endif

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
// #include <Windows.h>
#include <windows.h>
// #include <processenv.h>
// #include <consoleapi.h>
#endif

using kapa::infrastructure::app::cli::CliApp;
using kapa::infrastructure::app::cli::ExitCode;
using kapa::tarracsh::domain::Options;
using kapa::infrastructure::log::Log;

using kapa::infrastructure::filesystem::utils::ensureDir;

using namespace kapa::cldiff::app;

std::unique_ptr<ClassfileDiffApp> ClassfileDiffApp::_app;


ExitCode ClassfileDiffApp::start(int argc, char *argv[]) {
    //TODO
    return 0;
}

ExitCode ClassfileDiffApp::run(int argc, char** argv) {
    const auto log = std::make_shared<Log>();
    _app.reset(new ClassfileDiffApp("", "Classfile diff", log));

    const auto result = _app->start(argc, argv);

    if (_app->_options.pause) {
        std::cin.get();
    }

    return result;

}

ClassfileDiffApp::ClassfileDiffApp(const std::string &description, const std::string &name, std::shared_ptr<Log> log) :
    CliApp(description, name), _log(log), _results(_options) {

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
