#ifndef TARRACSH_DIGEST_ENTRY_INFO
#define TARRACSH_DIGEST_ENTRY_INFO


#include <filesystem>
#include "../jar/JarEntry.h"
#include "domain/Utils.h"


namespace kapa::tarracsh::domain {

struct JarEntryInfo {
    explicit JarEntryInfo(const char *jarFilename, const jar::JarEntry &jarEntry)
        : jarEntry(jarEntry) {
        strongClassname = utils::getStrongClassname(jarFilename, jarEntry.getClassname().c_str());
    }

    const jar::JarEntry &jarEntry;
    std::string strongClassname;
};

}
#endif
