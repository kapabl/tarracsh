#ifndef TARRACSH_OPTIONS_H
#define TARRACSH_OPTIONS_H
#include <functional>
#include <string>

namespace kapa::tarracsh::domain {
// struct Options;

struct DigestServerOptions {
    bool isServerMode{false};
    int port{0xCA9A};
    bool stopServer{false};
    std::string listenAddress{"0.0.0.0"};
    [[nodiscard]] std::string getListenServerAddress() const;
};

struct DigestClientOptions {
    bool isClientMode{ false };
    int port{ 0xCA9A };
    std::string host{ "localhost" };

    [[nodiscard]] std::string getServerAddress() const;
};

struct Options {
    std::string classFilePath;
    std::string input;
    std::string directory;
    std::string jarFile;
    std::string classPath;
    std::string queryValue;
    std::string outputDir;
    bool isParse{false};
    bool isPublicDigest{false};
    bool isCallGraph{false};
    bool printClassParse{false};
    bool printConstantPool{false};
    bool rebuild{false};
    bool dryRun{false};
   
    struct Diff {
        bool enabled{ true };
        bool print{ false };
    } diff;

    std::string logFile;
    int workers{4};
    bool useFileTimestamp{true};
    bool pause{false};
    bool printProfiler{false};
    bool printCPoolHtmlNav{false};
    bool descriptiveCPoolEntries{true};
    bool verbose{false};
    DigestServerOptions digestServer;
    DigestClientOptions digestClient;
    bool diffOnServer;

    [[nodiscard]] bool canPrintProgress() const;
    [[nodiscard]] bool processInput();


};


}

#endif
