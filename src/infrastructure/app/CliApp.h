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
};


}

#endif
