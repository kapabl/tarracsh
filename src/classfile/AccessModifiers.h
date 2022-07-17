#ifndef ACCESS_MODIFIERS_H
#define ACCESS_MODIFIERS_H


#include <string>
#include <array>

#include "StringUtils.h"
#include "StructsCommon.h"

namespace org::kapa::tarracsh::accessModifiers {

using common::u1;
using common::u2;
using common::u4;

class AccessModifiers {
public:
    [[nodiscard]] std::wstring toString(const u2 accessFlags) const {
        u4 bit = 1;
        std::vector<std::wstring> presentAccessModifiers;
        for (auto &accessModifier : _accessModifiers) {
            if ((accessFlags & bit) != 0) {
                presentAccessModifiers.push_back(stringUtils::toLower(accessModifier));
            }
            bit = bit << 1;
        }
        auto result = stringUtils::join<std::wstring>(presentAccessModifiers, L" ");
        return result;
    }

private:
    // clang-format off
    inline static std::array<std::wstring,19> _accessModifiers{
        L"PUBLIC",
        L"PRIVATE",
        L"PROTECTED",
        L"STATIC",
        L"FINAL",
        L"SYNCHRONIZED",
        L"SUPER",
        L"VOLATILE",
        L"BRIDGE",
        L"TRANSIENT",
        L"VARARGS",
        L"NATIVE",
        L"INTERFACE",
        L"ABSTRACT",
        L"STRICT",
        L"SYNTHETIC",
        L"ANNOTATION",
        L"ENUM",
        L"MODULE"};
};

}


#endif

