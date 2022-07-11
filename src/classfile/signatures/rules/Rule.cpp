#include "Rule.h"
#include "Kleene.h"
#include "Optional.h"
#include "Terminal.h"
#include "Or.h"
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


RulePtr Rule::optional() {
    RulePtr rule(new Optional(*this));
    return rule;
}

KleenePtr Rule::kleenePlus() {
    KleenePtr result(new Kleene(*this, 1));
    return result;
}

KleenePtr Rule::kleeneStar() {
    KleenePtr result(new Kleene(*this, 0));
    return result;
}

namespace org::kapa::tarrash::signatures {

const RulePtr &operator>>(const RulePtr &left, const std::wstring &right) {
    const auto rule = make_shared<Terminal>(right);
    left->followBy(rule);
    return left;
}

const RulePtr &operator>>(const RulePtr &left, const wchar_t *right) {
    const std::wstring value(right);
    const auto rule = make_shared<Terminal>(value);
    left->followBy(rule);
    return left;
}

const RulePtr &operator>>(const RulePtr &left, const char right) {
    const auto terminalWChar = char2wchar(right);
    const std::wstring value{terminalWChar};
    const auto rule = make_shared<Terminal>(value);
    left->followBy(rule);
    return left;
}

RulePtr operator>>(const char left, const RulePtr &right) {
    const auto terminalWChar = char2wchar(left);
    const std::wstring value{terminalWChar};
    const auto rule = make_shared<Terminal>(value);
    rule->followBy(right);
    return rule;
}

const RulePtr &operator>>(const RulePtr &left, const RulePtr &right) {
    left->followBy(right);
    return left;
}

// RulePtr operator>>(const RulePtr &left, const TerminalPtr &right) { return left->followBy(right); }
RulePtr operator+(const RulePtr &right) { return right->kleenePlus(); }
RulePtr operator*(const RulePtr &right) { return right->kleeneStar(); }
RulePtr operator!(const RulePtr &right) { return right->optional(); }


OrPtr operator|(const RulePtr &left, const char right) {
    const auto terminalWChar = char2wchar(right);
    const std::wstring value{terminalWChar};
    const auto rule = make_shared<Terminal>(value);
    return left | rule;
}

OrPtr operator|(const RulePtr &left, const RulePtr &right) {
    const auto orRule = make_shared<Or>(left);
    return orRule | left;
}

OrPtr operator|(const OrPtr &left, const RulePtr &right) {
    left->add(right);
    return left;
}
}
