
#ifndef TARRASH_CLASSFILESTRUCTURE_H
#define TARRASH_CLASSFILESTRUCTURE_H

#include "StructsCommon.h"
#include "StackFrame.h"
#include "AttributeStructures.h"
#include "StringUtils.h"

#pragma pack(push, 1)

namespace org {
namespace kapa {
namespace tarrash {

struct ClassFileHeader {
    u4 magic;
    u2 minorVersion;
    u2 majorVersion;
};

struct ClassInfo {
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
    u2 accessFlags;
    u2 nameIndex;
    u2 descriptorIndex;
    vector<AttributeInfo> attributes;
};

struct MethodInfo {
    u2 accessFlags;
    u2 nameIndex;
    u2 descriptorIndex;
    vector<AttributeInfo> attributes;
};

struct ConstPoolBase {
    ConstantPoolTag tag;
};

struct MemberInfo : ConstPoolBase {
    u2 classIndex;
    u2 nameAndTypeIndex;
};

struct Fieldref_info : MemberInfo {};
struct Methodref_info : MemberInfo {};
struct InterfaceMethodref_info : MemberInfo {};

struct Utf8_info : ConstPoolBase {
    u2 length;
    u1 bytes[1];

    wstring getValue() const {
        auto result = stringUtils::utf82wstring(bytes);
        return result;
    }

    wstring getValueAsClassname() const {
        auto result = getValue();
        for (auto &wchar : result) {
            if (wchar == L'/') {
                wchar = L'.';
            }
        }
        return result;
    }
};

struct Class_info : ConstPoolBase {
    u2 nameIndex;
};

struct String_info : ConstPoolBase {
    u2 stringIndex;
};

struct MethodType_info : ConstPoolBase {
    u2 descriptorIndex;
};

struct Integer_info : ConstPoolBase {
    int value;
};

struct Float_info : ConstPoolBase {
    float value;
};

struct Long_info : ConstPoolBase {
    union {
        struct {
            u4 highBytes;
            u4 lowBytes;
        };
        long long value;
    };
};

struct Double_info : ConstPoolBase {
    double value;
};

struct MethodHandle_info : ConstPoolBase {
    u1 referenceKind;
    u2 referenceIndex;
};

struct InvokeDynamic_info : ConstPoolBase {
    u2 bootstrapMethodAttrIndex;
    u2 nameAndTypeIndex;
};

struct ConstantPoolRecord {
    union {
        ConstPoolBase base;
        Utf8_info utf8Info;
        Class_info classInfo;
        Fieldref_info fieldrefInfo;
        Methodref_info methodrefInfo;
        InterfaceMethodref_info interfaceMethodrefInfo;
        MethodType_info methodTypeInfo;
        String_info stringInfo;
        Float_info floatInfo;
        Integer_info integerInfo;
        Double_info doubleInfo;
        Long_info longInfo;
        MethodHandle_info methodHandleInfo;
        InvokeDynamic_info invokeDynamicInfo;
    };
};

struct Descriptor {
    bool isArray;
    bool isClass;
    int dimensions;
    wstring type;

    Descriptor() : isArray(false), isClass(false), dimensions(0) {}

    wstring toString() const {
        wstring result(type);

        for (int i = 0; i < dimensions; i++) {
            result += L"[]";
        }

        return result;
    }
};

struct MethodDescriptor {
    vector<Descriptor> arguments;
    Descriptor returnType;

    wstring argumentsToString() const {
        vector<wstring> parts;

        for (auto &descriptor : arguments) {
            parts.push_back(descriptor.toString());
        }

        wstring result = L"(" + stringUtils::join<wstring>(parts, L", ") + L")";

        return result;
    }
};

} // namespace tarrash
} // namespace kapa
} // namespace org
#pragma pack(pop)

#endif // TARRASH_CLASSFILESTRUCTURE_H
