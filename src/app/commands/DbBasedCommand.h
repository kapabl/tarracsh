#ifndef TARRACSH_DB_BASED_CMD_H
#define TARRACSH_DB_BASED_CMD_H

#include "infrastructure/app/Command.h"
#include "infrastructure/db/Database.h"
#include "domain/Options.h"
#include "domain/stats/Results.h"
#include "infrastructure/app/CliApp.h"


namespace kapa::tarracsh::app::commands {

class DbBasedCommand : public infrastructure::app::cli::command::Command {
public:
    explicit DbBasedCommand(CLI::App *parent, domain::BaseOptions& dbBasedOptions);
    void addCommand() override;


protected:
    domain::stats::Results &_results;
    domain::Options &_options;
    CLI::App *_serverSubCommand{nullptr};
    domain::BaseOptions& _dbBasedOptions;
    [[nodiscard]] CLI::App *addServerSubCommand() const;
    [[nodiscard]] CLI::Option* addClientOptions() const;
    [[nodiscard]] bool isClientMode() const;
    [[nodiscard]] static bool runAsClient();
    [[nodiscard]] CLI::Option *addQueryOptions() const;
    virtual void addMainSubCommand() = 0;
    [[nodiscard]] bool isServerMode() const;
    [[nodiscard]] static bool runAsServer();

    std::shared_ptr<kapa::infrastructure::db::Database> _db;

    [[nodiscard]] virtual bool initDb() = 0;
    [[nodiscard]] virtual bool runAsStandalone() = 0;
};


}
#endif
