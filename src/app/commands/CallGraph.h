#ifndef TARRACSH_CALL_GRAPH_CMD_H
#define TARRACSH_CALL_GRAPH_CMD_H
#include "Command.h"
#include "../Options.h"
#include "../stats/Stats.h"


namespace org::kapa::tarracsh::app::commands {

class CallGraph final: public infrastructure::cli::command::Command {
public:
    explicit CallGraph(CLI::App* parent);
    [[nodiscard]] infrastructure::cli::command::ExitCode run() const override;
    void addCommand() override;
private:
    stats::Results& _results;
    Options& _options;
};


}
#endif
