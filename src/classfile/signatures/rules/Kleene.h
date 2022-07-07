#ifndef SIGNATURE_KLEENE_RULE_H
#define SIGNATURE_KLEENE_RULE_H
#include <vector>

#include "Optional.h"
#include "Rule.h"
#include "../SignatureScanner.h"



namespace org::kapa::tarrash::signatures {

class Kleene final : public Rule {
public:
    Kleene(Rule& rule, SignatureScanner &scanner, int minimum );
    
    Kleene(const Kleene &other);

    Kleene(Kleene &&other) noexcept;

    Kleene & operator=(const Kleene &other);

    Kleene & operator=(Kleene &&other) noexcept;

    bool match() override;



private:
    Rule _rule;
    int _minimum = 0;
};


} 
#endif
