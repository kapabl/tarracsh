#ifndef ACCESS_MODIFIERS_H
#define ACCESS_MODIFIERS_H


#include <string>
#include <array>

#include "ClassFileConstants.h"
#include "../../infrastructure/string/StringUtils.h"
#include "constantpool/StructsCommon.h"

namespace kapa::tarracsh::domain::classfile::accessmodifier {

using constantpool::u1;
using constantpool::u2;
using constantpool::u4;

class AccessModifiers {
public:
    [[nodiscard]] std::string toString(const u2 accessFlags) const {
        u4 bit = 1;
        std::vector<std::string> presentAccessModifiers;
        for (auto &accessModifier : _accessModifiers) {
            if ((accessFlags & bit) != 0) {
                presentAccessModifiers.push_back(infrastructure::string::stringUtils::toLower(accessModifier));
            }
            bit = bit << 1;
        }
        auto result = infrastructure::string::stringUtils::join<std::string>(presentAccessModifiers, " ");
        return result;
    }

    [[nodiscard]] static bool isPublic(const u2 accessFlags) {
        const bool result = (accessFlags & JVM_ACC_PUBLIC) != 0;
        return result;
    }

private:
    // clang-format off
    inline static std::array<std::string,19> _accessModifiers{
        "PUBLIC",
        "PRIVATE",
        "PROTECTED",
        "STATIC",
        "FINAL",
        "SYNCHRONIZED",
        "SUPER",
        "VOLATILE",
        "BRIDGE",
        "TRANSIENT",
        "VARARGS",
        "NATIVE",
        "INTERFACE",
        "ABSTRACT",
        "STRICT",
        "SYNTHETIC",
        "ANNOTATION",
        "ENUM",
        "MODULE"};
};

}


#endif

