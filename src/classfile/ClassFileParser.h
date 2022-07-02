//
// Created by xman on 6/24/2022.
//

#ifndef TARRASH_CLASSFILEPARSER_H
#define TARRASH_CLASSFILEPARSER_H

#include <bit>
#include <cassert>
// #include <cstdint>
#include <filesystem>
#include <fstream>
// #include <cinttypes>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "includes/classfile_constants.h"
#include "ClassFileStructures.h"
#include "ConstantPool.h"
#include "DescriptorParser.h"
#include "MethodDescriptorParser.h"
#include "AttributesManager.h"
#include "AccessModifiers.h"
#include "StringUtils.h"

using namespace std;

namespace org {
namespace kapa {
namespace tarrash {

class ClassFileParser {

public:
    ClassFileParser(string fileName)
        : _attributesManager(_constantPool, _isBigEndian),
          _fileName(std::move(fileName)),
          _bytesRead(0) {

        processFile();
    }

    void output() {
        outputClass();
        outputMethods();
        outputFields();
        outputInterfaces();
    }


    bool isValid() { return _isValid; }

    ~ClassFileParser() = default;

private:
    attributes::AttributesManager _attributesManager;
    accessModifiers::AccessModifiers _accessModifiers;

    void outputAccessModifiers(const u2 accessFlags) const { wcout << _accessModifiers.toString(accessFlags) << " "; }

    void outputMethod(MethodInfo &methodInfo) {
        const auto accessModifiers = _accessModifiers.toString(methodInfo.accessFlags);
        const auto name = _constantPool[methodInfo.nameIndex].utf8Info.getValue();
        const auto &utf8DDesc = _constantPool[methodInfo.descriptorIndex].utf8Info;

        MethodDescriptorParser methodDescriptorParser(utf8DDesc.getValue());
        auto &methodDescriptor = methodDescriptorParser.getDescriptor();
        const auto returnType = methodDescriptor.returnType.toString();
        const auto arguments = methodDescriptor.argumentsToString();

        const auto attributesString = attributesToString(methodInfo.attributes);

        const vector parts{attributesString, accessModifiers, returnType, name + arguments};

        wcout << join<wstring>(parts, L" ") << L";";
        cout << endl;
    }

    void outputMethods() {
        cout << endl;
        cout << "//Methods: " << _methods.size() << endl;
        cout << "//-----------------------------------" << endl;
        for (auto &methodInfo : _methods) {
            outputMethod(methodInfo);
            cout << endl;
        }
        cout << endl;
    }

    wstring getClassInfoName(const u2 index) const {
        const Class_info &classInfo = _constantPool[index].classInfo;
        auto &classname = _constantPool[classInfo.nameIndex].utf8Info;
        auto result = classname.getValueAsClassname();
        return result;
    }

    void outputClass(const wstring &type) {
        cout << endl;
        cout << "//Class " << endl;
        cout << "//------------------------------------" << endl;
        const auto accessModifiers = _accessModifiers.toString(_mainClassInfo.accessFlags);

        const auto attributesString = attributesToString(_attributes);

        wcout << attributesString << accessModifiers << L" " << type << L" "
            << getClassInfoName(_mainClassInfo.thisClass);

        if (_mainClassInfo.superClass != 0) {
            wcout << L" extends " << getClassInfoName(_mainClassInfo.superClass);
        }

        // TODO implemented interfaces
    }

    void outputClass() {

        outputClass(_mainClassInfo.isInterface() ? L"interface" : L"class");
        cout << endl;
    }

    wstring attributesToString(vector<AttributeInfo> &attributes) {

        vector<wstring> parts;

        parts.reserve(attributes.size());
        for (auto &attribute : attributes) {
            parts.push_back(_attributesManager.toString(attribute));
        }

        auto result = join<wstring>(parts, L"\n\n");
        if (!result.empty()) {
            result = L"/*\n" + result + L"\n*/\n";
        }
        return result;
    }

    void outputField(FieldInfo &fieldInfo) {
        const auto accessModifiers = _accessModifiers.toString(fieldInfo.accessFlags);
        const auto name = _constantPool[fieldInfo.nameIndex].utf8Info.getValue();
        const auto descriptorString = _constantPool[fieldInfo.descriptorIndex].utf8Info.getValue();
        const auto descriptor = DescriptorParser(descriptorString).getDescriptor();

        const auto attributeString = attributesToString(fieldInfo.attributes);

        const vector parts{attributeString, accessModifiers, descriptor.toString(), name};

        wcout << join<wstring>(parts, L" ") << ";";

    }

    void outputFields() {
        cout << endl;
        cout << "//Fields: " << _fields.size() << endl;
        cout << "//------------------------------------" << endl;
        for (auto &fieldInfo : _fields) {
            outputField(fieldInfo);
            cout << endl << endl;
        }
        cout << endl;
    }

