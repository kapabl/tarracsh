#ifndef TARRACSH_PUBLIC_DIGEST_CMD_H
#define TARRACSH_PUBLIC_DIGEST_CMD_H


#include "domain/db/DigestDb.h"
#include "app/commands/DbBasedCommand.h"
#include "infrastructure/app/CliApp.h"


namespace kapa::tarracsh::app::commands::digest {

class PublicDigest final : public DbBasedCommand {
public:
    explicit PublicDigest(CLI::App *parent);

    [[nodiscard]] infrastructure::app::cli::ExitCode run() override;


private:
    [[nodiscard]] static bool runAsClient();
    void addMainSubCommand() override;
    [[nodiscard]] infrastructure::app::cli::ExitCode digestInput();
    [[nodiscard]] static bool runAsServer();
    [[nodiscard]] bool initDb() override;
    bool runAsStandalone() override;


};


}
#endif
