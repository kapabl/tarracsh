#ifndef SIGNATURE_KLEENE_RULE_H
#define SIGNATURE_KLEENE_RULE_H

#include "Rule.h"
#include "../SignatureScanner.h"


namespace org::kapa::tarrash::signatures {

class Kleene final : public Rule {
public:
    Kleene(Rule &rule, int minimum);

    Kleene(const Kleene &other);

    Kleene(Kleene &&other) noexcept;

    Kleene &operator=(const Kleene &other);

    Kleene &operator=(Kleene &&other) noexcept;

    // bool match(SignatureScanner &scanner) override;

    template<typename TVisitable>
    bool match(SignatureScanner &scanner, std::vector<TVisitable>& visitables);

    ~Kleene() override = default;


private:
    Rule _rule;
    int _minimum = 0;
};

template<typename TVisitable>
bool Kleene::match(SignatureScanner& scanner, std::vector<TVisitable>& visitables) {
    auto matchedCount = 0;
    //auto index = 0u;

    auto visitable = TVisitable();
    while (_rule.match(scanner, visitable)) {
        visitables.push_back(visitable);
        visitable = TVisitable();
        matchedCount++;
    }

    const auto result = matchedCount >= _minimum;
    if ( !result ) {
        visitables.clear();
    }

    return true;
}

}
#endif
