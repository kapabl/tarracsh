#ifndef TARRACSH_APP_H
#define TARRACSH_APP_H

#include "Options.h"
#include "Config.h"
#include "stats/Stats.h"
#include <CLI11.hpp>

#include "commands/PublicDigest.h"
#include "commands/CallGraph.h"
#include "commands/Parse.h"

namespace org::kapa::tarracsh::app {
using ExitCode = infrastructure::cli::command::ExitCode;




class App final : public CLI::App, public Config {

public:

    static ExitCode run(int argc, char *argv[]);
    ExitCode start(int argc, char *argv[]);

    static stats::Results &getGlobalResults() { return _app->_results; }
    static Options &getGlobalOptions() { return _app->_options; }

    static void logln(const std::string &msg, const bool doStdout = false) {
        _app->_results.log.writeln(msg, doStdout);
        if (doStdout) {
            std::cout << msg << std::endl;
        }
    }

    static void log(const std::string &msg) { _app->_results.log.write(msg); }

    static bool isValidInput(Options& options);

    [[nodiscard]] static App& getApp() { return *_app; }

    stats::Results& getResults() override { return _results; }
    Options& getOptions() override { return _options; }

private:
    Options _options;
    void setupCliOptions();

    std::unique_ptr<commands::Parse> _parseCommand;
    std::unique_ptr<commands::CallGraph> _callGraphCommand;
    std::unique_ptr<commands::PublicDigest> _digestCommand;
    static std::unique_ptr<App> _app;


    App(const std::string& description, const std::string& name);


    int parseCli(int argc, char **argv);
#ifdef _WIN32
    static void prepareConsoleForVT100();
    static void prepareConsoleForUTF8();
#endif
    stats::Results _results;

    bool isCPoolPrinterNeeded() const;
    void init();
};


}

#endif
