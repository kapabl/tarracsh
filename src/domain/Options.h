#ifndef TARRACSH_OPTIONS_H
#define TARRACSH_OPTIONS_H
#include <format>
#include <functional>
#include <iostream>
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
    bool isClientMode{false};
    int port{0xCA9A};
    std::string host{"localhost"};

    [[nodiscard]] std::string getServerAddress() const;
};

struct InputOptions {
    std::string input;
    bool isDir{ false };
    bool isJar{ false };
    bool isClassfile{ false };
    [[nodiscard]] bool processInput();
    [[nodiscard]] bool isValidInput();
};

struct DigestOptions : InputOptions {
    bool dryRun{false};
    bool rebuild{false};
    bool isDiff{false};
    std::string queryValue;
    DigestServerOptions server;
    DigestClientOptions client;
    bool displayRaw{ false };
};

struct CallGraphOptions : InputOptions {
    //TODO
};

struct ParseOptions : InputOptions {
    bool print{false};
    bool printConstantPool{false};
    bool printCPoolHtmlNav{false};
    bool descriptiveCPoolEntries{true};
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
    InputOptions& getInputOptions();


};


}

#endif
