// ReSharper disable CppExpressionWithoutSideEffects
#include "ParsingRules.h"

#include "Rule.h"


using namespace org::kapa::tarrash::signatures;
using namespace std;


void ParsingRules::initRules() {

    _class >> 'L'
        >> !(+_formalTypeParameter)
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
    _methodType >> !(+_formalTypeParameter) >> (*_type) >> _returnType >> _throw;
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
