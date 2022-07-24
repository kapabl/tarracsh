#ifndef SIGNATURE_KLEENE_RULE_H
#define SIGNATURE_KLEENE_RULE_H

#include "Rule.h"
#include "../SignatureScanner.h"


namespace org::kapa::tarracsh::signatures {

// class Kleene;

// template <typename T>
// bool invokeMatch(Rule::RuleVariant &ruleVariant, SignatureScanner &scanner, T &node);


class Kleene final : public Rule {
public:
    Kleene();
    Kleene(RuleVariant rule, int minimum);

    Kleene(const Kleene &other);

    Kleene(Kleene &&other) noexcept;

    Kleene &operator=(const Kleene &other);

    Kleene &operator=(Kleene &&other) noexcept;

    template <typename T>
    bool match(SignatureScanner &scanner, T &node) {
        bool result = false;
        DEBUG_RULE(this);
        return result;
    }

    template <typename T>
    bool match(SignatureScanner &scanner, std::vector<T> &list) {
        bool result = false;
        DEBUG_RULE(this);

        const auto startPosition = scanner.getPosition();
        //TODO
        auto matchedCount = 0;
        T item;
        // while (_ruleVariant.match(scanner, item)) {
        auto matchResult = invokeMatch(_ruleVariant, scanner, item);
        while (matchResult.first ) {
            list.push_back(item);
            matchedCount++;
            matchResult = invokeMatch(_ruleVariant, scanner, item);
        }
        
        result = matchedCount >= _minimum;
        if ( !result ) {
            scanner.reset(startPosition);
        }

        //TODO match the followBy
        return true;
    }

    ~Kleene() override = default;


private:
    //Rule _ruleVariant;
    RuleVariant _ruleVariant;
    int _minimum = 0;
};


}


#endif
