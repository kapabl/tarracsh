#ifndef SIGNATURE_STRUCTURES_H
#define SIGNATURE_STRUCTURES_H

#include <vector>
#include "../constantpool/StructsCommon.h"
#include "rules/NodeTraits.h"
#include "../AttributeStructures.h"
#include "visit_struct/visit_struct_intrusive.hpp"

namespace kapa::tarracsh::domain::classfile::signature {


struct SignatureBase : attribute::AttributeBase {
    constantpool::u2 signatureIndex{};
};

struct ClassSignature : SignatureBase {
};

struct MethodSignature : SignatureBase {
};

struct FieldSignature : SignatureBase {
};

struct Identifier {
    BEGIN_VISITABLES(Identifier);
    VISITABLE(std::wstring, value);
    END_VISITABLES;
};


struct FieldTypeSignature;
struct TypeArgument;
struct TypeArguments {
    BEGIN_VISITABLES(TypeArguments);
    VISITABLE(std::vector<TypeArgument>, arguments);
    END_VISITABLES;
};

struct PackageSpecifier {
    BEGIN_VISITABLES(PackageSpecifier);
    // VISITABLE(Identifier, identifier);
    // VISITABLE(std::wstring, packateSeparatorTerminal);
    // VISITABLE(std::vector<std::shared_ptr<PackageSpecifier>>, next);
       VISITABLE(std::vector<Identifier>, next);
    END_VISITABLES;
};

struct SimpleClassTypeSignature {
    BEGIN_VISITABLES(SimpleClassTypeSignature);
    VISITABLE(Identifier, identifier);
    VISITABLE(TypeArguments, typeArgumentOpt);
    END_VISITABLES;
};

struct ClassTypeSuffix {
    BEGIN_VISITABLES(ClassTypeSuffix);
    VISITABLE(std::wstring, dotTerminal);
    VISITABLE(SimpleClassTypeSignature, suffix);
    END_VISITABLES;
};

struct ClassType {
    BEGIN_VISITABLES(ClassType);
    VISITABLE(PackageSpecifier, packageSpecifier);
    VISITABLE(SimpleClassTypeSignature, simpleClassTypeSignature);
    VISITABLE(std::vector<ClassTypeSuffix>, classTypeSignatureSuffixes);
    END_VISITABLES;
};



struct BaseType {
    BEGIN_VISITABLES(BaseType);
    VISITABLE(std::wstring, type);
    END_VISITABLES;
};

template<>
struct is_or_single_capture<BaseType> {
    static constexpr bool value = true;
};


struct TypeVariable {
    BEGIN_VISITABLES(TypeVariable);
    VISITABLE(Identifier, identifier);
    END_VISITABLES;
};

struct TypeSignature;

struct ArrayType {
    BEGIN_VISITABLES(ArrayType);
    VISITABLE(std::shared_ptr<TypeSignature>, typeSignature);
    END_VISITABLES;
};

struct FieldTypeSignature {
    BEGIN_VISITABLES(FieldTypeSignature);
    VISITABLE(ClassType, classTypeSignature);
    VISITABLE(ArrayType, arrayTypeSignature);
    VISITABLE(TypeVariable, typeVariableSignature);
    END_VISITABLES;
};

struct TypeSignature {
    BEGIN_VISITABLES(TypeSignature);
    VISITABLE(FieldTypeSignature, fieldTypeSignature);
    VISITABLE(BaseType, baseType);
    END_VISITABLES;
};

struct FieldTypeSignatureOpt {
    BEGIN_VISITABLES(FieldTypeSignatureOpt);
    VISITABLE(FieldTypeSignature, fieldTypeSignature);
    END_VISITABLES;
};


struct DefinedType {
    BEGIN_VISITABLES(DefinedType);
    VISITABLE(std::wstring, wildCharIndicator);
    VISITABLE(FieldTypeSignature, fieldTypeSignature);
    END_VISITABLES;
};



struct TypeArgument {
    BEGIN_VISITABLES(TypeArgument);
    VISITABLE(DefinedType, definedType);
    VISITABLE(std::wstring, starTerminal);
    END_VISITABLES;
};


struct ClassBound {
    BEGIN_VISITABLES(ClassBound);
    VISITABLE(std::wstring, colon);
    VISITABLE(FieldTypeSignatureOpt, fieldTypeSignatureOpt);
    END_VISITABLES;
};

struct InterfaceBound {
    BEGIN_VISITABLES(InterfaceBound);
    VISITABLE(std::wstring, colon);
    VISITABLE(FieldTypeSignature, fieldTypeSignature);
    END_VISITABLES;
};


struct FormalTypeParameter {
    BEGIN_VISITABLES(FormalTypeParameter);
    VISITABLE(std::wstring, identifier);
    VISITABLE(ClassBound, classBound);
    VISITABLE(std::vector<InterfaceBound>, interfaces);
    END_VISITABLES;
};

struct FormalTypeParameters {
    BEGIN_VISITABLES(FormalTypeParameters);
    VISITABLE(std::vector<FormalTypeParameter>, parameters);
    END_VISITABLES;
};

struct ClassSignatureNode {
    BEGIN_VISITABLES(ClassSignatureNode);
    VISITABLE(std::wstring, classTerminal);
    VISITABLE(FormalTypeParameters, formalTypeParameters);
    VISITABLE(ClassType, superclassSignature);
    VISITABLE(std::vector<ClassType>, interfaces);
    END_VISITABLES;
};

struct ReturnType {
    BEGIN_VISITABLES(ReturnType);
    VISITABLE(TypeSignature, typeSignature);
    VISITABLE(std::wstring, voidDescriptor);
    END_VISITABLES;
};

struct ThrowsSignature {
    BEGIN_VISITABLES(ThrowsSignature);
    VISITABLE(ClassType, classTypeSignature);
    VISITABLE(TypeVariable, typeVariableSignature);
    END_VISITABLES;
};


struct MethodSignatureNode {
    BEGIN_VISITABLES(MethodSignatureNode);
    // VISITABLE(FormalTypeParameters, formalTypeParameters);
    VISITABLE(std::vector<FormalTypeParameter>, parameters);
    VISITABLE(std::vector<TypeSignature>, signatures);
    VISITABLE(ReturnType, returnType);
    VISITABLE(std::vector<ThrowsSignature>, throwsSignatures);
    END_VISITABLES;
};

}


#endif
