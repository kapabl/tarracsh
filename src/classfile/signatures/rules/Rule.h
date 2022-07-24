#ifndef SIGNATURE_RULE_H
#define SIGNATURE_RULE_H
#include <memory>
#include <vector>
#include <variant>
#include <iostream>

#include "../SignatureScanner.h"
#include "visit_struct/visit_struct.hpp"

namespace org::kapa::tarracsh::signatures {

#define DECLARE_RULE_CLASS( ClassName ) \
    class ClassName;\
    typedef std::shared_ptr<ClassName> ClassName##Ptr

DECLARE_RULE_CLASS(Rule);
DECLARE_RULE_CLASS(Or);
DECLARE_RULE_CLASS(Kleene);
DECLARE_RULE_CLASS(Optional);
DECLARE_RULE_CLASS(Rule);
DECLARE_RULE_CLASS(Terminal);
DECLARE_RULE_CLASS(JvmIdentifier);

#undef DECLARE_RULE_CLASS

#define _DEBUG_RULE 


#ifdef _DEBUG
class DebugRule;
#define SET_RULE_NAME( rule ) rule->setName( L""#rule )
#define GET_RULE_NAME( rule ) rule->getName()
#define SET_RULE_NAME2( rule, name ) rule->setName( name )
#ifdef _DEBUG_RULE
#define DEBUG_RULE( rule ) DebugRule debugRule( *rule, result );
#endif
#else
    #define SET_RULE_NAME2
    #define SET_RULE_NAME
    #define GET_RULE_NAME
    #define DEBUG_RULE
#endif

typedef std::variant<
    RulePtr,
    KleenePtr,
    TerminalPtr,
    JvmIdentifierPtr,
    OptionalPtr,
    OrPtr>
RuleVariant;


class Rule {
public:
    explicit Rule();
    explicit Rule(const bool isAnchor );
    Rule(const Rule &other);
    Rule(Rule &&other) noexcept;
    Rule &operator=(const Rule &other);
    Rule &operator=(Rule &&other) noexcept;
    virtual ~Rule();


    template <typename T>
    bool match(SignatureScanner &scanner, T &node) {
        bool result = true;
        DEBUG_RULE(this);
        auto index = 0u;

        visit_struct::for_each(node, [this, &index, &result, &scanner](const char *name, auto &value) {
            if (!result) return;

            while (index < _followBy.size()) {
                auto &ruleVariant = _followBy[index];
                auto matchResult = invokeMatch(ruleVariant, scanner, value);
                result = matchResult.first;

                if (!result) return;

                auto canCapture = matchResult.second;
                if (canCapture) break;
                index++;
            }
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

    template <typename T>
    bool followByMatch(SignatureScanner &scanner, T &node) {
        auto result = false;

        const auto scannerPosition = scanner.getPosition();

        for (auto &ruleVariant : _followBy) {
            const auto matchResult = invokeMatch(ruleVariant, scanner, node);
            result = matchResult.first;
            if (!result) break;
        }

        if (!result) {
            scanner.reset(scannerPosition);
        }

        return result;
    }


    bool match(SignatureScanner &scanner, std::wstring &value);

    template <typename T>
    bool match(SignatureScanner &scanner, std::vector<T> &list) {
        auto result = false;
        DEBUG_RULE(this);
        result = followByMatch(scanner, list);
        return result;
    }

    template <typename T>
    void followByRuleVariant(const T &ruleVariant) {
        _followBy.push_back(ruleVariant);
    }

#ifdef _DEBUG
    void setName(std::wstring name) {
        _name = name;
    }

    std::wstring getName() const { return _name; }
#endif

    void setCapture(const bool value) { _capture = value; }
    [[nodiscard]] bool getCapture() const { return _capture; }

    void setAnchor(const bool value) { _isAnchor = value; }
    [[nodiscard]] bool isAnchor() const { return _isAnchor; }


protected:
    std::vector<RuleVariant> _followBy;
    std::wstring _name;
    bool _capture{true};
    bool _isAnchor{false};

};

#ifdef _DEBUG
class DebugRule {
public:
    explicit DebugRule(const Rule &rule, bool &result)
        : _rule(rule), _result(result) {
        _indent += 2;
        const auto indentSpace = std::wstring(_indent, L' ');
        std::wcout << indentSpace << L"IN =>" << rule.getName() << std::endl;

    }

    ~DebugRule() {
        const auto indentSpace = std::wstring(_indent, L' ');
        std::wcout << indentSpace << L"OUT <=" << (_result ? L" y " : L" n ") << _rule.getName() << std::endl;
        _indent -= 2;
    }

private:
    const Rule &_rule;
    bool &_result;
    inline static int _indent{0};

};
#endif

}


#endif
