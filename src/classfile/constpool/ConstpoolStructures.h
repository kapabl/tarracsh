#ifndef TARRACSH_CLASSFILESTRUCTURE_H
#define TARRACSH_CLASSFILESTRUCTURE_H

#include <unicode/unistr.h>
#include "StructsCommon.h"
#include "StackFrame.h"
#include "../AttributeStructures.h"
#include "../utils/StringUtils.h"

#pragma pack(push, 1)

namespace org::kapa::tarracsh {

using common::u1;
using common::u2;
using common::u4;

struct ClassFileHeader {
    u4 magic;
    u2 minorVersion;
    u2 majorVersion;
};

struct MainClassInfo {
    u2 accessFlags;
    u2 thisClass;
    u2 superClass;

    bool isInterface() const { return (accessFlags & JVM_ACC_INTERFACE) != 0; }
};

struct FieldInfo {
    u2 accessFlags{};
    u2 nameIndex{};
    u2 descriptorIndex{};
    std::vector<attributes::AttributeInfo> attributes;
};

struct MethodInfo {
    u2 accessFlags{};
    u2 nameIndex{};
    u2 descriptorIndex{};
    std::vector<attributes::AttributeInfo> attributes;
};

struct ConstPoolBase {
    ConstantPoolTag tag;
};

struct MemberInfo : ConstPoolBase {
    u2 classIndex;
    u2 nameAndTypeIndex;
};

struct NameAndTypeInfo : ConstPoolBase {
    u2 nameIndex;
    u2 descriptorIndex;
};


struct FieldrefInfo : MemberInfo {
};

struct MethodrefInfo : MemberInfo {
};

struct InterfaceMethodrefInfo : MemberInfo {
};

struct Utf8Info : ConstPoolBase {
    u2 length;
    u1 bytes[1];

    // [[nodiscard]] std::wstring getValue(bool withEscaped = false) const {
    //     auto result = stringUtils::utf82wstring(bytes, withEscaped);
    //     return result;
    // }
    //
    // [[nodiscard]] std::wstring getValueAsClassname(bool withEscaped = false) const {
    //     auto result = getValue(withEscaped);
    //     for (auto &wchar : result) {
    //         if (wchar == L'/') {
    //             wchar = L'.';
    //         }
    //     }
    //     return result;
    // }
    
    [[nodiscard]] std::string getAsUtf8(bool withEscaped = false) const {
        const icu::StringPiece stringPiece(reinterpret_cast<const char*>(bytes), length);
        std::string result;
        icu::UnicodeString::fromUTF8(stringPiece).toUTF8String(result);
        return result;
    }
};

struct ClassInfo : ConstPoolBase {
    u2 nameIndex;
};

struct StringInfo : ConstPoolBase {
    u2 stringIndex;
};

struct MethodTypeInfo : ConstPoolBase {
    u2 descriptorIndex;
};

struct IntegerInfo : ConstPoolBase {
    int value;
};

struct FloatInfo : ConstPoolBase {
    u4 u4Value;

    [[nodiscard]] float getFloat() const {
        const float result = *reinterpret_cast<const float *>(&u4Value);
        return result;
    }
};

struct LongInfo : ConstPoolBase {
    u4 highBytes;
    u4 lowBytes;

    [[nodiscard]] long long getLongLong() const {
        long long result = highBytes;
        result = result << 32 | lowBytes;
        return result;
    }
};

struct DoubleInfo : ConstPoolBase {
    u4 highBytes;
    u4 lowBytes;

    [[nodiscard]] double getDouble() const {
        uint64_t uint64 = highBytes;
        uint64 = uint64 << 32 | lowBytes;
        const double result = *reinterpret_cast<double *>(&uint64);
        return result;
    }
};

struct MethodHandleInfo : ConstPoolBase {
    MethodHandleSubtypes referenceKind;
    u2 referenceIndex;
};

struct DynamicInfo : ConstPoolBase {
    u2 bootstrapMethodAttrIndex;
    u2 nameAndTypeIndex;
};

struct PackageInfo : ConstPoolBase {
    u2 nameIndex;
};

struct InvokeDynamicInfo : ConstPoolBase {
    u2 bootstrapMethodAttrIndex;
    u2 nameAndTypeIndex;
};

struct ModuleInfo : ConstPoolBase {
    u2 nameIndex;
};

struct ConstantPoolRecord {
    union {
        ConstPoolBase base;
        Utf8Info utf8Info;
        ClassInfo classInfo;
        FieldrefInfo fieldrefInfo;
        MethodrefInfo methodrefInfo;
        NameAndTypeInfo nameAndTypeInfo;
        InterfaceMethodrefInfo interfaceMethodrefInfo;
        MethodTypeInfo methodTypeInfo;
        StringInfo stringInfo;
        FloatInfo floatInfo;
        IntegerInfo integerInfo;
        DoubleInfo doubleInfo;
        LongInfo longInfo;
        MethodHandleInfo methodHandleInfo;
        InvokeDynamicInfo invokeDynamicInfo;
        ModuleInfo moduleInfo;
        DynamicInfo dynamicInfo;
        PackageInfo packageInfo;
    };
};

struct Descriptor {
    bool isArray;
    bool isClass;
    int dimensions;
    std::string type;

    Descriptor()
        : isArray(false), isClass(false), dimensions(0) {
    }

    [[nodiscard]] std::string toString() const {
        std::string result(type);

        for (int i = 0; i < dimensions; i++) {
            result += "[]";
        }

        return result;
    }
};

struct MethodDescriptor {
    std::vector<Descriptor> arguments;
    Descriptor returnType;

    [[nodiscard]] std::string argumentsToString() const {
        std::vector<std::string> parts;

        for (auto &descriptor : arguments) {
            parts.push_back(descriptor.toString());
        }

        std::string result = "(" + stringUtils::join<std::string>(parts, ", ") + ")";

        return result;
    }
};

}
#pragma pack(pop)

#endif 