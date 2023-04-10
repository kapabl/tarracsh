#ifndef TARRACSH_PARSE_CMD_H
#define TARRACSH_PARSE_CMD_H

#include "infrastructure/app/Command.h"
#include "domain/Options.h"
#include "domain/stats/Results.h"


namespace kapa::tarracsh::app::commands {

class Parse final: public infrastructure::app::cli::command::Command {
public:
    explicit Parse(CLI::App* parent);
    [[nodiscard]] infrastructure::app::cli::ExitCode run() override;
    void addCommand() override;
private:
    domain::stats::Results& _results;
    domain::Options& _options;
};


}
#endif
