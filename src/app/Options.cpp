#include "Options.h"
#include "../utils/FilesystemUtils.h"

using namespace org::kapa::tarracsh;

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
        if (fsUtils::isJar(path)) {
            jarFile = input;
        } else if (fsUtils::isClassfile(path)) {
            classFilePath = input;
        } else {
            result = false;
        }
    } else {
        result = false;
    }
    return result;
}
