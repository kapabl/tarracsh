#include "Kleene.h"
#include "RuleFuncs.h"


using namespace org::kapa::tarracsh::signatures;

Kleene::Kleene()
    : Rule(),
    _minimum(0) {
}

Kleene::Kleene(Rule &rule, const int minimum)
    : Rule(),
      _rule(rule),
      _minimum(minimum) {
}

Kleene::Kleene(const Kleene &other)
    : Rule(other),
      _rule(other._rule) {
}

Kleene::Kleene(Kleene &&other) noexcept
    : Rule(std::move(other)),
      _rule(std::move(other._rule)) {
}

Kleene &Kleene::operator=(const Kleene &other) {
    if (this == &other) return *this;
    Rule::operator=(other);
    _rule = other._rule;
    return *this;
}

Kleene &Kleene::operator=(Kleene &&other) noexcept {
    if (this == &other)
        return *this;
    _rule = std::move(other._rule);
    Rule::operator =(std::move(other));
    return *this;
}

// bool Kleene::match(SignatureScanner &scanner) {
//     auto matchedCount = 0;
//     while (_rule.match(scanner)) {
//         matchedCount++;
//     }
//
//     auto result = matchedCount >= _minimum;
//
//     return true;
// }


