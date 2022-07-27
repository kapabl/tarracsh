#include "JvmIdentifier.h"
#include "RuleFuncs.h"

using namespace org::kapa::tarracsh::signatures;
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
    const string InvalidChars = ".;[/<>:";
    for( auto& character: InvalidChars) {
        validJvmIdentifierChars[character] = false;
    }
}

template <>
bool JvmIdentifier::match<Identifier>(SignatureScanner &scanner, Identifier &node) {
    return match(scanner, node.value);
}

template <>
bool JvmIdentifier::match<wstring>(SignatureScanner &scanner, std::wstring &node) {
    auto result = false;
    DEBUG_RULE(this);
    node.clear();
    const auto scannerPosition = scanner.getPosition();
    auto length = 0u;
    while (!scanner.isEOF()) {
        const auto character = scanner.peekNextChar();
        if (!isValidJvmIdentifierChar(character)) break;
        scanner.step();
        node.push_back(character);
        ++length;
    }
    result = length > 0;
    if ( result ) {
        consoleOut(node);
        //TODO continue with the followBy
        //Rule::match(scanner, node)
            
    }
    else {
        scanner.reset(scannerPosition);
        node.clear();
    }

    return result;
}

std::wstring JvmIdentifier::getValue() { return _value; }

