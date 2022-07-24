#ifndef SIGNATURE_JVM_IDENTIFIER_RULE_H
#define SIGNATURE_JVM_IDENTIFIER_RULE_H

#include <string>
#include "Rule.h"

namespace org::kapa::tarracsh::signatures {

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


    //TODO: the followBy ... we dont have the struct/node here`
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
        if ( result ) {
            //TODO continue with the followBy
            //Rule::match(scanner, node)
        }
        else {
            scanner.reset(scannerPosition);
            identifier.clear();
        }

        return result;
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
