#ifndef SIGNATURE_RULE_H
#define SIGNATURE_RULE_H
#include <memory>
#include <vector>

#include "../SignatureScanner.h"

namespace org::kapa::tarrash::signatures {


class Kleene;
class Optional;
class Rule;
class Terminal;
class Or;

typedef std::shared_ptr<Rule> RulePtr;
typedef std::shared_ptr<Optional> OptionalPtr;
typedef std::shared_ptr<Kleene> KleenePtr;
typedef std::shared_ptr<Terminal> TerminalPtr;
typedef std::shared_ptr<Or> OrPtr;

class Rule {
public:
    explicit Rule(SignatureScanner &scanner);
    Rule(const Rule &other);
    Rule(Rule &&other) noexcept;
    Rule &operator=(const Rule &other);
    Rule &operator=(Rule &&other) noexcept;
    virtual ~Rule();
    virtual bool match();
    void parse();

    template <typename T>
    RulePtr followBy(const std::shared_ptr<T> rule) {
        _followBy.push_back(rule);
        auto result = RulePtr(this);
        return result;
    }

    RulePtr optional();
    RulePtr kleenePlus();
    RulePtr kleeneStar();
    SignatureScanner &getScanner() { return _scanner; }
    

protected:
    SignatureScanner _scanner;
    std::vector<RulePtr> _followBy;
    bool _optional = false;

    // Rule &followBy(Kleene& rule);
    // Rule &followBy(Optional& rule);
    // OptionalPtr optional();
    // KleenePtr kleenePlus();
    // KleenePtr kleeneStar();

};

RulePtr operator>>(const RulePtr &left, const std::wstring& right );
RulePtr operator>>(const RulePtr &left, const wchar_t* right );
RulePtr operator>>(const RulePtr &left, const char right );
RulePtr operator>>(const char left, const RulePtr &right);
RulePtr operator>>(const RulePtr &left, const RulePtr &right);
RulePtr operator+(const RulePtr &right);
RulePtr operator!(const RulePtr &right);
RulePtr operator*(const RulePtr &right);
OrPtr operator|(const RulePtr &left, const RulePtr& right );
OrPtr operator|(const RulePtr &left, const char right);
OrPtr operator|(const OrPtr &left, const RulePtr &right);
// RulePtr operator+>>(const RulePtr &left, const Optional &right);
// RulePtr operator>>(const RulePtr &left, const Kleene &right);
// OptionalPtr operator!();
// KleenePtr operator+();
// KleenePtr operator*();
} // namespace org::kapa::tarrash::signatures
#endif
