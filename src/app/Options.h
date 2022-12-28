#ifndef TARRACSH_OPTIONS_H
#define TARRACSH_OPTIONS_H

#include <string>

namespace org::kapa::tarracsh {

struct Options {
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

    bool printCPoolHtmlNav{false};
    
    [[nodiscard]] bool canPrintProgress() const;

    bool processInput();


};

}

#endif
