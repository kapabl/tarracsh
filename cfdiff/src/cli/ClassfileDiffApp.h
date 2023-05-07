#ifndef CLDIFF_APP_H
#define CLDIFF_APP_H


#include "domain/Options.h"
#include "domain/stats/Results.h"
#include "infrastructure/app/CliApp.h"

namespace kapa::cldiff::app {

class ClassfileDiffApp final : public infrastructure::app::cli::CliApp {

public:
    [[nodiscard]] infrastructure::app::cli::ExitCode start(int argc, char *argv[]);

    static infrastructure::app::cli::ExitCode run(int argc, char **argv);

private:
    static std::unique_ptr<ClassfileDiffApp> _app;
    std::shared_ptr<kapa::infrastructure::log::Log> _log;
    tarracsh::domain::Options _options;

    tarracsh::domain::stats::Results _results;

    void init() const;

    ClassfileDiffApp(const std::string &description, const std::string &name,
        std::shared_ptr<infrastructure::log::Log> log);


};


}
#endif
