#ifndef SIGNATURE_OR_RULE_H
#define SIGNATURE_OR_RULE_H
#include "Rule.h"


namespace org::kapa::tarracsh::signatures {

// template <typename T>
// bool invokeMatch(RuleVariant& ruleVariant, SignatureScanner& scanner, T& node);

class Or final :  Rule {
public:
    Or();


    explicit Or(const Rule& left);
    Or(const Or &other);
    Or(Or &&other) noexcept;
    Or &operator=(const Or &other);
    Or &operator=(Or &&other) noexcept;
    ~Or() override = default;
    
    void add(const Rule &rule);

    template <typename T>
    bool match(SignatureScanner &scanner, T &node) {
        auto result = false;
        for (auto &rule : _rules) {
            result = invokeMatch<T>(rule, scanner, node);
            if (result) break;
        }
        return result;
    }

    template <typename T>
    bool match(SignatureScanner &scanner, std::vector<T> &list) {
        return false;
    }

    template <>
    bool match<std::wstring>(SignatureScanner &scanner, std::wstring &value) {
        return false;
    }

private:
    std::vector<RuleVariant> _rules;
    //Rule::RuleVariant _rule1;
};

}
#endif
