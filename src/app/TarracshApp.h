#ifndef TARRACSH_APP_H
#define TARRACSH_APP_H

#include "Tarracsh.h"
#include "stats/Stats.h"

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

    static void logln(const std::string &msg, bool doStdout = false) {
        _results.log.writeln(msg);
        if (doStdout) {
            std::cout << msg << std::endl;
        }
    }
    
    static void log(const std::string &msg) { _results.log.write(msg); }

private:
    static Options _options;
    void setupCliOptions();

    CLI::Option_group* addParseOptions(CLI::Option* inputOption);
    CLI::Option_group* addCallGraphOptions(CLI::Option *inputOption);
    //CLI::Option_group* addPublicDigestOptions(CLI::Option *inputOption);
    CLI::App* addPublicDigestOptions(CLI::Option *inputOption);

    int parseCli(int argc, char **argv);
#ifdef _WIN32
    static void prepareConsoleForVT100();
    static void prepareForUTF8();
#endif
    static stats::Results _results;

    void init() const;
};


}

#endif
