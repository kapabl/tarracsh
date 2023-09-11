#include <fmt/format.h>
#include <fmt/chrono.h>
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

SubCommandOptions& Options::getSubCommandOptions() {
    if (isPublicDigest) return digest;
    if (isCallGraph) return callGraph;
    return parse;
}

bool SubCommandOptions::processInput() {
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

bool SubCommandOptions::isValidInput() {
    const auto result = processInput();
    if (!result) {
        std::cout << fmt::format("Input should be a directory, jar or class file. Invalid input:{}", input) << std::endl;
    }
    return result;
}

std::string ServerOptions::getListenServerAddress() const {
    auto result = fmt::format("{}:{}", listenAddress, port);
    return result;
}

std::string ClientOptions::getServerAddress() const {
    auto result = fmt::format("{}:{}", host, port);
    return result;
}
