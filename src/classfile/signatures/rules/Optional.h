#ifndef SIGNATURE_OPTIONAL_RULE_H
#define SIGNATURE_OPTIONAL_RULE_H
#include "Rule.h"



namespace org::kapa::tarracsh::signatures {
class Optional final : public Rule {
public:
    Optional();
    Optional(const RuleVariant& ruleVariant);

    Optional(const Optional &other);

    Optional(Optional &&other) noexcept;

    Optional & operator=(const Optional &other);

    Optional & operator=(Optional &&other) noexcept;
    ~Optional() override = default;

    template <typename T>
    bool match(SignatureScanner& scanner, T& node) {
        bool result = true;
        DEBUG_RULE(this);
        auto matchResult = invokeMatch(_ruleVariant, scanner, node);
        return true;
    }

    bool match(SignatureScanner& scanner, std::wstring& value);

    // template <typename T>
    // bool match(SignatureScanner& scanner, std::vector<T>& list) {
    //     auto matchResult = invokeMatch(_ruleVariant, scanner, list);
    //     //TODO match the followBy
    //     return true;
    // }
    //


private:
    RuleVariant _ruleVariant;

};

} // namespace org::kapa::tarracsh::signatures

#endif
