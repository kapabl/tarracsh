#ifndef CLDIFF_APP_H
#define CLDIFF_APP_H


#include "domain/Options.h"
#include "domain/classfile/ClassFileParser.h"
#include "domain/stats/Results.h"
#include "infrastructure/app/CliApp.h"

namespace kapa::cldiff::app {

class ClassfileDiffApp final : public infrastructure::app::cli::CliApp {

public:
    static infrastructure::app::cli::ExitCode run(int argc, char **argv);

private:

    static std::unique_ptr<ClassfileDiffApp> _app;
    std::shared_ptr<infrastructure::log::Log> _log;
    tarracsh::domain::Options _options;

    tarracsh::domain::stats::Results _results;
    bool _ignoreClassname{false};
    bool _publicOnly{true};

    std::string _leftFile;
    std::string _rightFile;
    std::vector<std::string> _differences;
    std::vector<std::string> _equalities;

    void init() const;

    ClassfileDiffApp(const std::string &description, const std::string &name,
        std::shared_ptr<infrastructure::log::Log> log);

    void compareSuperClass(tarracsh::domain::classfile::ClassFileParser & leftParser, tarracsh::domain::classfile::ClassFileParser & rightParser);
    void compareMainClassAccessFlags(tarracsh::domain::classfile::ClassFileParser & leftParser, tarracsh::domain::classfile::ClassFileParser & rightParser);
    void compareMainClass(tarracsh::domain::classfile::ClassFileParser & leftParser, tarracsh::domain::classfile::ClassFileParser & rightParser);
    void compareMethods(tarracsh::domain::classfile::ClassFileParser & leftParser, tarracsh::domain::classfile::ClassFileParser & rightParser);
    void compareFields(tarracsh::domain::classfile::ClassFileParser & leftParser, tarracsh::domain::classfile::ClassFileParser & rightParser);
    void compareMembers(tarracsh::domain::classfile::ClassFileParser & leftParser, tarracsh::domain::classfile::ClassFileParser & rightParser);
    void compare(tarracsh::domain::classfile::ClassFileParser & leftParser, tarracsh::domain::classfile::ClassFileParser & rightParser);
    [[nodiscard]] auto start(int argc, char* argv[]) -> infrastructure::app::cli::ExitCode;
    void setupCliOptions();
    void validateFile(const std::string& filename) const;
    auto parseCli(int argc, char** argv) -> infrastructure::app::cli::ExitCode;
    void outputResult() const;


};


}
#endif
