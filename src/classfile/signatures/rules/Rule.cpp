#include "Rule.h"
#include "Kleene.h"
#include "Optional.h"
#include "Terminal.h"
#include "JvmIdentifier.h"
#include "Or.h"
#include "RuleFuncs.h"
#include "../../StringUtils.h"



using namespace org::kapa::tarrash::signatures;
using namespace org::kapa::tarrash::stringUtils;
using namespace std;

Rule::Rule() = default;

Rule::Rule(const Rule &other) {
}

Rule::Rule(Rule &&other) noexcept {
    _followBy = std::move(other._followBy);
}

Rule &Rule::operator=(const Rule &other) {
    if (this == &other) { return *this; }
    _followBy = other._followBy;
    return *this;
}

Rule &Rule::operator=(Rule &&other) noexcept {
    if (this == &other) { return *this; }
    _followBy = std::move(other._followBy);
    return *this;
}

Rule::~Rule() = default;


// bool Rule::match(SignatureScanner &scanner) {
//     // NOLINT(misc-no-recursion)
//     bool result = true;
//     for (const auto &rule : _followBy) {
//         result = rule->match(scanner);
//         if (!result) break;
//     }
//     return result;
// }

void Rule::parse(SignatureScanner &scanner) {
    //TODO
}


Optional Rule::optional() {
    Optional result(*this);
    return result;
}

Kleene Rule::kleenePlus() {
    Kleene result(*this, 1);
    return result;
}

Kleene Rule::kleeneStar() {
    Kleene result(*this, 0);
    return result;
}

/*
OptionalPtr Rule::optional()  {
    OptionalPtr result(new Optional(*this));
    return result;
}

KleenePtr Rule::kleenePlus()  {
    KleenePtr result( new Kleene(*this, 1) );
    return result;
}

KleenePtr Rule::kleeneStar() {
    KleenePtr result(new Kleene(*this, 0));
    return result;
}*/

namespace org::kapa::tarrash::signatures {

Rule &operator>>(Rule &left, const std::wstring &right) {
    const auto rule = Terminal(right);
    // left.followBy(rule);
    followBy(left, rule);
    return left;
}

Rule &operator>>(Rule &left, const wchar_t *right) {
    const std::wstring value(right);
    Terminal rule(value);
    // left.followBy(rule);
    followBy(left, rule);
    return left;
}

Rule &operator>>(Rule &left, const char right) {
    const auto terminalWChar = char2wchar(right);
    const std::wstring value{terminalWChar};
    Terminal rule(value);
    // left.followBy(rule);
    followBy(left, rule);
    return left;
}

Terminal operator>>(const char left, const Rule &right) {
    const auto terminalWChar = char2wchar(left);
    const std::wstring value{terminalWChar};
    auto result = Terminal(value);
    // result.followBy(right);
    followBy(result, right);
    return result;
}

Rule &operator>>(Rule &left, const Rule &right) {
    // left.followBy(right);
    followBy(left, right);
    return left;
}

// Rule operator>>(const Rule &left, const Terminal &right) { return left->followBy(right); }
Kleene operator+(Rule &right) { return right.kleenePlus(); }
Kleene operator*(Rule &right) { return right.kleeneStar(); }
Optional operator!(Rule &right) { return right.optional(); }


Or operator|(Rule &left, const char right) {
    const auto terminalWChar = char2wchar(right);
    const std::wstring value{terminalWChar};
    Terminal rule(value);
    return left | rule;
}

Or operator|(Rule &left, const Rule &right) {
    auto orRule = Or();
    return orRule | left;
}

Or operator|(Or left, const Terminal& right) {
    left.add(right);
    return left;
}

Or operator|(Or left, const Rule &right) {
    left.add(right);
    return left;
}

}
