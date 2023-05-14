#ifndef CLDIFF_APP_H
#define CLDIFF_APP_H


#include "domain/Options.h"
#include "domain/classfile/ClassFileParser.h"
#include "domain/stats/Results.h"
#include "infrastructure/app/CliApp.h"

namespace kapa::cldiff::app {
using tarracsh::domain::classfile::ClassFileParser;

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

    std::unique_ptr<ClassFileParser> _leftParser;
    std::unique_ptr<ClassFileParser> _rightParser;
    unsigned int _differenceCount{0};
    bool _ignoreCode{false};

    typedef std::function<void(const bool, const std::string &, const std::string &)> ClassnameCompareCallback;

    void init() const;

    ClassfileDiffApp(const std::string &description, const std::string &name,
                     std::shared_ptr<infrastructure::log::Log> log);
    static std::string getClassInfoName(const std::unique_ptr<ClassFileParser> &parser, const tarracsh::domain::classfile::constantpool::u2 classInfoIndex);

    auto areClassnamesEqual(
        tarracsh::domain::classfile::constantpool::u2 leftClassInfo,
        tarracsh::domain::classfile::constantpool::u2 rightClassInfo,
        const ClassnameCompareCallback &callback) const -> bool;
    auto areClassnamesEqual(
        tarracsh::domain::classfile::constantpool::u2 leftClassInfo,
        tarracsh::domain::classfile::constantpool::u2 rightClassInfo) const -> bool;

    void compareSuperClass();
    bool compareAccessFlags(const tarracsh::domain::classfile::constantpool::u2 accessFlagsLeft,
                            const tarracsh::domain::classfile::constantpool::u2 accessFlagsRight, std::string &message);
    void compareMainClassAccessFlags();
    void compareMainClassname();
    void compareMainClassInterfaces();
    auto getLeftString(tarracsh::domain::classfile::constantpool::u2 constantPoolIndex) const -> std::string;
    auto getRightString(tarracsh::domain::classfile::constantpool::u2 constantPoolIndex) const -> std::string;
    static auto getString(const std::unique_ptr<ClassFileParser> &parser, tarracsh::domain::classfile::constantpool::u2 constantPoolIndex) -> std::string;
    bool compareAttributes(const std::vector<tarracsh::domain::classfile::attribute::AttributeInfo> &left,
                           const std::vector<tarracsh::domain::classfile::attribute::AttributeInfo> &right, std::string &message);
    void compareMainClassAttributes();
    void compareMainClass();
    auto areMethodsEqual(const tarracsh::domain::classfile::constantpool::MethodInfo &leftMethod,
                         const tarracsh::domain::classfile::constantpool::MethodInfo &rightMethod, std::string &message) -> bool;
    void compareMethods();
    bool compareDescriptors(const tarracsh::domain::classfile::constantpool::u2 leftIndex,
                            const tarracsh::domain::classfile::constantpool::u2 rightIndex, std::string &message);
    void compareFields();
    void compareMembers();
    void compare();
    [[nodiscard]] auto start(int argc, char *argv[]) -> infrastructure::app::cli::ExitCode;
    void setupCliOptions();
    void validateFile(const std::string &filename) const;
    auto parseCli(int argc, char **argv) -> infrastructure::app::cli::ExitCode;
    void outputResult() const;


};


}
#endif
