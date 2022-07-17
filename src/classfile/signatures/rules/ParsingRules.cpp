// ReSharper disable CppExpressionWithoutSideEffects
#include "ParsingRules.h"

#include "Rule.h"
#include "Kleene.h"
#include "Or.h"
#include "Terminal.h"
#include "Optional.h"
#include "JvmIdentifier.h"
#include "RuleFuncs.h"


using namespace org::kapa::tarracsh::signatures;
using namespace std;


void ParsingRules::setRuleNames() {
    SET_RULE_NAME(_class);
    SET_RULE_NAME(_formalTypeParameter);
    SET_RULE_NAME(_classBound);
    SET_RULE_NAME(_interfaceBound);
    SET_RULE_NAME(_superclass);
    SET_RULE_NAME(_superinterface);
    SET_RULE_NAME(_fieldType);
    SET_RULE_NAME(_classType);
    SET_RULE_NAME(_packageSpecifier);
    SET_RULE_NAME(_simpleClassType);
    SET_RULE_NAME(_typeArguments);
    SET_RULE_NAME(_wildcardIndicator);
    SET_RULE_NAME(_typeArgument);
    SET_RULE_NAME(_classTypeSuffix);
    SET_RULE_NAME(_typeVariable);
    SET_RULE_NAME(_arrayType);
    SET_RULE_NAME(_type);
    SET_RULE_NAME(_methodType);
    SET_RULE_NAME(_returnType);
    SET_RULE_NAME(_throw);
}

void ParsingRules::initRules() {

    setRuleNames();

    _class >> 'L'
        >> (*_formalTypeParameter)
        >> _superclass
        >> *_superinterface;

    _formalTypeParameter >> _jvmIdentifier >> _classBound >> *_interfaceBound;
    _classBound >> ':' >> !_fieldType;
    _interfaceBound >> ':' >> _fieldType;
    _superclass >> _classType;
    _superinterface >> _classType;
    _fieldType >> _classType | _arrayType | _typeVariable;
    _classType >> 'L' >> !_packageSpecifier >> _simpleClassType >> *_classTypeSuffix;
    _packageSpecifier >> _jvmIdentifier >> '/' >> *_packageSpecifier;
    _simpleClassType >> _jvmIdentifier >> !_typeArguments;
    _typeArguments >> +_typeArgument;
    _wildcardIndicator >> _plusTerminal | '-';
    _typeArgument >> (_wildcardIndicator >> _fieldType) | '*';
    _classTypeSuffix >> '.' >> _simpleClassType;
    _typeVariable >> 'T' >> _jvmIdentifier;
    _arrayType >> '[' >> _type;
    _type >> _fieldType | _baseType;

    _methodType >> (*_formalTypeParameter) >> '(' >> *_type >> ')' >> _returnType >> _throw;
    _returnType >> _type | _voidDescriptor;
    _throw >> _classType | _typeVariable;

}

ParsingRules &ParsingRules::getInstance() {
    if (nullptr == instance) {
        instance = new ParsingRules();
        instance->initRules();
    }
    return *instance;
}
