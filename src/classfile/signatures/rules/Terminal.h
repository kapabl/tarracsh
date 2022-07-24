#ifndef SIGNATURE_TERMINAL_RULE_H
#define SIGNATURE_TERMINAL_RULE_H

#include <string>
#include "Rule.h"

namespace org::kapa::tarracsh::signatures {

class Terminal final : public Rule {
public:
    Terminal();
    explicit Terminal(std::wstring value);
    explicit Terminal(const wchar_t *value);
    Terminal(const Terminal &other);
    Terminal(Terminal &&other) noexcept;
    Terminal &operator=(const Terminal &other);
    Terminal &operator=(Terminal &&other) noexcept;
    // bool match (SignatureScanner &scanner) override;
    ~Terminal() override = default;


    bool terminalMatch(SignatureScanner& scanner) {
        const auto scannerPosition = scanner.getPosition();
        auto position = 0u;
        while (position < _value.length() && !scanner.isEOF()) {
            const auto character = scanner.getNextChar();
            if (_value[position] != character) break;
            position++;
        }
        const auto result = position == _value.length();
        if (!result) {
             scanner.reset(scannerPosition);
        }

        return result;
    }

    template <typename T>
    bool match(SignatureScanner &scanner, T &node) {
        bool result = false;
        DEBUG_RULE(this);
        result = terminalMatch(scanner);
        if (result) {
            //TODO match the followBy
        }
        return result;
    }

    bool match(SignatureScanner& scanner, std::wstring& value);

    std::wstring getValue();

private:
    std::wstring _value;
};
}
#endif
