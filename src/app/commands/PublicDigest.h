#ifndef TARRACSH_PUBLIC_DIGEST_CMD_H
#define TARRACSH_PUBLIC_DIGEST_CMD_H

#include "Command.h"
#include "../Options.h"
#include "../stats/Stats.h"


namespace org::kapa::tarracsh::app::commands {

class PublicDigest final: public infrastructure::cli::command::Command {
public:
    explicit PublicDigest(CLI::App* parent);
    [[nodiscard]] infrastructure::cli::command::ExitCode run() const override;
    void addCommand() override;


private:
    stats::Results& _results;
    Options& _options;
    CLI::App * _digestServerOptions{nullptr};
    CLI::App* addServerOptions() const;
};


}
#endif
