#include "Kleene.h"
#include "RuleFuncs.h"


using namespace kapa::tarracsh::signatures;

Kleene::Kleene()
    : Rule(),
    _minimum(0) {
}

Kleene::Kleene(RuleVariant ruleVariant, const int minimum)
    : Rule(),
      _ruleVariant(std::move(ruleVariant)),
      _minimum(minimum) {
}

Kleene::Kleene(const Kleene &other)
    : Rule(other),
      _ruleVariant(other._ruleVariant) {
}

Kleene::Kleene(Kleene &&other) noexcept
    : Rule(std::move(other)),
      _ruleVariant(std::move(other._ruleVariant)) {
}

Kleene &Kleene::operator=(const Kleene &other) {
    if (this == &other) return *this;
    Rule::operator=(other);
    _ruleVariant = other._ruleVariant;
    return *this;
}

Kleene &Kleene::operator=(Kleene &&other) noexcept {
    if (this == &other)
        return *this;
    _ruleVariant = std::move(other._ruleVariant);
    Rule::operator =(std::move(other));
    return *this;
}



