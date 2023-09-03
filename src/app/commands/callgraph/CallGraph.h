#ifndef TARRACSH_CALL_GRAPH_CMD_H
#define TARRACSH_CALL_GRAPH_CMD_H

#include "app/commands/DbBasedCommand.h"


namespace kapa::tarracsh::app::commands::callgraph {

class CallGraph final: public DbBasedCommand {
public:
    explicit CallGraph(CLI::App* parent);
    [[nodiscard]] infrastructure::app::cli::ExitCode run() override;

private:
    [[nodiscard]] static bool runAsClient();
    void addMainSubCommand() override;
    [[nodiscard]] infrastructure::app::cli::ExitCode processInput();
    [[nodiscard]] static bool runAsServer();
    [[nodiscard]] bool initDb() override;
    bool runAsStandalone() override;

    auto registerColumns() -> bool;


};


}
#endif
