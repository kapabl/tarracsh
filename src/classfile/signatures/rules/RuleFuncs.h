#ifndef INVOKEMATCH_H
#define INVOKEMATCH_H

#include "Rule.h"
#include "Kleene.h"
#include "Or.h"
#include "Terminal.h"
#include "Optional.h"
#include "JvmIdentifier.h"

namespace org::kapa::tarrash::signatures {

template <typename T>
bool invokeMatch(RuleVariant &ruleVariant, SignatureScanner &scanner, T &node) {
    bool result = false;
    if (std::holds_alternative<Kleene>(ruleVariant)) {
        result = std::get<Kleene>(ruleVariant).match(scanner, node);
    } else if (std::holds_alternative<Rule>(ruleVariant)) {
        result = std::get<Rule>(ruleVariant).match(scanner, node);
    } else if (std::holds_alternative<Or>(ruleVariant)) {
        result = std::get<Or>(ruleVariant).match(scanner, node);
    } else if (std::holds_alternative<Optional>(ruleVariant)) {
        result = std::get<Optional>(ruleVariant).match(scanner, node);
    } else if (std::holds_alternative<Terminal>(ruleVariant)) {
        result = std::get<Terminal>(ruleVariant).match(scanner, node);
    } else if (std::holds_alternative<JvmIdentifier>(ruleVariant)) {
        result = std::get<JvmIdentifier>(ruleVariant).match(scanner, node);
    }
    return result;
}

template <typename T>
bool invokeMatch(Rule& rule, SignatureScanner& scanner, T& node) {
    bool result = rule.match( scanner, node );
    return result;
}
template <typename TLeft, typename TRight>
void followBy(TLeft &left, TRight &right) {
    RuleVariant ruleVariant;
    ruleVariant = right;
    left.followByRuleVariant(ruleVariant);
}


Rule &operator>>(Rule &left, const std::wstring &right);
Rule &operator>>(Rule &left, const wchar_t *right);
Rule &operator>>(Rule &left, char right);
Terminal operator>>(const char left, const Rule &right);
Rule &operator>>(Rule &left, const Rule &right);
Kleene operator+(Rule &right);
Optional operator!(Rule &right);
Kleene operator*(Rule &right);
Or operator|(Rule &left, const Rule &right);
Or operator|(Rule &left, char right);
Or operator|(Or left, const Rule &right);
Or operator|(Or left, const Terminal &right);

}
#endif
