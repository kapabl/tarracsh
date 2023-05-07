#ifndef KAPA_APP_H
#define KAPA_APP_H

#include <CLI/CLI.hpp>
#include "../log/Log.h"


namespace kapa::infrastructure::app::cli {
typedef int ExitCode;


class CliApp: public CLI::App {

public:

    CliApp(const std::string& description, const std::string& name):
        CLI::App(description, name)
    {
    }
protected:
#ifdef _WIN32
    static void prepareConsoleForVT100();
    static void prepareConsoleForUTF8();
    // static int __stdcall ctrlHandler(unsigned long fdwCtrlType);
    void controlCHandler();
#endif

};


}

#endif
