//
// Created by xman on 6/24/2022.
//

#ifndef TARRASH_CLASSFILEPARSER_H
#define TARRASH_CLASSFILEPARSER_H

#include <bit>
#include "ClassFileStructure.h"
#include "MethodDescriptorParser.h"
#include "DescriptorParser.h"
#include "ConstantPool.h"
#include "StringUtils.h"

namespace org { namespace kapa { namespace tarrash {


class ClassFileParser {

public:
    ClassFileParser(string fileName) : _fileName(fileName), _bytesRead(0) {
        processFile();
    }

    void output() {
        outputMethods();
        outputFields();
        outputInterfaces();
        outputAttributes();
    }

    virtual ~ClassFileParser() {
    }

private:

    vector <string> _accessModifiers = {
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
            "MODULE"
    };


    void outputAccessModifiers(u2 accessFlags) {
        u2 bit = 1;
        vector <string> presentAccessModifiers;
        for (auto &accessModifier: _accessModifiers) {
            if ((accessFlags & bit) != 0) {
                presentAccessModifiers.push_back( toLower( accessModifier ));
            }
            bit = bit << 1;
        }
        cout << join(presentAccessModifiers, " ") << " ";
    }


    void outputMethod(const MethodInfo &methodInfo) {
        outputAccessModifiers(methodInfo.accessFlags);
        auto &utf8Name = _constantPool[methodInfo.nameIndex].utf8Info;
        auto &utf8DDesc = _constantPool[methodInfo.descriptorIndex].utf8Info;

        MethodDescriptorParser methodDescriptorParser( utf8DDesc.getValue() );
        auto& methodDescriptor = methodDescriptorParser.getDescriptor();
        auto returnType = methodDescriptor.returnType.toString();
        auto arguments = methodDescriptor.argumentsToString();

        wcout << returnType << " " << utf8Name.getValue() << arguments;

        /*struct MethodInfo {
            u2 accessFlags;
            u2 nameIndex;
            u2 descriptorIndex;
            u2 attributesCount;
            vector<AttributeInfo> attributes;
        };*/

        cout << endl;
    }

    void outputMethods() {
        cout << "Methods: " << _methods.size() << endl;
        cout << "------------------------------------" << endl;
        for (auto &methodInfo: _methods) {
            outputMethod(methodInfo);
        }
    }

    void outputField( const FieldInfo& fieldInfo ) {
        /**
         * struct FieldInfo {
u2 accessFlags;
u2 nameIndex;
u2 descriptorIndex;
u2 attributesCount;
vector <AttributeInfo> attributes;
}
         */
    }

    void outputFields() {
        cout << "Fields: " << _fields.size() << endl;
        cout << "------------------------------------" << endl;
        for (auto &fieldInfo: _fields) {
            outputField(fieldInfo);
        }
    }

    void outputInterfaces() {
        //TODO
    }

    void outputAttributes() {
        //TODO
    }

    u2 swapShort(u2 value) {
        const u2 result = value >> 8 | value << 8;
        return result;
    }

    u4 swapLong(u4 value) {
        const u4 result =
                value >> 24 |
                (value << 8 & 0x00FF0000) |
                (value >> 8 & 0x0000FF00) |
                value << 24;
        return result;

    }

    template<typename T>
    void readRaw(T &buffer, unsigned int byteCount) {

        assert(_bytesRead + byteCount <= _fileSize);
        char *charBuffer = reinterpret_cast<char *>(&buffer);

        _file.read(charBuffer, byteCount);

        _bytesRead += byteCount;
    }

    template<typename T>
    void readRaw(T &buffer) {
        readRaw(buffer, sizeof(buffer));
    }

    template<typename T>
    void read(T &buffer, unsigned int byteCount) {

        readRaw(buffer, byteCount);
        if (!_isBigEndian) {
            switch (byteCount) {
                case 2:
                    buffer = swapShort(buffer);
                    break;
                case 4:
                    buffer = swapLong(buffer);
                    break;
                default:
                    break;
            }
        }
    }

    template<typename T>
    void read(T &buffer) { read(buffer, sizeof(T)); }

    void readHeader() {
        readRaw(_header, sizeof(ClassFileHeader));
        _isBigEndian = _header.magic == 0x0cafebabe;
        if (!_isBigEndian && _header.magic != swapLong(0x0cafebabe)) {
            cout << "Invalid class file " << _fileName << endl;
            exit(1);
        }
    }


