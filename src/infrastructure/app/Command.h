#ifndef KAPA_CLI_COMMAND_H
#define KAPA_CLI_COMMAND_H

#include <CLI/CLI.hpp>

#include "CliApp.h"


namespace kapa::infrastructure::app::cli::command {


class Command {
public:
    explicit Command(CLI::App* parent): _parent(parent) {}
    virtual ~Command() = default;
    [[nodiscard]] virtual ExitCode run() = 0;
    [[nodiscard]] virtual bool isSelected() const { return _parent->got_subcommand(_subCommand); }
    virtual void addCommand() = 0;
    [[nodiscard]] CLI::App* getSubCommand() const { return _subCommand; }

protected:
    CLI::App* _parent{ nullptr };
    CLI::App* _subCommand{ nullptr };
};


}
#endif
