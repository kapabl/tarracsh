#include "Or.h"
using namespace org::kapa::tarrash::signatures;

Or::Or(const RulePtr &left): Rule( left->getScanner()) { _rules.push_back(left); }


Or::Or(SignatureScanner &scanner)
    : Rule(scanner) {
}

Or::Or(const Or &other) = default;


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

bool Or::match() {
    //TODO
    return Rule::match();
}

void Or::add(const RulePtr &rule) { _rules.push_back(rule); }
