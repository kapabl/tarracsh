#include "Rule.h"
#include "Kleene.h"
#include "Optional.h"
#include "Terminal.h"
#include "Or.h"
#include "../../StringUtils.h"

using namespace org::kapa::tarrash::signatures;
using namespace org::kapa::tarrash::stringUtils;
using namespace std;

Rule::Rule(SignatureScanner &scanner)
    : _scanner(scanner) {

}

Rule::Rule(const Rule &other)
    : _scanner(other._scanner) {
}

Rule::Rule(Rule &&other) noexcept
    : _scanner(std::move(other._scanner)) {
}

Rule &Rule::operator=(const Rule &other) {
    if (this == &other)
        return *this;
    _scanner = other._scanner;
    return *this;
}

Rule &Rule::operator=(Rule &&other) noexcept {
    if (this == &other)
        return *this;
    _scanner = std::move(other._scanner);
    return *this;
}

Rule::~Rule() = default;


bool Rule::match() {
    bool result = true;
    for (auto &rule : _followBy) {
        result = rule->match();
        if (!result) break;
    }
    return result;
}

void Rule::parse() {
    //TODO
}


// RulePtr Rule::followBy(const RulePtr &rule) {
//     _followBy.push_back(rule);
//     auto sharedPtr = shared_ptr<Rule>(this);
//     return sharedPtr;
// }

RulePtr Rule::optional() {
    RulePtr rule(new Optional(*this, _scanner));
    return rule;
}

RulePtr Rule::kleenePlus() {
    KleenePtr result(new Kleene(*this, _scanner, 1));
    return result;
}

RulePtr Rule::kleeneStar() {
    KleenePtr result(new Kleene(*this, _scanner, 0));
    return result;
}

namespace org::kapa::tarrash::signatures {

RulePtr operator>>(const RulePtr &left, const std::wstring &right) {
    const auto rule = make_shared<Terminal>(right, left->getScanner());
    return left->followBy(rule);
}

RulePtr operator>>(const RulePtr &left, const wchar_t * right) {
    const std::wstring value(right);
    const auto rule = make_shared<Terminal>(value, left->getScanner());
    return left->followBy(rule);
}

RulePtr operator>>(const RulePtr &left, const char right) {
    const auto terminalWChar = char2wchar(right);
    const std::wstring value{terminalWChar};
    const auto rule = make_shared<Terminal>(value, left->getScanner());
    return left->followBy(rule);
}

RulePtr operator>>(const char left, const RulePtr &right ) {
    const auto terminalWChar = char2wchar(left);
    const std::wstring value{terminalWChar};
    const auto rule = make_shared<Terminal>(value, right->getScanner());
    return rule->followBy(right);
}

RulePtr operator>>(const RulePtr &left, const RulePtr &right) { return left->followBy(right); }
RulePtr operator>>(const RulePtr &left, const TerminalPtr &right) { return left->followBy(right); }
RulePtr operator+(const RulePtr &right) { return right->kleenePlus(); }
RulePtr operator*(const RulePtr &right) { return right->kleeneStar(); }
RulePtr operator!(const RulePtr &right) { return right->optional(); }


OrPtr operator|(const RulePtr &left, const char right) {
    const auto terminalWChar = char2wchar(right);
    const std::wstring value{terminalWChar};
    const auto rule = make_shared<Terminal>(value, left->getScanner());
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
