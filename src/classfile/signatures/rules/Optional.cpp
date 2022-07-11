
#include "Rule.h"
#include "Optional.h"
#include "Kleene.h"

using namespace org::kapa::tarrash::signatures;

Optional::Optional(const Rule &rule)
    : Rule(),
      _rule(rule) {
}

Optional::Optional(const Optional &other)
    : Rule(other),
      _rule(other._rule) {
}

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

