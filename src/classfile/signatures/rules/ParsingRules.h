#ifndef TARRACSH_PARSING_RULES_H
#define TARRACSH_PARSING_RULES_H

#include "Rule.h"
#include "Or.h"
#include "Terminal.h"
#include "JvmIdentifier.h"
#include <memory>

namespace org::kapa::tarracsh::signatures {

class ParsingRules {


protected:
    ParsingRules() = default;

    inline static ParsingRules *instance = nullptr;

public:
    ParsingRules(ParsingRules &other) = delete;
    void operator=(const ParsingRules &) = delete;

    static ParsingRules &getInstance();

    RulePtr &getClassRule() { return _class; }
    RulePtr &getMethodRule() { return _methodType; }
    OrPtr &getFieldRule() { return _fieldType; }

private:
    void initRules();

    RulePtr _class = std::make_shared<Rule>();
    OrPtr _fieldType = std::make_shared<Or>();
    RulePtr _methodType = std::make_shared<Rule>();

};


}

#endif
