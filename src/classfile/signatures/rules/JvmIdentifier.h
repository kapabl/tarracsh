#ifndef SIGNATURE_JVM_IDENTIFIER_RULE_H
#define SIGNATURE_JVM_IDENTIFIER_RULE_H

#include <string>
#include "Rule.h"

namespace org::kapa::tarrash::signatures {

class JvmIdentifier;
typedef std::shared_ptr<JvmIdentifier> JvmIdentifierPtr;

class JvmIdentifier final : public Rule {
public:
    explicit JvmIdentifier();
    JvmIdentifier(const JvmIdentifier &other);
    JvmIdentifier(JvmIdentifier &&other) noexcept;
    JvmIdentifier &operator=(const JvmIdentifier &other);
    JvmIdentifier &operator=(JvmIdentifier &&other) noexcept;

    // bool match (SignatureScanner &scanner) override;
    bool match (SignatureScanner &scanner, std::wstring& identifier);
    ~JvmIdentifier() override = default;

    std::wstring getValue();

    static std::vector<bool> validJvmIdentifierChars;
    static bool isValidJvmIdentifierChar(wchar_t character);
    static void initJvmIdentifierChar();
    

private:
    std::wstring _value;

    struct StaticInit {
        StaticInit() { initJvmIdentifierChar(); }
    };
    static StaticInit _staticInit;
};
}
#endif
