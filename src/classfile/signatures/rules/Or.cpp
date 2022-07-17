#include "Rule.h"
#include "Or.h"
#include "RuleFuncs.h"
using namespace org::kapa::tarracsh::signatures;


Or::Or() = default;
Or::Or(const Or &other) = default;
Or::Or(const Rule &left) : Rule() { _rules.emplace_back(left); }


Or::Or(Or &&other) noexcept: Rule(std::move(other)) {
}

Or & Or::operator=(const Or &other) {
    if (this == &other)
        return *this;
    Rule::operator =(other);
    return *this;
}

Or & Or::operator=(Or &&other) noexcept {
    if (this == &other)
        return *this;
    Rule::operator =(std::move(other));
    return *this;
}

void Or::add(const Rule &rule) { _rules.emplace_back(rule); }
