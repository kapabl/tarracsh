#ifndef TARRACSH_APP_H
#define TARRACSH_APP_H

#include "Options.h"
#include "stats/Stats.h"
#include <CLI11.hpp>
namespace org::kapa::tarracsh {


class TarracshApp final : public CLI::App {

public:
    TarracshApp(const std::string &description, const std::string &name)
        : App(description, name) {
    }

    static int run(int argc, char *argv[]);
    int start(int argc, char *argv[]);

    static stats::Results &getResults() { return _results; }
    static Options &getOptions() { return _options; }

    static void logln(const std::string &msg, const bool doStdout = false) {
        _results.log.writeln(msg, doStdout );
        if (doStdout) {
            std::cout << msg << std::endl;
        }
    }

    static void log(const std::string &msg) { _results.log.write(msg); }

private:
    static Options _options;
    void setupCliOptions();
    bool isValidInput() const;

    CLI::App *addParseSubCommand();
    CLI::App *addCallGraphSubCommand();
    CLI::App *addPublicDigestSubCommand();
    
    CLI::App *_digest;
    CLI::App *_callGraph;
    CLI::App *_parse;


    int parseCli(int argc, char **argv);
#ifdef _WIN32
    static void prepareConsoleForVT100();
    static void prepareConsoleForUTF8();
#endif
    static stats::Results _results;

    static bool isCPoolPrinterNeeded();
    void init() const;
};


}

#endif
