#include "Or.h"
using namespace org::kapa::tarrash::signatures;


Or::Or() = default;
Or::Or(const Or &other) = default;
Or::Or(const RulePtr &left) : Rule() { _rules.push_back(left); }


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

// bool Or::match(SignatureScanner &scanner) {
//     auto result = false;
//     for(const auto &rule: _rules) {
//         result = rule->match(scanner);
//         if (result) break;
//     }
//     return result;
// }

void Or::add(const RulePtr &rule) { _rules.push_back(rule); }
