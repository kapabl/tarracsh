#ifndef SIGNATURE_RULE_H
#define SIGNATURE_RULE_H
#include <memory>
#include <vector>
#include <concepts>
#include <variant>

#include "../SignatureScanner.h"
#include "visit_struct/visit_struct.hpp"

namespace org::kapa::tarrash::signatures {

class Rule;
class Kleene;
class Optional;
class Rule;
class Terminal;
class Or;

using RulePtr = std::shared_ptr<Rule>;
using OptionalPtr = std::shared_ptr<Optional>;
using KleenePtr = std::shared_ptr<Kleene>;
using TerminalPtr = std::shared_ptr<Terminal>;
using OrPtr = std::shared_ptr<Or>;


class Rule {
public:
    explicit Rule();
    Rule(const Rule &other);
    Rule(Rule &&other) noexcept;
    Rule &operator=(const Rule &other);
    Rule &operator=(Rule &&other) noexcept;
    virtual ~Rule();
    // virtual bool match(SignatureScanner &scanner);

    template <typename TVisitable>
    bool match(SignatureScanner &scanner, TVisitable &output);

    void parse(SignatureScanner &scanner);

    template <typename T>
    void followBy(const std::shared_ptr<T> rule) {
        _followBy.push_back(rule);
    }

    RulePtr optional();
    KleenePtr kleenePlus();
    KleenePtr kleeneStar();

    typedef std::variant<
        RulePtr,
        KleenePtr,
        OrPtr,
        TerminalPtr,
        OptionalPtr>
    RuleVariants;


protected:
    // std::vector<RulePtr> _followBy;
    std::vector<RuleVariants> _followBy;


};




const RulePtr &operator>>(const RulePtr &left, const std::wstring &right);
const RulePtr &operator>>(const RulePtr &left, const wchar_t *right);
const RulePtr &operator>>(const RulePtr &left, char right);
RulePtr operator>>(char left, const RulePtr &right);
const RulePtr &operator>>(const RulePtr &left, const RulePtr &right);
RulePtr operator+(const RulePtr &right);
RulePtr operator!(const RulePtr &right);
RulePtr operator*(const RulePtr &right);
OrPtr operator|(const RulePtr &left, const RulePtr &right);
OrPtr operator|(const RulePtr &left, char right);
OrPtr operator|(const OrPtr &left, const RulePtr &right);

}

#include "Kleene.h"
#include "Or.h"
#include "Terminal.h"
#include "Optional.h"
template <typename TVisitable> bool org::kapa::tarrash::signatures::Rule::match(SignatureScanner &scanner, TVisitable &output) {
    bool result = true;

    auto index = 0u;

    visit_struct::for_each(output, [this, &index,&result, &scanner](const char *name, const auto &value) {
        const auto &rule = _followBy[index];
        //rule.get()
        //result = rule->match(scanner, value );
        if (std::holds_alternative<KleenePtr>(rule)) {
            std::get<KleenePtr>(rule)->match(scanner, value);
        } else if (std::holds_alternative<RulePtr>(rule)) {
            std::get<RulePtr>(rule)->match(scanner, value);
        }
        else if (std::holds_alternative<OrPtr>(rule)) {
            std::get<OrPtr>(rule)->match(scanner, value);
        }
        else if (std::holds_alternative<TerminalPtr>(rule)) {
            std::get<TerminalPtr>(rule)->match(scanner, value);
        }
        else if (std::holds_alternative<OptionalPtr>(rule)) {
            std::get<OptionalPtr>(rule)->match(scanner, value);
        }
        index++;
    });
    return result;
}
#endif
