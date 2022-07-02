#ifndef ACCESS_MODIFIERS_H
#define ACCESS_MODIFIERS_H


#include <string>
#include <array>

#include "StringUtils.h"
#include "StructsCommon.h"

using namespace org::kapa::tarrash::common;
using namespace org::kapa::tarrash::stringUtils;

namespace org::kapa::tarrash::accessModifiers {

class AccessModifiers {
public:

    std::wstring toString(const u2 accessFlags) const {
        u2 bit = 1;
        std::vector<std::wstring> presentAccessModifiers;
        for (auto &accessModifier : _accessModifiers) {
            if ((accessFlags & bit) != 0) {
                presentAccessModifiers.push_back(toLower(accessModifier));
            }
            bit = bit << 1;
        }
        auto result = join<wstring>(presentAccessModifiers, L" ");
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

