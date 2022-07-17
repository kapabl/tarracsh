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
    const string invalidChars = ".;[/<>:";
    for( auto& character: invalidChars) {
        validJvmIdentifierChars[character] = false;
    }
}

std::wstring JvmIdentifier::getValue() { return _value; }

