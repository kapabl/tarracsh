#ifndef TARRACSH_CALL_GRAPH_CMD_H
#define TARRACSH_CALL_GRAPH_CMD_H
#include "../../infrastructure/app/Command.h"
#include "../../domain/Options.h"
#include "../../domain/db/CallGraphDb.h"
#include "../../domain/stats/Results.h"


namespace kapa::tarracsh::app::commands {

class CallGraphCommand final: public infrastructure::app::cli::command::Command {
public:
    explicit CallGraphCommand(CLI::App* parent);
    bool initDb();
    [[nodiscard]] infrastructure::app::cli::ExitCode run() override;
    void addCommand() override;
private:
    domain::stats::Results& _results;
    domain::Options& _options;

    std::shared_ptr<domain::db::callgraph::CallGraphDb> _db;
};


}
#endif
