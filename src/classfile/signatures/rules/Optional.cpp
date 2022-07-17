
#include "Rule.h"
#include "Optional.h"
#include "Kleene.h"
#include "RuleFuncs.h"

using namespace org::kapa::tarracsh::signatures;

Optional::Optional() = default;

Optional::Optional(Rule rule)
    : Rule(),
      _rule(std::move(rule)) {
}

Optional::Optional(const Optional &other) = default;

Optional::Optional(Optional &&other) noexcept
    : Rule(std::move(other)),
      _rule(std::move(other._rule)) {
}

Optional &Optional::operator=(const Optional &other) {
    if (this == &other)
        return *this;
    Rule::operator =(other);
    _rule = other._rule;
    return *this;
}

Optional &Optional::operator=(Optional &&other) noexcept {
    if (this == &other)
        return *this;
    Rule::operator =(std::move(other));
    _rule = std::move(other._rule);
    return *this;
}

// bool Optional::match(SignatureScanner &scanner) {
//     auto innerRuleMatched = _rule.match(scanner);
//     return true;
// }

