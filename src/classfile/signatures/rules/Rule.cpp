#include "Rule.h"
#include "Kleene.h"
#include "Optional.h"
#include "Or.h"
#include "RuleFuncs.h"
#include "../../../utils/StringUtils.h"


using namespace org::kapa::tarracsh::signatures;
using namespace org::kapa::tarracsh::stringUtils;
using namespace std;

Rule::Rule() = default;
Rule::Rule(const Rule &other) = default;
Rule::Rule(Rule &&other) noexcept = default;
Rule::Rule(const bool isAnchor): _isAnchor( isAnchor ) {}


Rule &Rule::operator=(const Rule &other) {
    if (this == &other) { return *this; }
    _followBy = other._followBy;
    _name = other._name;
    _capture = other._capture;
    _isAnchor = other._isAnchor;
    return *this;
}

Rule &Rule::operator=(Rule &&other) noexcept {
    if (this == &other) { return *this; }
    _followBy = std::move(other._followBy);
    _name = std::move(other._name);
    _capture = other._capture;
    _isAnchor = other._isAnchor;
    return *this;
}

Rule::~Rule() = default;

bool Rule::match(SignatureScanner &scanner, std::wstring &value) {
    auto result = false;
    DEBUG_RULE(this);
    result = followByMatch(scanner, value);
    return result;
}


// bool Rule::match(SignatureScanner &scanner) {
//     // NOLINT(misc-no-recursion)
//     bool result = true;
//     for (const auto &rule : _followBy) {
//         result = rule->match(scanner);
//         if (!result) break;
//     }
//     return result;
// }


namespace org::kapa::tarracsh::signatures {




}
