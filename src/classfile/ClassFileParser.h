//
// Created by xman on 6/24/2022.
//

#ifndef TARRASH_CLASSFILEPARSER_H
#define TARRASH_CLASSFILEPARSER_H

#include <cassert>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

#include "ClassFileStructures.h"
#include "ConstantPool.h"
#include "AttributesManager.h"
#include "AccessModifiers.h"
#include "StringUtils.h"


namespace org::kapa::tarrash {

class ClassFileParser final {

public:
    explicit ClassFileParser(std::string fileName, std::string classPath);

    ClassFileParser(const ClassFileParser &) = delete;
    ClassFileParser(const ClassFileParser &&) = delete;
    ClassFileParser &operator=(const ClassFileParser &) = delete;
    ClassFileParser &operator=(const ClassFileParser &&) = delete;

    void output();

    [[nodiscard]] bool isValid() const { return _isValid; }

    ~ClassFileParser() = default;
    void run();

private:
    std::string _fileName;
    std::string _classPath;
    bool _isBigEndian{true};

    ClassFileHeader _header{};
    ConstantPool _constantPool;
    MainClassInfo _mainClassInfo{};
    std::vector<u2> _interfaces;
    std::vector<FieldInfo> _fields;
    std::vector<MethodInfo> _methods;
    std::vector<attributes::AttributeInfo> _attributes;

    uint64_t _fileSize{};
    unsigned int _bytesRead{};

    std::ifstream _file;
    bool _isValid = true;
    attributes::AttributesManager _attributesManager;
    accessModifiers::AccessModifiers _accessModifiers;

    void outputAccessModifiers(const u2 accessFlags) const;
    void outputMethod(MethodInfo &methodInfo);
    void outputMethods();

    [[nodiscard]] std::wstring getClassInfoName(const u2 index) const;

    void outputClass(const std::wstring &type);
    void outputClass();
    std::wstring attributesToString(std::vector<attributes::AttributeInfo> &attributes);
    void outputField(FieldInfo &fieldInfo);
    void outputFields();
    void outputInterfaces();

    template <typename T> void readRaw(T &buffer, unsigned int byteCount) {

        assert(_bytesRead + byteCount <= _fileSize);
        const auto charBuffer = reinterpret_cast<char *>(&buffer);

        _file.read(charBuffer, byteCount);

        _bytesRead += byteCount;
    }

    template <typename T> void readRaw(T &buffer) { readRaw(buffer, sizeof(buffer)); }

    template <typename T = u2>
    void read(u2 &buffer) {
        readRaw(buffer, 2);
        if (!_isBigEndian) {
            buffer = stringUtils::swapShort(buffer);
        }
    }

    template <typename T = u4>
    void read(u4 &buffer) {
        readRaw(buffer, 4);
        if (!_isBigEndian) {
            buffer = stringUtils::swapLong(buffer);
        }
    }

    u2 readU2();
    u4 readU4();
    u1 readU1();
    bool readHeader();
    void readConstPoolRecord();
    void readConstantsPool();
    void readMainClassInfo();
    void readInterfaces();
    void readFields();
    void readAttributesSection(std::vector<attributes::AttributeInfo> &attributes, const int count,
                               const attributes::AttributeOwner owner);
    void readMethods();
    void readAttributes();
    void processFile();

};
}
#endif 
