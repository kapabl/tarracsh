#ifndef TARRACSH_PUBLIC_DIGEST_CMD_H
#define TARRACSH_PUBLIC_DIGEST_CMD_H

#include "../../../infrastructure/app/Command.h"
#include "../../../domain/db/DigestDb.h"
#include "../../../domain/Options.h"
#include "../../../domain/stats/Results.h"
#include "../../../infrastructure/app/CliApp.h"


namespace kapa::tarracsh::app::commands::digest {

class PublicDigest final : public infrastructure::app::cli::command::Command {
public:
    explicit PublicDigest(CLI::App *parent);
    bool initDb();

    [[nodiscard]] infrastructure::app::cli::ExitCode run() override;
    void addCommand() override;


private:
    domain::stats::Results &_results;
    domain::Options &_options;
    CLI::App *_digestServerOptions{nullptr};
    [[nodiscard]] CLI::App *addServerOptions() const;
    infrastructure::app::cli::ExitCode digestInput();

    std::shared_ptr<domain::db::digest::DigestDb> _db;
};


}
#endif
