#ifndef TARRACSH_APP_H
#define TARRACSH_APP_H

#include "Tarracsh.h"

namespace org::kapa::tarracsh {


class TarracshApp final : public CLI::App {

public:

    TarracshApp(const std::string &description, const std::string& name):
        App(description, name ) {}

    static int run(int argc, char *argv[]);
    int start(int argc, char *argv[]);

private:

    Options _options{};
    void setupCliOptions();
    int parseCli(int argc, char** argv);
#ifdef _WIN32
    static void prepareConsoleForVT100();
    static void prepareForUTF8();
#endif

    void init() const;
};


}

#endif
