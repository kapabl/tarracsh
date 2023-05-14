#ifndef ACCESS_MODIFIERS_H
#define ACCESS_MODIFIERS_H


#include <string>
#include <array>

#include "ClassFileConstants.h"
#include "infrastructure/string/StringUtils.h"
#include "constantpool/StructsCommon.h"

namespace kapa::tarracsh::domain::classfile::accessmodifier {

using constantpool::u1;
using constantpool::u2;
using constantpool::u4;

// clang-format off
inline static std::array<std::string, 19> _accessModifiers{
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


[[nodiscard]] static auto toString(const u2 accessFlags) -> std::string {
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

[[nodiscard]] static auto isPublic(const u2 accessFlags) -> bool {
    const bool result = (accessFlags & JVM_ACC_PUBLIC) != 0;
    return result;
}

[[nodiscard]] static auto isPrivate(const u2 accessFlags) -> bool {
    const bool result = (accessFlags & JVM_ACC_PRIVATE) != 0;
    return result;
}

[[nodiscard]] static auto isProtected(const u2 accessFlags) -> bool {
    const bool result = (accessFlags & JVM_ACC_PROTECTED) != 0;
    return result;
}

[[nodiscard]] static auto isStatic(const u2 accessFlags) -> bool {
    const bool result = (accessFlags & JVM_ACC_STATIC) != 0;
    return result;
}

[[nodiscard]] static auto isFinal(const u2 accessFlags) -> bool {
    const bool result = (accessFlags & JVM_ACC_FINAL) != 0;
    return result;
}

[[nodiscard]] static auto isSynchronized(const u2 accessFlags) -> bool {
    const bool result = (accessFlags & JVM_ACC_SYNCHRONIZED) != 0;
    return result;
}

[[nodiscard]] static auto isSuper(const u2 accessFlags) -> bool {
    const bool result = (accessFlags & JVM_ACC_SUPER) != 0;
    return result;
}

[[nodiscard]] static auto isVolatile(const u2 accessFlags) -> bool {
    const bool result = (accessFlags & JVM_ACC_VOLATILE) != 0;
    return result;
}

[[nodiscard]] static auto isBridge(const u2 accessFlags) -> bool {
    const bool result = (accessFlags & JVM_ACC_BRIDGE) != 0;
    return result;
}

[[nodiscard]] static auto isTransient(const u2 accessFlags) -> bool {
    const bool result = (accessFlags & JVM_ACC_TRANSIENT) != 0;
    return result;
}

[[nodiscard]] static auto isVarargs(const u2 accessFlags) -> bool {
    const bool result = (accessFlags & JVM_ACC_VARARGS) != 0;
    return result;
}

[[nodiscard]] static auto isNative(const u2 accessFlags) -> bool {
    const bool result = (accessFlags & JVM_ACC_NATIVE) != 0;
    return result;
}

[[nodiscard]] static auto isInterface(const u2 accessFlags) -> bool {
    const bool result = (accessFlags & JVM_ACC_INTERFACE) != 0;
    return result;
}

[[nodiscard]] static auto isAbstract(const u2 accessFlags) -> bool {
    const bool result = (accessFlags & JVM_ACC_ABSTRACT) != 0;
    return result;
}


}


#endif
