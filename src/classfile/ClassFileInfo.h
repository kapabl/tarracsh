#ifndef CLASSFILE_INFO_H
#define CLASSFILE_INFO_H
#include <filesystem>
#include "../utils/FilesystemUtils.h"
#include "../jars/JarEntry.h"


namespace org::kapa::tarracsh {

struct ClassFileInfo {


    explicit ClassFileInfo(const std::string& filename) {
        this->filename = filename;
        size = std::filesystem::file_size(filename);
        timestamp = fsUtils::getLastWriteTimestamp(filename);
        isStandalone = true;
    }

    explicit ClassFileInfo(jar::JarEntry const& jarEntry) {
        //TODO
        // filename = jarEntry.
        // size = std::filesystem::file_size(filename);
        // timestamp = fsUtils::getLastWriteTimestamp(filename);
        isStandalone = false;
    }

    std::string filename;
    uintmax_t size;
    long long timestamp;
    bool isStandalone;

};

}
#endif