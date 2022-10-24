#ifndef TARRACSH_H
#define TARRACSH_H

#define TARRACSH_VERSION "0.0.1"

#include <string>

#include "CLI11.hpp"
#include "Log.h"


namespace org::kapa::tarracsh {

struct Options {
    CLI::Option *classfileOption;
    CLI::Option *dirOption;
    CLI::Option *jarOption;

    std::string classFilePath;
    std::string directory;
    std::string jarFile;
    std::string classPath;
    std::string outputDir{"./output"};
    bool generatePublicDigest{false};
    bool printClassParse{false};
    bool printConstantPool{false};
    bool rebuild{false};
    std::string logFile{outputDir + "/result.log"};
    int workers{4};
    bool useFileTimestamp{true};

};


struct PublicDigestResult {
    int unchangedCount{};
    int count{};
    int same{};
    int differentDigest{};
    int newFile{};
};

struct ClassfileStats {
    unsigned count{};
    unsigned parsedCount{};
    unsigned errors{};
    PublicDigestResult digest;
};


struct Results {

    ClassfileStats classfiles;

    struct {
        unsigned count{};
        unsigned parsedCount{};
        unsigned errors{};
        unsigned classfileCount{};
        PublicDigestResult digest;
        ClassfileStats classfiles;
    } jarfiles;


    unsigned long classfileTime{};
    unsigned long jarfileTime{};

    Log resultLog;

};

struct PrintTimeScope {
    PrintTimeScope(const bool autoStart ) {
        if (autoStart) {
            start();
        }
    }

    void start() {
        startTime = std::chrono::high_resolution_clock::now();
    }

    void stop() {
        endTime = std::chrono::high_resolution_clock::now();
    }

    void printElapsedTime() {
        stop();
        auto totalTime = std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime);
        std::cout << std::endl << std::format("total time: {}", totalTime) << std::endl;
    }

    ~PrintTimeScope() {

        printElapsedTime();
    }

    std::chrono::time_point<std::chrono::steady_clock> startTime{};
    std::chrono::time_point<std::chrono::steady_clock> endTime{};

};

}

#endif
