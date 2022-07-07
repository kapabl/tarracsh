#ifndef SIGNATURE_STRUCTURES_H
#define SIGNATURE_STRUCTURES_H

#include <vector>
#include "../StructsCommon.h"

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
    SuperclassSignature superclassSignature;
    std::vector<SuperinterfaceSignature> interfaces;
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

#endif
