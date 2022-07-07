#include "Rule.h"
#include "Kleene.h"

using namespace org::kapa::tarrash::signatures;

Kleene::Kleene(Rule &rule, SignatureScanner &scanner, const int minimum)
    : Rule(scanner),
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

bool Kleene::match() {
    //TODO
    return true;
}