    void outputInterfaces() {
        // TODO
    }


    template <typename T> void readRaw(T &buffer, unsigned int byteCount) {

        assert(_bytesRead + byteCount <= _fileSize);
        const auto charBuffer = reinterpret_cast<char *>(&buffer);

        _file.read(charBuffer, byteCount);

        _bytesRead += byteCount;
    }

    template <typename T> void readRaw(T &buffer) { readRaw(buffer, sizeof(buffer)); }

    template <typename T> void read(T &buffer, unsigned int byteCount) {

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

    template <typename T> void read(T &buffer) { read(buffer, sizeof(T)); }

    u2 readU2() {
        u2 result;
        read(result);
        return result;
    }

    u1 readU1() {
        u1 result;
        read(result);
        return result;
    }

    bool readHeader() {
        readRaw(_header, sizeof(ClassFileHeader));
        _isBigEndian = _header.magic == 0x0cafebabe;
        if (!_isBigEndian && _header.magic != swapLong(0x0cafebabe)) {
            cout << "Invalid class file " << _fileName << endl;
            _isValid = false;
        }

        return _isValid;
    }

    void readConstPoolRecord() {
        ConstantPoolTag tag;
        readRaw(tag);

        switch (tag) {
            case JVM_CONSTANT_Utf8: {
                u2 length;
                read(length);
                const auto recordSize = sizeof(Utf8_info) + length + 1;
                Utf8_info &utf8Info = *static_cast<Utf8_info *>(malloc(recordSize));
                utf8Info.tag = tag;
                utf8Info.length = length;
                const auto data = reinterpret_cast<u1 *>(utf8Info.bytes);
                readRaw(*data, utf8Info.length);
                utf8Info.bytes[length] = 0;
                _constantPool.addRecord(utf8Info, recordSize);

                free(&utf8Info);

                break;
            }

            case JVM_CONSTANT_Float:
            case JVM_CONSTANT_Integer: {
                Integer_info integerInfo{{tag}};
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

            // case JVM_CONSTANT_ExternalMax:
            case JVM_CONSTANT_Package:
            case JVM_CONSTANT_Dynamic:
            case JVM_CONSTANT_Module:
            case JVM_CONSTANT_Unicode:
                // TODO newer java version
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

    void readClassInfo() {
        read(_mainClassInfo.accessFlags);
        read(_mainClassInfo.thisClass);
        read(_mainClassInfo.superClass);
    }

    void readInterfaces() {
        const auto count = readU2();
        _interfaces.resize(count);
        for (auto i = 0; i < count; ++i) {
            read(_interfaces[i]);
        }
    }

    void readFields() {
        const auto count = readU2();

        for (auto i = 0; i < count; ++i) {
            FieldInfo fieldInfo;
            read(fieldInfo.accessFlags);
            read(fieldInfo.nameIndex);
            read(fieldInfo.descriptorIndex);
            const auto attributeCount = readU2();
            readAttributesSection(fieldInfo.attributes, attributeCount);
            _fields.push_back(fieldInfo);
        }
    }

    void readAttributesSection(vector<AttributeInfo> &attributes, const int count) {
        for (auto i = 0; i < count; ++i) {
            AttributeInfo attributeInfo;
            read(attributeInfo.nameIndex);
            read(attributeInfo.length);
            for (auto length = 0u; length < attributeInfo.length; ++length) {
                const auto byte = readU1();
                attributeInfo.info.push_back(byte);
            }
            attributes.push_back(attributeInfo);
        }
    }

    void readMethods() {
        const auto count = readU2();;
        for (auto i = 0; i < count; ++i) {
            MethodInfo methodInfo;
            read(methodInfo.accessFlags);
            read(methodInfo.nameIndex);
            read(methodInfo.descriptorIndex);

            const auto attributeCount = readU2();
            readAttributesSection(methodInfo.attributes, attributeCount);
            _methods.push_back(methodInfo);
        }
    }

    void readAttributes() {
        const auto count = readU2();
        readAttributesSection(_attributes, count);
    }

    void processFile() {

        _fileSize = file_size(_fileName);

        _file.open(_fileName, ifstream::binary);

        if (readHeader()) {
            readConstantsPool();
            readClassInfo();
            readInterfaces();
            readFields();
            readMethods();
            readAttributes();
        }

        _file.close();
    }

    std::string _fileName;
    bool _isBigEndian;

    ClassFileHeader _header;
    ConstantPool _constantPool;
    ClassInfo _mainClassInfo;
    vector<u2> _interfaces;
    vector<FieldInfo> _fields;
    vector<MethodInfo> _methods;
    vector<AttributeInfo> _attributes;

    unsigned int _fileSize;
    unsigned int _bytesRead;

    ifstream _file;
    bool _isValid = true;
};
} // namespace tarrash
} // namespace kapa
} // namespace org

#endif // TARRASH_CLASSFILEPARSER_H
