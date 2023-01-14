#ifndef TARRACSH_CALL_GRAPH_CMD_H
#define TARRACSH_CALL_GRAPH_CMD_H
#include "../../infrastructure/app/Command.h"
#include "../Options.h"
#include "../stats/Stats.h"


namespace kapa::tarracsh::app::commands {

class CallGraph final: public infrastructure::app::cli::command::Command {
public:
    explicit CallGraph(CLI::App* parent);
    [[nodiscard]] infrastructure::app::cli::ExitCode run() const override;
    void addCommand() override;
private:
    stats::Results& _results;
    Options& _options;
};


}
#endif
