#ifndef SIGNATURE_OPTIONAL_RULE_H
#define SIGNATURE_OPTIONAL_RULE_H
#include "Rule.h"


namespace org::kapa::tarrash::signatures {
class Optional final : public Rule {
public:
    Optional();
    Optional(Rule rule);

    Optional(const Optional &other);

    Optional(Optional &&other) noexcept;

    Optional & operator=(const Optional &other);

    Optional & operator=(Optional &&other) noexcept;
    ~Optional() override = default;

    template <typename T>
    bool match(SignatureScanner& scanner, T& node) {
        auto innerRuleMatched = _rule.match(scanner, node);
        return true;
    }

    template <typename T>
    bool match(SignatureScanner& scanner, std::vector<T>& list) {
        return false;
    }

    template <>
    bool match<std::wstring>(SignatureScanner& scanner, std::wstring& value) {
        return false;
    }

private:
    Rule _rule;

};
} // namespace org::kapa::tarrash::signatures
#endif
