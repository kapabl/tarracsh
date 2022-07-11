#include "JvmIdentifier.h"
#include "JvmIdentifier.h"

using namespace org::kapa::tarrash::signatures;
using namespace std;


JvmIdentifier::JvmIdentifier() = default;


JvmIdentifier::JvmIdentifier(const JvmIdentifier &other) = default;
JvmIdentifier::JvmIdentifier(JvmIdentifier &&other) noexcept = default;
JvmIdentifier &JvmIdentifier::operator=(const JvmIdentifier &other) = default;
JvmIdentifier &JvmIdentifier::operator=(JvmIdentifier &&other) noexcept = default;


bool JvmIdentifier::isValidJvmIdentifierChar(wchar_t character) {
    auto result = validJvmIdentifierChars[character];
    return result;
}

vector<bool> JvmIdentifier::validJvmIdentifierChars(1024 * 64, true);
JvmIdentifier::StaticInit JvmIdentifier::_staticInit;

void JvmIdentifier::initJvmIdentifierChar() {
    // JvmIdentifier::validJvmIdentifierChars.resize(1024 * 64, true);
    const string invalidChars = ".;[/<>:";
    for( auto& character: invalidChars) {
        validJvmIdentifierChars[character] = false;
    }
}

// bool JvmIdentifier::match(SignatureScanner &scanner) {
//
//     const auto scannerPosition = scanner.getPosition();
//     auto length = 0u;
//     while (!scanner.isEOF()) {
//         const auto character = scanner.getNextChar();
//         if (!isValidJvmIdentifierChar(character)) break;
//         ++length;
//     }
//     const auto result = length > 0;
//     if (!result) {
//         scanner.reset(scannerPosition);
//     }
//     return false;
// }

bool JvmIdentifier::match(SignatureScanner& scanner, std::wstring& identifier) {

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

std::wstring JvmIdentifier::getValue() { return _value; }

