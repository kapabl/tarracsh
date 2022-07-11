#ifndef SIGNATURE_STRUCTURES_H
#define SIGNATURE_STRUCTURES_H

#include <vector>
#include "../StructsCommon.h"
#include "visit_struct/visit_struct_intrusive.hpp"

namespace org::kapa::tarrash::signatures {

struct SignatureBase : attributes::AttributeBase {
    u2 signatureIndex{};
};

struct ClassSignature : SignatureBase {
};

struct MethodSignature : SignatureBase {
};

struct FieldSignature : SignatureBase {
};

struct Identifier {
    std::wstring value;
};


struct FieldTypeSignature;
struct TypeArgument;

struct TypeArguments {
    std::vector<TypeArgument> arguments;
};

struct PackageSpecifier {
    std::vector<Identifier> parts;
};

struct SimpleClassTypeSignature {
    Identifier identifier;
    TypeArguments typeArgumentOpt;
};

struct ClassTypeSignatureSuffix {
    SimpleClassTypeSignature suffix;
};

struct ClassTypeSignature {
    PackageSpecifier packageSpecifier;
    SimpleClassTypeSignature simpleClassTypeSignature;
    std::vector<ClassTypeSignatureSuffix> classTypeSignatureSuffixes;
};

struct BaseType {
    std::string type;
};


struct TypeVariableSignature {
    Identifier identifier;
};

struct TypeSignature;

struct ArrayTypeSignature {
    std::shared_ptr<TypeSignature> typeSignature;
};

struct FieldTypeSignature {
    ClassTypeSignature classTypeSignature;
    ArrayTypeSignature arrayTypeSignature;
    TypeVariableSignature typeVariableSignature;
};

struct TypeArgument {
    // wildCharIndicator -> '+', '-' or ''
    wchar_t wildCharIndicator{};
    FieldTypeSignature fieldTypeSignature;
};


struct TypeSignature {
    FieldTypeSignature fieldTypeSignature;
    BaseType baseType;
};

struct FieldTypeSignatureOpt {
    FieldTypeSignature fieldTypeSignature;
};


struct SuperclassSignature : ClassTypeSignature {

};

struct SuperinterfaceSignature : ClassTypeSignature {

};


struct ClassBound {
    FieldTypeSignatureOpt fieldTypeSignatureOpt;
};

struct InterfaceBound {
    FieldTypeSignature fieldTypeSignature;
};

struct FormalTypeParameter {
    std::wstring identifier;
    ClassBound classBound;
    std::vector<InterfaceBound> interfaces;
};

struct FormalTypeParameters {
    std::vector<FormalTypeParameter> parameters;
};

struct ClassSignatureNode {
    FormalTypeParameters formalTypeParameters;
    ClassTypeSignature superclassSignature;
    std::vector<ClassTypeSignature> interfaces;
};

struct ReturnType {
    TypeSignature typeSignature;
    wchar_t voidDescriptor{'V'};
};

struct ThrowsSignature {
    ClassTypeSignature classTypeSignature;
    TypeVariableSignature typeVariableSignature{};
};


struct MethodSignatureNode {
    FormalTypeParameters formalTypeParameters;
    std::vector<TypeSignature> signatures;
    ReturnType returnType;
    std::vector<ThrowsSignature> throwsSignatures;
};

}

VISITABLE_STRUCT(org::kapa::tarrash::signatures::Identifier, value);
VISITABLE_STRUCT(org::kapa::tarrash::signatures::ClassSignatureNode, formalTypeParameters, superclassSignature, interfaces);
VISITABLE_STRUCT(org::kapa::tarrash::signatures::FormalTypeParameters, parameters);
VISITABLE_STRUCT(org::kapa::tarrash::signatures::ReturnType, typeSignature, voidDescriptor);
VISITABLE_STRUCT(org::kapa::tarrash::signatures::ThrowsSignature, classTypeSignature, typeVariableSignature);
// VISITABLE_STRUCT(org::kapa::tarrash::signatures::SuperclassSignature);
// VISITABLE_STRUCT(org::kapa::tarrash::signatures::SuperinterfaceSignature);
VISITABLE_STRUCT(org::kapa::tarrash::signatures::ClassTypeSignature, packageSpecifier, simpleClassTypeSignature, classTypeSignatureSuffixes);

// struct ClassTypeSignature {
//     PackageSpecifier packageSpecifier;
//     SimpleClassTypeSignature simpleClassTypeSignature;
//     std::vector<ClassTypeSignatureSuffix> classTypeSignatureSuffixes;
// };

#endif
