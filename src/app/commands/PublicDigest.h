#ifndef TARRACSH_PUBLIC_DIGEST_CMD_H
#define TARRACSH_PUBLIC_DIGEST_CMD_H

#include "../../infrastructure/app/Command.h"
#include "../Options.h"
#include "../stats/Stats.h"
#include "../../infrastructure/app/CliApp.h"


namespace kapa::tarracsh::app::commands {

class PublicDigest final : public infrastructure::app::cli::command::Command {
public:
    explicit PublicDigest(CLI::App *parent);
    [[nodiscard]] infrastructure::app::cli::ExitCode run() const override;
    void addCommand() override;


private:
    stats::Results &_results;
    Options &_options;
    CLI::App *_digestServerOptions{nullptr};
    [[nodiscard]] CLI::App *addServerOptions() const;
};


}
#endif
