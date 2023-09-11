#ifndef TARRACSH_OPTIONS_H
#define TARRACSH_OPTIONS_H
#include <fmt/format.h>
#include <functional>
#include <iostream>
#include <string>

namespace kapa::tarracsh::domain {
// struct Options;

struct ServerOptions {
    bool isServerMode{false};
    int port{0xCA9A};
    bool stopServer{false};
    std::string listenAddress{"0.0.0.0"};
    [[nodiscard]] std::string getListenServerAddress() const;
};

struct ClientOptions {
    bool isClientMode{false};
    int port{0xCA9A};
    std::string host{"localhost"};

    [[nodiscard]] std::string getServerAddress() const;
};

struct SubCommandOptions {
    std::string input;
    bool isDir{ false };
    bool isJar{ false };
    bool isClassfile{ false };
    [[nodiscard]] bool processInput();
    [[nodiscard]] bool isValidInput();
    bool dryRun{ false };
    bool rebuild{ false };
    bool isDiff{ false };
    std::string queryValue;
    ServerOptions server;
    ClientOptions client;
    bool displayRaw{ false };
};

struct DigestOptions : SubCommandOptions {
};

struct CallGraphOptions : SubCommandOptions {
};

struct ParseOptions : SubCommandOptions {
    bool print{false};
    bool printConstantPool{false};
    bool printCPoolHtmlNav{false};
    bool descriptiveCPoolEntries{true};
    std::vector<std::string> cpoolFilter;
};

struct Options {
    std::string outputDir;
    bool isParse{false};
    bool isPublicDigest{false};
    bool isCallGraph{false};

    DigestOptions digest;
    CallGraphOptions callGraph;
    ParseOptions parse;

    std::string logFile;
    int workers{4};
    bool useFileTimestamp{true};
    bool pause{false};
    bool printProfiler{false};
    bool verbose{false};
    
    [[nodiscard]] bool canPrintProgress() const;
    SubCommandOptions& getSubCommandOptions();


};


}

#endif
