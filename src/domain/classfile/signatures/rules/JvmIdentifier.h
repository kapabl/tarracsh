#ifndef SIGNATURE_JVM_IDENTIFIER_RULE_H
#define SIGNATURE_JVM_IDENTIFIER_RULE_H

#include <string>
#include "Rule.h"
#include "../SignatureStructures.h"

namespace kapa::tarracsh::signatures {
struct Identifier;


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


    template <>
    bool match<Identifier>(SignatureScanner& scanner, Identifier& node);

    template <typename T>
    bool match(SignatureScanner& scanner, std::vector<T>& list) {
        return false;
    }

    template <>
    bool match<std::wstring>(SignatureScanner& scanner, std::wstring& node);


private:
    std::wstring _value;

    struct StaticInit {
        StaticInit() { initJvmIdentifierChar(); }
    };
    static StaticInit _staticInit;
};
}
#endif
