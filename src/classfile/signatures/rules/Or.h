#ifndef SIGNATURE_OR_RULE_H
#define SIGNATURE_OR_RULE_H
#include "Rule.h"


namespace org::kapa::tarrash::signatures {

class Or final : public Rule {
public:
    explicit Or(const RulePtr &left);
    explicit Or();

    Or(const Or &other);
    Or(Or &&other) noexcept;
    Or &operator=(const Or &other);
    Or &operator=(Or &&other) noexcept;
    ~Or() override = default;

    // bool match(SignatureScanner &scanner) override;
    void add(const RulePtr & rule);

private:
    std::vector<RulePtr> _rules;
};

}
#endif