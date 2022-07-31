#ifndef TARRACSH_APP_H
#define TARRACSH_APP_H

#include "Tarracsh.h"

namespace org::kapa::tarracsh {


class TarracshApp final : public CLI::App {

public:

    TarracshApp(std::string description, std::string name):
        App( description, name ) {}

    static int run(int argc, char *argv[]);
    int start(int argc, char *argv[]);

private:

    Options _options{};
    void setupCliOptions();
    int parseCli(int argc, char** argv);
#ifdef _WIN32
    static void prepareConsoleForWindows();
#endif

    static void init();
};


}

#endif
