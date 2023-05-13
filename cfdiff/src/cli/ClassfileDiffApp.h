#ifndef CLDIFF_APP_H
#define CLDIFF_APP_H


#include "domain/Options.h"
#include "domain/stats/Results.h"
#include "infrastructure/app/CliApp.h"

namespace kapa::cldiff::app {

class ClassfileDiffApp final : public infrastructure::app::cli::CliApp {

public:
    static infrastructure::app::cli::ExitCode run(int argc, char **argv);

private:
    void setupCliOptions();
    void validateFile(const std::string &file) const;
    infrastructure::app::cli::ExitCode parseCli(int argc, char** argv);
    static std::unique_ptr<ClassfileDiffApp> _app;
    std::shared_ptr<infrastructure::log::Log> _log;
    tarracsh::domain::Options _options;

    tarracsh::domain::stats::Results _results;
    bool _ignoreClassname{false};
    bool _publicOnly{true};

    std::string _leftFile;
    std::string _rightFile;

    void init() const;

    ClassfileDiffApp(const std::string &description, const std::string &name,
        std::shared_ptr<infrastructure::log::Log> log);

    [[nodiscard]] infrastructure::app::cli::ExitCode start(int argc, char* argv[]);


};


}
#endif
