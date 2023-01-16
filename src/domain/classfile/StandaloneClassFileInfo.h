#ifndef CLASSFILE_INFO_H
#define CLASSFILE_INFO_H
#include <filesystem>
#include "../../infrastructure/filesystem/Utils.h"


namespace kapa::tarracsh::domain::classfile {

struct StandaloneClassFileInfo {

    explicit StandaloneClassFileInfo(const std::string& filename) {
        this->filename = filename;
        size = std::filesystem::file_size(filename);
        timestamp = infrastructure::filesystem::utils::getLastWriteTimestamp(filename);
        isStandalone = true;
    }

    std::string filename;
    uintmax_t size;
    long long timestamp;
    bool isStandalone;

};

}
#endif