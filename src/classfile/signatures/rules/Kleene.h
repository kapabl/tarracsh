#ifndef SIGNATURE_KLEENE_RULE_H
#define SIGNATURE_KLEENE_RULE_H

#include "Rule.h"
#include "../SignatureScanner.h"


namespace org::kapa::tarrash::signatures {

// class Kleene;

// template <typename T>
// bool invokeMatch(Rule::RuleVariant &ruleVariant, SignatureScanner &scanner, T &node);

class Kleene final : public Rule {
public:
    Kleene();
    Kleene(Rule &rule, int minimum);

    Kleene(const Kleene &other);

    Kleene(Kleene &&other) noexcept;

    Kleene &operator=(const Kleene &other);

    Kleene &operator=(Kleene &&other) noexcept;

    template <typename T>
    bool match(SignatureScanner &scanner, T &node) {
        return false;
    }

    template <typename T>
    bool match(SignatureScanner &scanner, std::vector<T> &list) {
        //TODO
        // auto matchedCount = 0;
        // T item;
        // // while (_rule.match(scanner, item)) {
        // while (invokeMatch<T>(_rule, scanner, item)) {
        //     list.push_back(item);
        //     matchedCount++;
        // }
        //
        // auto result = matchedCount >= _minimum;

        return true;
    }

    template <>
    bool match<std::wstring>(SignatureScanner &scanner, std::wstring &value) {
        return false;
    }


    ~Kleene() override = default;


private:
    Rule _rule;
    // RuleVariant _rule;
    int _minimum = 0;
};


}


#endif
