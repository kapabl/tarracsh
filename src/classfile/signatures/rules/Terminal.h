#ifndef SIGNATURE_TERMINAL_RULE_H
#define SIGNATURE_TERMINAL_RULE_H

#include <string>
#include "Rule.h"

namespace org::kapa::tarrash::signatures {

class Terminal final : public Rule {
public:
    Terminal();
    explicit Terminal(std::wstring value);
    explicit Terminal(const wchar_t* value);
    Terminal(const Terminal &other);
    Terminal(Terminal &&other) noexcept;
    Terminal &operator=(const Terminal &other);
    Terminal &operator=(Terminal &&other) noexcept;
    // bool match (SignatureScanner &scanner) override;
    ~Terminal() override = default;

    template <typename T>
    bool match(SignatureScanner& scanner, T& node) {
        return false;
    }

    template <typename T>
    bool match(SignatureScanner& scanner, std::vector<T>& list) {
        return false;
    }

    template <>
    bool match<std::wstring>(SignatureScanner& scanner, std::wstring& value) {
        const auto scannerPosition = scanner.getPosition();
        auto position = 0u;
        while (position < _value.length() && !scanner.isEOF()) {
            const auto character = scanner.getNextChar();
            if (_value[position] != character ) break;
            position++;
        }
        const auto result = position == _value.length();
        if ( result ) {
            value = _value;
        }
        else {
            scanner.reset(scannerPosition);
        }
        return false;
    }

    std::wstring getValue();

private:
    std::wstring _value;
};
}
#endif
