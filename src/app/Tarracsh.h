#ifndef TARRACSH_H
#define TARRACSH_H

#define TARRACSH_VERSION "0.0.1"

#include <string>

#include "CLI11.hpp"
#include "../classfile/Log.h"
#include "../utils/FilesystemUtils.h"


namespace org::kapa::tarracsh {

struct Options {
    CLI::Option *inputOption;

    std::string classFilePath;
    std::string input;
    std::string directory;
    std::string jarFile;
    std::string classPath;
    std::string queryValue;
    std::string outputDir{"./output"};
    bool isParse{false};
    bool isPublicDigest{false};
    bool isCallGraph{false};
    bool printClassParse{false};
    bool printConstantPool{false};
    bool rebuild{false};
    bool dryRun{false};
    bool doDiffReport{true};
    bool printDiffReport{false};
    std::string logFile{outputDir + "/result.log"};
    int workers{4};
    bool useFileTimestamp{true};
    bool pause{false};
    bool printProfiler{false};

    struct SubCommands {
        CLI::App* digest;
        CLI::App* callGraph;
        CLI::App* parse;
    } subCommands;

    bool printCPoolHtmlNav{false};
    
    [[nodiscard]] bool canPrintProgress() const {
        const auto result = !printClassParse && !printConstantPool;
        return result;
    }

    bool processInput() {
        auto result = true;
        if (std::filesystem::is_directory(input)) {
            directory = input;
        }
        else if (std::filesystem::exists(input)) {
            const auto path = std::filesystem::path(input);
            if (fsUtils::isJar(path)) {
                jarFile = input;
            }
            else if (fsUtils::isClassfile(path)) {
                classFilePath = input;
            }
            else {
                result = false;
            }
        }
        else {
            result = false;
        }

        return result;
    }



};

}

#endif
