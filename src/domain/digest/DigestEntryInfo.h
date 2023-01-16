#ifndef TARRACSH_DIGEST_ENTRY_INFO
#define TARRACSH_DIGEST_ENTRY_INFO


#include <filesystem>
#include "../jar/JarEntry.h"
#include "../digest/DigestUtils.h"


namespace kapa::tarracsh::domain::digest {

struct DigestJarEntryInfo {
    explicit DigestJarEntryInfo(const char *jarFilename, const jar::JarEntry &jarEntry)
        : jarEntry(jarEntry) {
        strongClassname = digestUtils::getStrongClassname(jarFilename, jarEntry.getClassname().c_str());
    }

    const jar::JarEntry &jarEntry;
    std::string strongClassname;
};

}
#endif
