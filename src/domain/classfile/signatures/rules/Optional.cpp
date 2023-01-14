
#include "Rule.h"
#include "Optional.h"
#include "Kleene.h"
#include "RuleFuncs.h"

using namespace kapa::tarracsh::signatures;

Optional::Optional() = default;

Optional::Optional(const RuleVariant& ruleVariant)
    : Rule(),
      _ruleVariant(ruleVariant) {
}

Optional::Optional(const Optional &other) = default;

Optional::Optional(Optional &&other) noexcept
    : Rule(std::move(other)),
      _ruleVariant(std::move(other._ruleVariant)) {
}

Optional &Optional::operator=(const Optional &other) {
    if (this == &other)
        return *this;
    Rule::operator =(other);
    _ruleVariant = other._ruleVariant;
    return *this;
}

Optional &Optional::operator=(Optional &&other) noexcept {
    if (this == &other)
        return *this;
    Rule::operator =(std::move(other));
    _ruleVariant = std::move(other._ruleVariant);
    return *this;
}

bool Optional::match(SignatureScanner &scanner, std::wstring &value) {
    auto matchResult = invokeMatch<std::wstring>(_ruleVariant, scanner, value);
    return true;
}

// bool Optional::match(SignatureScanner &scanner) {
//     auto innerRuleMatched = _ruleVariant.match(scanner);
//     return true;
// }

