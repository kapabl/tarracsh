#ifndef SIGNATURE_STRUCTURES_H
#define SIGNATURE_STRUCTURES_H

// #include <vector>
#include "src/domain/classfile/types/StructsCommon.h"
#include "src/domain/classfile/types/AttributeStructures.h"


namespace kapa { namespace tarracsh { namespace domain { namespace classfile { namespace signature {


struct SignatureBase : attribute::AttributeBase {
    constantpool::u2 signatureIndex{};
};

struct ClassSignature : SignatureBase {
};

struct MethodSignature : SignatureBase {
};

struct FieldSignature : SignatureBase {
};


} } } } }


#endif
