#ifndef TARRACSH_CLASSFILEPARSER_H
#define TARRACSH_CLASSFILEPARSER_H

#include <cassert>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include "Tarracsh.h"

#include "ClassFileStructures.h"
#include "ConstantPool.h"
#include "AttributesManager.h"
#include "AccessModifiers.h"
#include "StringUtils.h"

#include "tables/PublicShaTable.h"


namespace org::kapa::tarracsh {

class ClassFileAnalyzer final {

public:
    explicit ClassFileAnalyzer(Options &options, Results &results);

    ClassFileAnalyzer(const ClassFileAnalyzer &) = delete;
    ClassFileAnalyzer(const ClassFileAnalyzer &&) = delete;
    ClassFileAnalyzer &operator=(const ClassFileAnalyzer &) = delete;
    ClassFileAnalyzer &operator=(const ClassFileAnalyzer &&) = delete;

    void output();

    [[nodiscard]] bool isValid() const { return _isValid; }

    ~ClassFileAnalyzer() = default;
    bool run();
    tables::Sha256 calculatePublicSha();
    std::optional<tables::Sha256> getPublicSha();
    attributes::AttributesManager &getAttributesManager() { return _attributesManager; }
    accessModifiers::AccessModifiers &getAccessModifiers() { return _accessModifiers; }
    ConstantPool &getConstantPool() { return _constantPool; }
    std::vector<MethodInfo> &getMethods() { return _methods; }
    MainClassInfo &getMainClassInfo() { return _mainClassInfo; }
    std::vector<attributes::AttributeInfo> &getAttributes() { return _attributes; }
    std::vector<FieldInfo> &getFields() { return _fields; }

    [[nodiscard]] uint64_t getFileSize() const { return _fileSize; }
    [[nodiscard]] std::filesystem::file_time_type getFileModifiedDate() const { return _lastWriteTime; }

    [[nodiscard]] uint64_t getFileModifiedTimestamp() const {
        const auto result = std::chrono::duration_cast<std::chrono::microseconds>(_lastWriteTime.time_since_epoch()).count();
        return result;
    }

    [[nodiscard]] std::wstring getMainClassname() const {
        std::wstring result = _constantPool.getClassInfoName(_mainClassInfo.thisClass);
        return result;
    }

private:
    Options _options;
    Results &_results;
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
    std::filesystem::file_time_type _lastWriteTime;


    [[nodiscard]] std::wstring getClassInfoName(const u2 index) const;

    template <typename T> void readRaw(T &buffer, unsigned int byteCount) {

        // assert(_bytesRead + byteCount <= _fileSize);

        if (_bytesRead + byteCount > _fileSize) {
            const auto errorMessage = std::format("Error - reading beyond size - {}", _options.classFile);
            _results.resultLog.writeln(errorMessage);
            throw std::runtime_error(errorMessage);
        }
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

    template <typename T = u2>
    void readReversed(u2& buffer) {
        readRaw(buffer, 2);
        if (_isBigEndian) {
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
    u2 readU2Reversed();
    u4 readU4();
    u1 readU1();
    bool readHeader();
    void readConstPoolEntry( int& index);
    void readConstantsPool();
    void readMainClassInfo();
    void readInterfaces();
    void readFields();
    void readAttributesSection(std::vector<attributes::AttributeInfo> &attributes, const int count,
                               const attributes::AttributeOwner owner);
    void readMethods();
    void readAttributes();
    void getFileInfo();
    void processFile();

};
}
#endif
