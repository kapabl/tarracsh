
#ifndef TARRASH_CLASSFILESTRUCTURE_H
#define TARRASH_CLASSFILESTRUCTURE_H

#pragma pack(push, 1)

typedef uint8_t u1;
typedef uint16_t u2;
typedef uint32_t u4;

struct ClassFileHeader {
    unsigned int magic;
    unsigned short minorVersion;
    unsigned short majorVersion;

};

struct CPInfo {
    ConstantPoolTag tag;
    unsigned char info[];
};

struct ConstPool {
    unsigned short constantPoolCount;
    CPInfo *cpInfo;
};


struct Interfaces {
    unsigned short interfacesCount;
    unsigned short interfaces[];
};

struct ClassFileMid {
    unsigned short accessFlags;
    unsigned short thisClass;
    unsigned short superClass;
    unsigned short interfacesCount;
};

struct AttributeInfo {
    unsigned short attributeNameIndex;
    unsigned int attributeLength;
    unsigned char *info;
};

struct FieldInfo {
    unsigned short accessFlags;
    unsigned short nameIndex;
    unsigned short descriptorIndex;
    unsigned short attributesCount;
    AttributeInfo *attributes;
};

struct Fields {
    unsigned short fields_count;
    FieldInfo *fields;
};

struct MethodInfo {
    unsigned short accessFlags;
    unsigned short nameIndex;
    unsigned short descriptorIndex;
    unsigned short attributesCount;
    AttributeInfo *attributes;
};


struct Methods {
    unsigned short methodsCount;
    MethodInfo *methods;
};


struct ConstPoolStruct {
    ConstantPoolTag tag;
};


struct MemberInfo : ConstPoolStruct {
    u2 classIndex;
    u2 nameAndTypeIndex;
};

struct CONSTANT_Fieldref_info : MemberInfo {
};
struct CONSTANT_Methodref_info : MemberInfo {
};
struct CONSTANT_InterfaceMethodref_info : MemberInfo {
};

struct CONSTANT_Utf8_info : ConstPoolStruct {
    u2 length;
    u1 bytes[1];
};

struct CONSTANT_Class_info : ConstPoolStruct {
    u2 nameIndex;
};


struct CONSTANT_String_info : ConstPoolStruct {
    u2 stringIndex;
};


struct CONSTANT_MethodType_info : ConstPoolStruct {
    u2 descriptorIndex;
};


struct CONSTANT_Integer_info : ConstPoolStruct {
    int value;
};

struct CONSTANT_Float_info : ConstPoolStruct {
    float value;
};

struct CONSTANT_Long_info : ConstPoolStruct {
    union {
        struct {
            u4 highBytes;
            u4 lowBytes;
        };
        long long value;
    };
};

struct CONSTANT_Double_info : ConstPoolStruct {
    double value;
};

struct CONSTANT_MethodHandle_info : ConstPoolStruct {
    u1 referenceKind;
    u2 referenceIndex;
};

struct CONSTANT_InvokeDynamic_info : ConstPoolStruct {
    u2 bootstrapMethodAttrIndex;
    u2 nameAndTypeIndex;
};

struct ConstantPoolRecord {
    union {
        CONSTANT_Utf8_info utf8Info;
        CONSTANT_Class_info classInfo;
        CONSTANT_Fieldref_info fieldrefInfo;
        CONSTANT_Methodref_info methodrefInfo;
        CONSTANT_InterfaceMethodref_info interfaceMethodrefInfo;
        CONSTANT_MethodType_info methodTypeInfo;
        CONSTANT_String_info stringInfo;
        CONSTANT_Float_info floatInfo;
        CONSTANT_Integer_info integerInfo;
        CONSTANT_Double_info doubleInfo;
        CONSTANT_Long_info longInfo;
        CONSTANT_MethodHandle_info methodHandleInfo;
        CONSTANT_InvokeDynamic_info invokeDynamicInfo;
    };
};

#pragma pack(pop)

#endif //TARRASH_CLASSFILESTRUCTURE_H
