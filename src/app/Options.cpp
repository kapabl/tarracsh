#include "Options.h"
#include "../infrastructure/filesystem/Utils.h"

using namespace kapa::tarracsh;

bool Options::canPrintProgress() const {
    const auto result = !printClassParse && !printConstantPool && !printDiffReport;
    return result;
}

bool Options::processInput() {
    auto result = true;
    if (std::filesystem::is_directory(input)) {
        directory = input;
    } else if (std::filesystem::exists(input)) {
        const auto path = std::filesystem::path(input);
        if (infrastructure::filesystem::utils::isJar(path)) {
            jarFile = input;
        } else if (infrastructure::filesystem::utils::isClassfile(path)) {
            classFilePath = input;
        } else {
            result = false;
        }
    } else {
        result = false;
    }
    return result;
}

std::string ServerOptions::getListenServerAddress() const {
    auto result = format("{}:{}", listenAddress, port);
    return result;
}

std::string ServerOptions::getServerAddress() const {
    auto result = format("{}:{}", host, port);
    return result;
}
