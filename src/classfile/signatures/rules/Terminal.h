#ifndef SIGNATURE_TERMINAL_RULE_H
#define SIGNATURE_TERMINAL_RULE_H

#include <string>
#include "Rule.h"

namespace org::kapa::tarrash::signatures {

class Terminal final : public Rule {
public:
    explicit Terminal(std::wstring value);
    explicit Terminal(const wchar_t* value);
    Terminal(const Terminal &other);
    Terminal(Terminal &&other) noexcept;
    Terminal &operator=(const Terminal &other);
    Terminal &operator=(Terminal &&other) noexcept;
    // bool match (SignatureScanner &scanner) override;
    ~Terminal() override = default;

    std::wstring getValue();

private:
    std::wstring _value;
};
}
#endif
