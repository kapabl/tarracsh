#ifndef TARRACSH_APP_H
#define TARRACSH_APP_H

#include "Tarracsh.h"
#include "Stats.h"

namespace org::kapa::tarracsh {


class TarracshApp final : public CLI::App {

public:
    TarracshApp(const std::string &description, const std::string &name)
        : App(description, name) {
    }

    static int run(int argc, char *argv[]);
    int start(int argc, char *argv[]);

    static stats::Results &getResults() { return _results; }

    static void logln(const std::string &msg, bool doStdout = false) {
        _results.log.writeln(msg);
        if (doStdout) {
            std::cout << msg << std::endl;
        }
    }


    static void log(const std::string &msg) { _results.log.write(msg); }

private:
    Options _options{};
    void setupCliOptions();
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
