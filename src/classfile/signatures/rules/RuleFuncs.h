#ifndef INVOKEMATCH_H
#define INVOKEMATCH_H

#include "Rule.h"
#include "Kleene.h"
#include "Or.h"
#include "Terminal.h"
#include "Optional.h"
#include "JvmIdentifier.h"
#include "../../StringUtils.h"

namespace org::kapa::tarracsh::signatures {

template <typename T>
std::pair<bool, bool> invokeMatch(RuleVariant &ruleVariant, SignatureScanner &scanner, T &node) {
    auto result = std::make_pair(false, false);
    if (std::holds_alternative<KleenePtr>(ruleVariant)) {
        const auto rule = std::get<KleenePtr>(ruleVariant);
        result.first = rule->match(scanner, node);
        result.second = rule->getCapture();
    } else if (std::holds_alternative<RulePtr>(ruleVariant)) {
        const auto rule = std::get<RulePtr>(ruleVariant);
        result.first = rule->match(scanner, node);
        result.second = rule->getCapture();
    } else if (std::holds_alternative<OrPtr>(ruleVariant)) {
        const auto rule = std::get<OrPtr>(ruleVariant);
        result.first = rule->match(scanner, node);
        result.second = rule->getCapture();
    } else if (std::holds_alternative<OptionalPtr>(ruleVariant)) {
        const auto rule = std::get<OptionalPtr>(ruleVariant);
        result.first = rule->match(scanner, node);
        result.second = rule->getCapture();
    } else if (std::holds_alternative<TerminalPtr>(ruleVariant)) {
        const auto rule = std::get<TerminalPtr>(ruleVariant);
        result.second = rule->getCapture();
        result.first = rule->match(scanner, node);
    } else if (std::holds_alternative<JvmIdentifierPtr>(ruleVariant)) {
        const auto rule = std::get<JvmIdentifierPtr>(ruleVariant);
        result.first = rule->match(scanner, node);
        result.second = rule->getCapture();
    }

    return result;
}


inline TerminalPtr makeTerminal(const std::wstring &value, bool capture = false) {
    auto rule = std::make_shared<Terminal>(value);
    rule->setCapture(capture);
    SET_RULE_NAME2(rule, std::format(L"terminal-{}", value));
    return rule;
}


template <typename TLeft, typename TRight>
RulePtr ruleFollowRule(TLeft &left, const TRight &right) {
    const RuleVariant leftRuleVariant(left);
    const RuleVariant rightRuleVariant(right);

    RulePtr result;

    if (left->isAnchor()) {
        left->followByRuleVariant(rightRuleVariant);
        result = left;
    } else {
        result = std::make_shared<Rule>(true);
        result->followByRuleVariant(leftRuleVariant);
        result->followByRuleVariant(rightRuleVariant);
    }

    return result;
}

template <typename T>
T operator>>(T left, const std::wstring &right) {
    const auto rightRule = makeTerminal(right);
    auto result = ruleFollowRule(left, rightRule);
    return result;

}

template <typename T>
T operator>>(T left, const wchar_t *right) {
    const std::wstring value(right);
    const auto rightRule = makeTerminal(value);
    return ruleFollowRule(left, rightRule);
}

template <typename T>
RulePtr operator>>(T left, char right) {
    const auto terminalWChar = stringUtils::char2wchar(right);
    const std::wstring value{terminalWChar};
    const auto rightRule = makeTerminal(value);
    return ruleFollowRule(left, rightRule);
}

template <typename T>
RulePtr operator>>(const std::wstring &left, T right) {
    auto leftRule = makeTerminal(left);
    auto result = ruleFollowRule(leftRule, right);
    return result;
}

template <typename T>
RulePtr operator>>(const wchar_t *left, T right) {
    const std::wstring value(left);
    auto leftRule = makeTerminal(value);
    auto result = ruleFollowRule(leftRule, right);
    return result;
}

template <typename T>
RulePtr operator>>(char left, T right) {
    const auto terminalWChar = stringUtils::char2wchar(left);
    const std::wstring value{terminalWChar};
    auto leftRule = makeTerminal(value);
    auto result = ruleFollowRule(leftRule, right);
    return result;
}

inline RulePtr operator>>(const char left, const RulePtr &right) {
    const auto terminalWChar = stringUtils::char2wchar(left);
    const std::wstring value{terminalWChar};
    auto leftRule = makeTerminal(value);
    auto result = ruleFollowRule(leftRule, right);
    return result;
}


template <typename T, typename T1>
RulePtr operator>>(T left, const T1 &right) {
    auto result = ruleFollowRule(left, right);
    return result;
}

template <typename T>
KleenePtr kleeneOper(T &rule, int minimum) {
    auto result = make_shared<Kleene>(RuleVariant(rule), minimum);
    SET_RULE_NAME2(result, std::format(L"k({}-{})", minimum, GET_RULE_NAME( rule ) ));
    return result;
}


inline KleenePtr operator+(const RulePtr &right) { return kleeneOper(right, 1); }
inline KleenePtr operator+(const OrPtr &right) { return kleeneOper(right, 1); }
inline KleenePtr operator+(const TerminalPtr &right) { return kleeneOper(right, 1); }
inline KleenePtr operator+(const JvmIdentifierPtr &right) { return kleeneOper(right, 1); }

inline KleenePtr operator*(const RulePtr &right) { return kleeneOper(right, 0); }
inline KleenePtr operator*(const OrPtr &right) { return kleeneOper(right, 0); }
inline KleenePtr operator*(const TerminalPtr &right) { return kleeneOper(right, 0); }
inline KleenePtr operator*(const JvmIdentifierPtr &right) { return kleeneOper(right, 0); }


template <typename T>
OptionalPtr optionalOper(T &rule) {
    auto result = make_shared<Optional>(RuleVariant(rule));
    SET_RULE_NAME2(result, std::format(L"optional({})", GET_RULE_NAME( rule )));
    return result;
}


inline OptionalPtr operator!(RulePtr right) { return optionalOper(right); }
inline OptionalPtr operator!(KleenePtr right) { return optionalOper(right); }
inline OptionalPtr operator!(OrPtr right) { return optionalOper(right); }
inline OptionalPtr operator!(TerminalPtr right) { return optionalOper(right); }
inline OptionalPtr operator!(JvmIdentifierPtr right) { return optionalOper(right); }


template <typename TRight>
inline OrPtr orTRuleWithRight(const OrPtr& left, TRight& right) {
    OrPtr result;
    if (left->isAnchor()) {
        result = left;
    }
    else {
        result = std::make_shared<Or>(true);
        result->addToOr(left);

    }
    result->addToOr(right);

    return result;
}

template <typename T>
OrPtr operator|(OrPtr left, const T &right) {
    // OrPtr result;
    // if (left->isAnchor()) {
    //     result = left;
    // } else {
    //     result = std::make_shared<Or>();
    //     result->addToOr(left);
    // }
    //
    // result->addToOr(right);
    // return result;
    return orTRuleWithRight(left, right);
}

inline OrPtr orWithWCharString(const OrPtr &left, const wchar_t *right) {
    const std::wstring value(right);
    const auto rightRule = makeTerminal(value);
    return left | rightRule;
}

inline OrPtr orWithChar(const OrPtr &left, const char right) {
    const auto terminalWChar = stringUtils::char2wchar(right);
    const std::wstring value{terminalWChar};
    return orWithWCharString(left, value.c_str());
}


template <typename TRule>
inline OrPtr orTRuleWithChar(const TRule &left, const char right) {
    const auto terminalWChar = stringUtils::char2wchar(right);
    const std::wstring value{terminalWChar};
    return orTRuleWithWCharString(left, value.c_str());
}

template <typename TRule>
inline OrPtr orTRuleWithWCharString(const TRule &left, const wchar_t *right) {
    const std::wstring value(right);
    const auto rightRule = makeTerminal(value);
    auto result = std::make_shared<Or>(true);
    result->addToOr(left);
    result->addToOr(rightRule);
    return result;
}

template <typename TRule, typename TRight>
inline OrPtr orTRuleWithRight(const TRule &left, TRight &right) {

    auto result = std::make_shared<Or>(true);
    result->addToOr(left);
    result->addToOr(right);

    return result;
}


template <typename T> OrPtr operator|(RulePtr left, const T &right) { return orTRuleWithRight(left, right); }
template <typename T> OrPtr operator|(KleenePtr left, const T &right) { return orTRuleWithRight(left, right); }
template <typename T> OrPtr operator|(OptionalPtr left, const T &right) { return orTRuleWithRight(left, right); }
template <typename T> OrPtr operator|(TerminalPtr left, const T &right) { return orTRuleWithRight(left, right); }
template <typename T> OrPtr operator|(JvmIdentifierPtr left, const T &right) { return orTRuleWithRight(left, right); }


inline OrPtr operator|(const RulePtr &left, const char right) { return orTRuleWithChar(left, right); }
inline OrPtr operator|(const RulePtr &left, const wchar_t *right) { return orTRuleWithWCharString(left, right); }

inline OrPtr operator|(const TerminalPtr &left, const char right) { return orTRuleWithChar(left, right); }
inline OrPtr operator|(const TerminalPtr &left, const wchar_t *right) { return orTRuleWithWCharString(left, right); }

inline OrPtr operator|(const KleenePtr &left, const char right) { return orTRuleWithChar(left, right); }
inline OrPtr operator|(const KleenePtr &left, const wchar_t *right) { return orTRuleWithWCharString(left, right); }

inline OrPtr operator|(const OrPtr &left, const char right) { return orWithChar(left, right); }
inline OrPtr operator|(const OrPtr &left, const wchar_t *right) { return orWithWCharString(left, right); }

inline OrPtr operator|(const OptionalPtr &left, const char right) { return orTRuleWithChar(left, right); }
inline OrPtr operator|(const OptionalPtr &left, const wchar_t *right) { return orTRuleWithWCharString(left, right); }


}
#endif
