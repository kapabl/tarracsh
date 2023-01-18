#ifndef TARRACSH_APP_H
#define TARRACSH_APP_H

#include "../infrastructure/app/CliApp.h"
#include "../domain/Options.h"
#include "Config.h"

#include "commands/digest/PublicDigest.h"
#include "commands/CallGraphCommand.h"
#include "commands/ParseCommand.h"

using CliApp = kapa::infrastructure::app::cli::CliApp;

namespace kapa::tarracsh::app {
using ExitCode = infrastructure::app::cli::ExitCode;


class App final : public CliApp, public Config {

public:

    [[nodiscard]] static ExitCode run(int argc, char *argv[]);
    [[nodiscard]] ExitCode start(int argc, char *argv[]);

    [[nodiscard]] static domain::stats::Results &getGlobalResults() { return _app->_results; }
    [[nodiscard]] static domain::Options &getGlobalOptions() { return _app->_options; }    
    [[nodiscard]] static bool isValidInput(domain::Options& options);
    [[nodiscard]] static App& getApp() { return *_app; }

    [[nodiscard]] domain::stats::Results& getResults() override { return _results; }
    [[nodiscard]] domain::Options& getOptions() override { return _options; }

    [[nodiscard]] infrastructure::log::Log& getLog() override { return *_log; }


protected:
    std::shared_ptr<infrastructure::log::Log> _log;

private:
    domain::Options _options;
    void setupCliOptions();

    std::unique_ptr<commands::ParseCommand> _parseCommand{};
    std::unique_ptr<commands::CallGraphCommand> _callGraphCommand{};
    std::unique_ptr<commands::digest::PublicDigest> _digestCommand{};
    static std::unique_ptr<App> _app;


    App(const std::string& description, const std::string& name, std::shared_ptr<infrastructure::log::Log> log);


    [[nodiscard]] int parseCli(int argc, char **argv);
#ifdef _WIN32
    static void prepareConsoleForVT100();
    static void prepareConsoleForUTF8();
    static int __stdcall ctrlHandler(unsigned long fdwCtrlType);
    void controlCHandler();
#endif
    domain::stats::Results _results;

    [[nodiscard]] bool isCPoolPrinterNeeded() const;
    void init() const;
};


}

#endif
