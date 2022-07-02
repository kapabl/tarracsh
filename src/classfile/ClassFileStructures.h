
#ifndef TARRASH_CLASSFILESTRUCTURE_H
#define TARRASH_CLASSFILESTRUCTURE_H

#include "StructsCommon.h"
#include "StackFrame.h"
#include "AttributeStructures.h"
#include "StringUtils.h"

#pragma pack(push, 1)

namespace org::kapa::tarrash {

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

    bool isInterface() { return (accessFlags & JVM_ACC_INTERFACE) != 0; }
};

struct AttributeInfo {
    u2 nameIndex{};
    u4 length{};
    vector<u1> info;
};

struct FieldInfo {
    u2 accessFlags{};
    u2 nameIndex{};
    u2 descriptorIndex{};
    vector<AttributeInfo> attributes;
};

struct MethodInfo {
    u2 accessFlags{};
    u2 nameIndex{};
    u2 descriptorIndex{};
    vector<AttributeInfo> attributes;
};

struct ConstPoolBase {
    ConstantPoolTag tag;
};

struct MemberInfo : ConstPoolBase {
    u2 classIndex;
    u2 nameAndTypeIndex;
};

struct FieldrefInfo : MemberInfo {};
struct MethodrefInfo : MemberInfo {};
struct InterfaceMethodrefInfo : MemberInfo {};

struct Utf8Info : ConstPoolBase {
    u2 length;
    u1 bytes[1];

    std::wstring getValue(bool withEscaped = false) const {
        auto result = stringUtils::utf82wstring(bytes, withEscaped);
        return result;
    }

    std::wstring getValueAsClassname(bool withEscaped = false) const {
        auto result = getValue(withEscaped);
        for (auto &wchar : result) {
            if (wchar == L'/') {
                wchar = L'.';
            }
        }
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
    float value;
};

struct LongInfo : ConstPoolBase {
    union {
        struct ValueParts {
            u4 highBytes;
            u4 lowBytes;
        } valueParts;
        long long value;
    } valueUnion;

};

struct DoubleInfo : ConstPoolBase {
    double value;
};

struct MethodHandleInfo : ConstPoolBase {
    u1 referenceKind;
    u2 referenceIndex;
};

struct InvokeDynamicInfo : ConstPoolBase {
    u2 bootstrapMethodAttrIndex;
    u2 nameAndTypeIndex;
};

struct ConstantPoolRecord {
    union {
        ConstPoolBase base;
        Utf8Info utf8Info;
        ClassInfo classInfo;
        FieldrefInfo fieldrefInfo;
        MethodrefInfo methodrefInfo;
        InterfaceMethodrefInfo interfaceMethodrefInfo;
        MethodTypeInfo methodTypeInfo;
        StringInfo stringInfo;
        FloatInfo floatInfo;
        IntegerInfo integerInfo;
        DoubleInfo doubleInfo;
        LongInfo longInfo;
        MethodHandleInfo methodHandleInfo;
        InvokeDynamicInfo invokeDynamicInfo;
    };
};

struct Descriptor {
    bool isArray;
    bool isClass;
    int dimensions;
    std::wstring type;

    Descriptor() : isArray(false), isClass(false), dimensions(0) {}

    std::wstring toString() const {
        std::wstring result(type);

        for (int i = 0; i < dimensions; i++) {
            result += L"[]";
        }

        return result;
    }
};

struct MethodDescriptor {
    vector<Descriptor> arguments;
    Descriptor returnType;

    std::wstring argumentsToString() const {
        vector<std::wstring> parts;

        for (auto &descriptor : arguments) {
            parts.push_back(descriptor.toString());
        }

        std::wstring result = L"(" + stringUtils::join<std::wstring>(parts, L", ") + L")";

        return result;
    }
};

}
#pragma pack(pop)

#endif // TARRASH_CLASSFILESTRUCTURE_H
