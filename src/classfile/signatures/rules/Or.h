#ifndef SIGNATURE_OR_RULE_H
#define SIGNATURE_OR_RULE_H
#include "Rule.h"


namespace org::kapa::tarracsh::signatures {

// template <typename T>
// bool invokeMatch(RuleVariant& ruleVariant, SignatureScanner& scanner, T& node);

class Or final: public Rule {
public:
    Or();
    explicit Or(const bool isAnchor);

    explicit Or(const RulePtr &left);
    Or(const Or &other);
    Or(Or &&other) noexcept;
    Or &operator=(const Or &other);
    Or &operator=(Or &&other) noexcept;
    ~Or() override = default;

    void addToOr(RuleVariant ruleVariant);

    template <typename T>
    bool match(SignatureScanner &scanner, T &node) {
        auto result = false;
        DEBUG_RULE(this);
        for (auto &rule : _rules) {
            auto matchResult = invokeMatch(rule, scanner, node);
            result = matchResult.first;
            if (result) break;
        }

        //TODO match the followby
        return result;
    }

    template <typename T>
    bool match(SignatureScanner &scanner, std::vector<T> &list) {
        auto result = false;
        DEBUG_RULE(this);
        return result;
    }

    template <>
    bool match<std::wstring>(SignatureScanner &scanner, std::wstring &value) {
        auto result = false;
        DEBUG_RULE(this);
        return result;
    }

private:
    std::vector<RuleVariant> _rules;
};

}
#endif
