#include <format>
#include "Options.h"
#include "infrastructure/filesystem/Utils.h"

using namespace kapa::tarracsh::domain;

bool Options::canPrintProgress() const {
    const auto result = !parse.print &&
        !parse.printConstantPool &&
        !digest.isDiff &&
        !digest.server.isServerMode;
    return result;
}

BaseOptions& Options::getBaseOptions() {
    if (isPublicDigest) return digest;
    if (isCallGraph) return callGraph;
    return parse;
}

bool BaseOptions::processInput() {
    isDir = std::filesystem::is_directory(input);
    if (isDir) return true;

    if (std::filesystem::exists(input)) {
        const auto path = std::filesystem::path(input);
        isJar = infrastructure::filesystem::utils::isJar(path);
        if (isJar) return true;

        isClassfile = infrastructure::filesystem::utils::isClassfile(path);
        if(isClassfile) return true;        
    }
    return false;
}

bool BaseOptions::isValidInput() {
    const auto result = processInput();
    if (!result) {
        std::cout << std::format("Input should be a directory, jar or class file. Invalid input:{}", input) << std::endl;
    }
    return result;
}

std::string ServerOptions::getListenServerAddress() const {
    auto result = format("{}:{}", listenAddress, port);
    return result;
}

std::string ClientOptions::getServerAddress() const {
    auto result = format("{}:{}", host, port);
    return result;
}
