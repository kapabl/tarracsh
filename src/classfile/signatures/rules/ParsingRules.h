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

    RulePtr getClassRule() { return _class; }
    RulePtr getMethodRule() { return _methodType; }
    RulePtr getFieldRule() { return _fieldType; }

private:
    void initRules();
    JvmIdentifierPtr _jvmIdentifier = std::make_shared<JvmIdentifier>();
    RulePtr _class = std::make_shared<Rule>();
    RulePtr _formalTypeParameter = std::make_shared<Rule>();
    RulePtr _superclass = std::make_shared<Rule>();
    RulePtr _superinterface = std::make_shared<Rule>();
    RulePtr _classBound = std::make_shared<Rule>();
    RulePtr _interfaceBound = std::make_shared<Rule>();
    RulePtr _fieldType = std::make_shared<Rule>();
    RulePtr _classType = std::make_shared<Rule>();
    RulePtr _arrayType = std::make_shared<Rule>();
    RulePtr _typeVariable = std::make_shared<Rule>();
    RulePtr _packageSpecifier = std::make_shared<Rule>();
    RulePtr _simpleClassType = std::make_shared<Rule>();
    RulePtr _classTypeSuffix = std::make_shared<Rule>();
    RulePtr _typeArgument = std::make_shared<Rule>();
    RulePtr _typeArguments = std::make_shared<Rule>();
    RulePtr _wildcardIndicator = std::make_shared<Rule>();
    RulePtr _type = std::make_shared<Rule>();
    RulePtr _baseType = std::make_shared<Rule>();

    TerminalPtr _plusTerminal = std::make_shared<Terminal>(L"+");
    TerminalPtr _voidDescriptor = std::make_shared<Terminal>(L"V");

    RulePtr _methodType = std::make_shared<Rule>();
    RulePtr _returnType = std::make_shared<Rule>();
    RulePtr _throw = std::make_shared<Rule>();
};


}

#endif