    void readConstPoolRecord() {
        ConstantPoolTag tag;
        readRaw(tag);

        switch (tag) {
            case JVM_CONSTANT_Utf8: {
                u2 length;
                read(length);
                auto recordSize = sizeof(Utf8_info) + length + 1;
                Utf8_info &utf8Info = *reinterpret_cast<Utf8_info *>(malloc(recordSize));
                utf8Info.tag = tag;
                utf8Info.length = length;
                u1 *data = reinterpret_cast<u1 *>(utf8Info.bytes);
                readRaw(*data, utf8Info.length);
                utf8Info.bytes[length] = 0;
                _constantPool.addRecord(utf8Info, recordSize);

                free(&utf8Info);

                break;
            }

            case JVM_CONSTANT_Float:
            case JVM_CONSTANT_Integer: {
                Integer_info integerInfo{tag};
                read(integerInfo.value);
                _constantPool.addRecord(integerInfo);
                break;
            }

            case JVM_CONSTANT_Long:
            case JVM_CONSTANT_Double: {
                Long_info longInfo{tag};
                read(longInfo.value);
                _constantPool.addRecord(longInfo);
                break;
            }

            case JVM_CONSTANT_String:
            case JVM_CONSTANT_MethodType:
            case JVM_CONSTANT_Class: {
                Class_info classInfo{tag};
                read(classInfo.nameIndex);
                _constantPool.addRecord(classInfo);
                break;
            }

            case JVM_CONSTANT_NameAndType:
            case JVM_CONSTANT_Methodref:
            case JVM_CONSTANT_Fieldref:
            case JVM_CONSTANT_InterfaceMethodref: {
                MemberInfo memberInfo{tag};
                read(memberInfo.classIndex);
                read(memberInfo.nameAndTypeIndex);
                _constantPool.addRecord(memberInfo);
                break;
            }

            case JVM_CONSTANT_MethodHandle: {
                MethodHandle_info methodHandleInfo{tag};
                read(methodHandleInfo.referenceKind);
                read(methodHandleInfo.referenceIndex);
                _constantPool.addRecord(methodHandleInfo);
                break;
            }

            case JVM_CONSTANT_InvokeDynamic: {
                InvokeDynamic_info invokeDynamicInfo{tag};
                read(invokeDynamicInfo.bootstrapMethodAttrIndex);
                read(invokeDynamicInfo.nameAndTypeIndex);
                _constantPool.addRecord(invokeDynamicInfo);
                break;
            }

                //case JVM_CONSTANT_ExternalMax:
            case JVM_CONSTANT_Package:
            case JVM_CONSTANT_Dynamic:
            case JVM_CONSTANT_Module:
            case JVM_CONSTANT_Unicode:
                //TODO newer java version
                break;

            default:
                assert(false);
                break;
        }
    }

    void readConstantsPool() {
        u2 count;
        read(count);
        _constantPool.setCount(count);
        int index = 0;
        while (index + 1 < count) {
            readConstPoolRecord();
            index++;
        }
        _constantPool.relocate();
    }

    void readMid() {
        read(_mid.accessFlags);
        read(_mid.thisClass);
        read(_mid.superClass);
    }

    void readInterfaces() {
        u2 count;
        read(count);
        _interfaces.resize(count);
        for (auto i = 0; i < count; ++i) {
            read(_interfaces[i]);
        }
    }

    void readFields() {
        u2 count;
        read(count);

        for (auto i = 0; i < count; ++i) {
            FieldInfo fieldInfo;
            read(fieldInfo.accessFlags);
            read(fieldInfo.nameIndex);
            read(fieldInfo.descriptorIndex);
            read(fieldInfo.attributesCount);
            readAttributesSection(fieldInfo.attributes, fieldInfo.attributesCount);
            _fields.push_back(fieldInfo);
        }
    }


    void readAttributesSection(vector <AttributeInfo> &attributes, int count) {
        for (auto i = 0; i < count; ++i) {
            AttributeInfo attributeInfo;
            read(attributeInfo.nameIndex);
            read(attributeInfo.length);
            for (auto length = 0; length < attributeInfo.length; ++length) {
                u1 byte;
                read(byte);
                attributeInfo.info.push_back(byte);
            }
            attributes.push_back(attributeInfo);

        }
    }

    void readMethods() {
        u2 count;
        read(count);
        for (auto i = 0; i < count; ++i) {
            MethodInfo methodInfo;
            read(methodInfo.accessFlags);
            read(methodInfo.nameIndex);
            read(methodInfo.descriptorIndex);
            read(methodInfo.attributesCount);
            readAttributesSection(methodInfo.attributes, methodInfo.attributesCount);
            _methods.push_back(methodInfo);
        }
    }

    void readAttributes() {
        u2 count;
        read(count);
        readAttributesSection(_attributes, count);
    }

    void processFile() {

        _fileSize = file_size(_fileName);

        _file.open(_fileName, ifstream::binary);
        readHeader();
        readConstantsPool();
        readMid();
        readInterfaces();
        readFields();
        readMethods();
        readAttributes();

        _file.close();


    }


    std::string _fileName;
    bool _isBigEndian;

    ClassFileHeader _header;
    ConstantPool _constantPool;
    ClassFileMid _mid;
    vector <u2> _interfaces;
    vector <FieldInfo> _fields;
    vector <MethodInfo> _methods;
    vector <AttributeInfo> _attributes;

    unsigned int _fileSize;
    unsigned int _bytesRead;

    ifstream _file;

};
}}}

#endif //TARRASH_CLASSFILEPARSER_H
