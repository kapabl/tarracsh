#ifndef TARRACSH_PARSE_CMD_H
#define TARRACSH_PARSE_CMD_H

#include "Command.h"
#include "../Options.h"
#include "../stats/Stats.h"


namespace org::kapa::tarracsh::app::commands {

class Parse final: public infrastructure::cli::command::Command {
public:
    explicit Parse(CLI::App* parent);
    [[nodiscard]] infrastructure::cli::command::ExitCode run() const override;
    void addCommand() override;
private:
    stats::Results& _results;
    Options& _options;
};


}
#endif
