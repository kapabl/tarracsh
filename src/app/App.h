#ifndef TARRACSH_APP_H
#define TARRACSH_APP_H

#include "../infrastructure/app/CliApp.h"
#include "Options.h"
#include "Config.h"
#include "stats/Stats.h"

#include "commands/PublicDigest.h"
#include "commands/CallGraph.h"
#include "commands/Parse.h"

using CliApp = kapa::infrastructure::app::cli::CliApp;

namespace kapa::tarracsh::app {
using ExitCode = infrastructure::app::cli::ExitCode;


class App final : public CliApp, public Config {

public:

    [[nodiscard]] static ExitCode run(int argc, char *argv[]);
    [[nodiscard]] ExitCode start(int argc, char *argv[]);

    [[nodiscard]] static stats::Results &getGlobalResults() { return _app->_results; }
    [[nodiscard]] static Options &getGlobalOptions() { return _app->_options; }
    
    [[nodiscard]] static bool isValidInput(Options& options);

    [[nodiscard]] static App& getApp() { return *_app; }

    [[nodiscard]] stats::Results& getResults() override { return _results; }
    [[nodiscard]] Options& getOptions() override { return _options; }

    [[nodiscard]] infrastructure::log::Log& getLog() override { return *_log; }


protected:
    std::shared_ptr<infrastructure::log::Log> _log;

private:
    Options _options;
    void setupCliOptions();

    std::unique_ptr<commands::Parse> _parseCommand{};
    std::unique_ptr<commands::CallGraph> _callGraphCommand{};
    std::unique_ptr<commands::PublicDigest> _digestCommand{};
    static std::unique_ptr<App> _app;


    App(const std::string& description, const std::string& name, std::shared_ptr<infrastructure::log::Log> log);


    [[nodiscard]] int parseCli(int argc, char **argv);
#ifdef _WIN32
    static void prepareConsoleForVT100();
    static void prepareConsoleForUTF8();
#endif
    stats::Results _results;

    [[nodiscard]] bool isCPoolPrinterNeeded() const;
    void init();
};


}

#endif
