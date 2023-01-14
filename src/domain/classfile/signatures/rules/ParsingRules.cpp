// ReSharper disable CppExpressionWithoutSideEffects
// ReSharper disable CppClangTidyClangDiagnosticUnusedValue
#include "ParsingRules.h"

#include "Rule.h"
#include "Kleene.h"
#include "Or.h"
#include "Terminal.h"
#include "Optional.h"
#include "JvmIdentifier.h"
#include "RuleFuncs.h"


using namespace kapa::tarracsh::signatures;
using namespace std;


void ParsingRules::initRules() {

    const auto jvmIdentifier = make_shared<JvmIdentifier>();
    const auto plusTerminal = make_shared<Terminal>(L"+");
    const auto wildcardIndicator = plusTerminal | '-';
    wildcardIndicator->setAnchor(false);

    const auto definedType = !wildcardIndicator >> _fieldType;

    const auto typeArgument = definedType | '*';
    typeArgument->setAnchor(false);

    const auto typeArguments = '<' >> +typeArgument >> '>';
    const auto packageSpecifier = +(jvmIdentifier >> '/');
    const auto simpleClassType = jvmIdentifier >> !typeArguments;
    const auto classTypeSuffix = '.' >> simpleClassType;
    const auto classType = 'L' >> !packageSpecifier >> simpleClassType >> *classTypeSuffix >> ';';
    const auto superClass = classType;
    const auto superInterface = classType;
    const auto interfaceBound = ':' >> _fieldType;
    const auto classBound = ':' >> !_fieldType;
    const auto formalTypeParameter = jvmIdentifier >> classBound >> *interfaceBound;
    const auto typeVariable = 'T' >> jvmIdentifier >> ';';
    const auto boolBaseType = make_shared<Terminal>(L"B");

    const auto baseType = boolBaseType | 'C' | 'D' | 'F' | 'I' | 'J' | 'S' | 'Z';
    baseType->captureChildren(true);

    const auto type = _fieldType | baseType;
    type->setAnchor(false);

    const auto types = '(' >> *type >> ')';
    const auto arrayType = '[' >> type;
    const auto voidDescriptor = std::make_shared<Terminal>(L"V");
    const auto returnType = type | voidDescriptor;
    const auto throwsSignature = classType | typeVariable;

    const auto formalTypeParameters = '<' >> (+formalTypeParameter) >> '>';

    _methodType =
        !formalTypeParameters
        >> types
        >> returnType
        >> *throwsSignature;

    _fieldType->addToOr(classType);
    _fieldType->addToOr(arrayType);
    _fieldType->addToOr(typeVariable);

    _class =
        !formalTypeParameters
        >> superClass
        >> *superInterface;

    SET_RULE_NAME(_class);
    SET_RULE_NAME(_fieldType);
    SET_RULE_NAME(_methodType);
    SET_RULE_NAME(throwsSignature);
    SET_RULE_NAME(returnType);
    SET_RULE_NAME(type);
    SET_RULE_NAME(types);
    SET_RULE_NAME(baseType);
    SET_RULE_NAME(arrayType);
    SET_RULE_NAME(typeVariable);
    SET_RULE_NAME(formalTypeParameter);
    SET_RULE_NAME(formalTypeParameters);
    SET_RULE_NAME(classBound);
    SET_RULE_NAME(interfaceBound);
    SET_RULE_NAME(superInterface);
    SET_RULE_NAME(superClass);
    SET_RULE_NAME(jvmIdentifier);
    SET_RULE_NAME(wildcardIndicator);
    SET_RULE_NAME(definedType);
    SET_RULE_NAME(typeArgument);
    SET_RULE_NAME(typeArguments);
    SET_RULE_NAME(packageSpecifier);
    SET_RULE_NAME(simpleClassType);
    SET_RULE_NAME(classTypeSuffix);
    SET_RULE_NAME(classType);
}

ParsingRules &ParsingRules::getInstance() {
    if (nullptr == instance) {
        instance = new ParsingRules();
        instance->initRules();
    }
    return *instance;
}
