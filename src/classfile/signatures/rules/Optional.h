#ifndef SIGNATURE_OPTIONAL_RULE_H
#define SIGNATURE_OPTIONAL_RULE_H
#include "Rule.h"


namespace org::kapa::tarrash::signatures {
class Optional final : public Rule {
public:
    Optional(const Rule &rule, SignatureScanner &scanner);

    Optional(const Optional &other);

    Optional(Optional &&other) noexcept;

    Optional & operator=(const Optional &other);

    Optional & operator=(Optional &&other) noexcept;

private:
    Rule _rule;

};
} // namespace org::kapa::tarrash::signatures
#endif
