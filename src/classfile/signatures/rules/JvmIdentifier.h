#ifndef SIGNATURE_JVM_IDENTIFIER_RULE_H
#define SIGNATURE_JVM_IDENTIFIER_RULE_H

#include <string>
#include "Rule.h"

namespace org::kapa::tarrash::signatures {

class JvmIdentifier final : public Rule {
public:
    JvmIdentifier();
    JvmIdentifier(const JvmIdentifier &other);
    JvmIdentifier(JvmIdentifier &&other) noexcept;
    JvmIdentifier &operator=(const JvmIdentifier &other);
    JvmIdentifier &operator=(JvmIdentifier &&other) noexcept;

    ~JvmIdentifier() override = default;

    std::wstring getValue();

    static std::vector<bool> validJvmIdentifierChars;
    static bool isValidJvmIdentifierChar(wchar_t character);
    static void initJvmIdentifierChar();

    template <typename T>
    bool match(SignatureScanner& scanner, T& node) {
        return false;
    }

    template <typename T>
    bool match(SignatureScanner& scanner, std::vector<T>& list) {
        return false;
    }
    template <>
    bool match<std::wstring>(SignatureScanner& scanner, std::wstring& identifier) {
        const auto scannerPosition = scanner.getPosition();
        auto length = 0u;
        while (!scanner.isEOF()) {
            const auto character = scanner.getNextChar();
            if (!isValidJvmIdentifierChar(character)) break;
            identifier.push_back(character);
            ++length;
        }
        const auto result = length > 0;
        if (!result) {
            scanner.reset(scannerPosition);
            identifier.clear();
        }
        return false;
    }
    

private:
    std::wstring _value;

    struct StaticInit {
        StaticInit() { initJvmIdentifierChar(); }
    };
    static StaticInit _staticInit;
};
}
#endif
