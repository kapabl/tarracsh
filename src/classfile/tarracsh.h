#ifndef TARRACSH_H
#define TARRACSH_H

#define TARRACSH_VERSION "0.0.1"

#include <string>

#include "CLI11.hpp"
#include "Log.h"


namespace org::kapa::tarracsh {

struct Options {
    CLI::Option* classfileOption;
    CLI::Option* dirOption;
    CLI::Option* jarOption;

    std::string classFile;
    std::string directory;
    std::string jarFile;
    std::string classPath;
    std::string outputDir{"./output"};
    bool generatePublicSha{false};
    bool printClassParse{false};
    bool printConstantPool{false};
    std::string logFile{outputDir + "/result.log"};
    int workers{4};
};


struct Results {
    unsigned classfileCount{};
    unsigned classfileErrors{};
    unsigned jarfileCount{};
    unsigned jarfileErrors{};
    unsigned long classfileTime{};
    unsigned long jarfileTime{};

    Log resultLog;
};

}

#endif
