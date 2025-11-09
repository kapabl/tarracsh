#ifndef TARRACSH_CLASSFILE_HEADER_H
#define TARRACSH_CLASSFILE_HEADER_H

#include "StructsCommon.h"

namespace kapa::tarracsh::domain::classfile::constantpool {

struct ClassFileHeader {
    u4 magic;
    u2 minorVersion;
    u2 majorVersion;
};

}

#endif
