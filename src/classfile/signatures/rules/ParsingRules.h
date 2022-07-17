#ifndef TARRASCH_PARSING_RULES_H
#define TARRASCH_PARSING_RULES_H

#include "Rule.h"
#include "Terminal.h"
#include "JvmIdentifier.h"
#include <memory>

namespace org::kapa::tarrash::signatures {

class ParsingRules {


protected:
    ParsingRules() = default;

    inline static ParsingRules *instance = nullptr;

public:
    ParsingRules(ParsingRules &other) = delete;
    void operator=(const ParsingRules &) = delete;

    static ParsingRules &getInstance();

    Rule& getClassRule() { return _class; }
    Rule& getMethodRule() { return _methodType; }
    Rule& getFieldRule() { return _fieldType; }

private:
    void initRules();
    void setRuleNames();

    JvmIdentifier _jvmIdentifier;
    Rule _class;
    Rule _formalTypeParameter;
    Rule _superclass;
    Rule _superinterface;
    Rule _classBound;
    Rule _interfaceBound;
    Rule _fieldType;
    Rule _classType;
    Rule _arrayType;
    Rule _typeVariable;
    Rule _packageSpecifier;
    Rule _simpleClassType;
    Rule _classTypeSuffix;
    Rule _typeArgument;
    Rule _typeArguments;
    Rule _wildcardIndicator;
    Rule _type;
    Rule _baseType;

    Terminal _plusTerminal;
    Terminal _voidDescriptor;

    Rule _methodType;
    Rule _returnType;
    Rule _throw;
};


}

#endif
