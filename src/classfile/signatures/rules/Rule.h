#ifndef SIGNATURE_RULE_H
#define SIGNATURE_RULE_H
#include <memory>
#include <vector>
#include <variant>

#include "../SignatureScanner.h"
#include "visit_struct/visit_struct.hpp"

namespace org::kapa::tarrash::signatures {

class Rule;
class Or;
class Kleene;
class Optional;
class Rule;
class Terminal;
class JvmIdentifier;


#ifdef _DEBUG
#define SET_RULE_NAME( rule ) rule.setName( #rule );
#define GET_RULE_NAME( rule ) rule.getName();
#else
#define SET_RULE_NAME
#define GET_RULE_NAME
#endif

typedef std::variant<
    Rule,
    Kleene,
    Terminal,
    JvmIdentifier,
    Optional,
    Or>
RuleVariant;


class Rule {
public:
    explicit Rule();
    Rule(const Rule &other);
    Rule(Rule &&other) noexcept;
    Rule &operator=(const Rule &other);
    Rule &operator=(Rule &&other) noexcept;
    virtual ~Rule();


    template <typename T>
    bool match(SignatureScanner &scanner, T &node) {

        auto index = 0u;
        bool result = true;

        visit_struct::for_each(node, [this, &index, &result, &scanner](const char *name, auto &value) {
            if (!result) return;
            auto &rule = _followBy[index];
            result = !invokeMatch(rule, scanner, value);
            index++;
        });
        return result;
    }

    template <typename T>
    bool match(SignatureScanner &scanner, std::shared_ptr<T> &node) {
        node.reset(new T());
        auto result = match(scanner, *node);
        return result;
    }

    template <>
    bool match<std::wstring>(SignatureScanner &scanner, std::wstring &value) {
        return false;
    }


    template <typename T>
    bool match(SignatureScanner &scanner, std::vector<T> &list) {
        return false;
    }


    void parse(SignatureScanner &scanner);

    // template <typename T>
    // void followBy(const T rule) {
    //     RuleVariant ruleVariant;
    //     ruleVariant = rule;
    //     _followBy.push_back(ruleVariant);
    // }

    template <typename T>
    void followByRuleVariant(const T& ruleVariant) {
        _followBy.push_back(ruleVariant);
    }

    Optional optional();
    Kleene kleenePlus();
    Kleene kleeneStar();

#ifdef _DEBUG
    void setName(const std::string &name) {
        _name = name;
    }

    std::string getName() const { return _name; }
#endif


protected:
    std::vector<RuleVariant> _followBy;
#ifdef _DEBUG
    std::string _name;
#endif


};

}


#endif
