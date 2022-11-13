#ifndef TARRACSH_CLASSFILEPARSER_H
#define TARRACSH_CLASSFILEPARSER_H

#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include "Tarracsh.h"

#include "ClassFileStructures.h"
#include "ConstantPool.h"
#include "AttributesManager.h"
#include "AccessModifiers.h"
#include "Stats.h"
#include "readers/ClassFileReader.h"
#include "tables/ClassfilesTable.h"


namespace org::kapa::tarracsh {

class ClassFileAnalyzer final {

public:
    explicit ClassFileAnalyzer(readers::ClassFileReader& reader, Options &options, stats::Results &results);

    ClassFileAnalyzer(const ClassFileAnalyzer &) = delete;
    ClassFileAnalyzer(const ClassFileAnalyzer &&) = delete;
    ClassFileAnalyzer &operator=(const ClassFileAnalyzer &) = delete;
    ClassFileAnalyzer &operator=(const ClassFileAnalyzer &&) = delete;

    [[nodiscard]] bool isValid() const { return _reader.isValid(); }

    ~ClassFileAnalyzer() = default;
    bool run();
    std::optional<tables::Md5Column> getPublicDigest();
    attributes::AttributesManager &getAttributesManager() { return _attributesManager; }
    accessModifiers::AccessModifiers &getAccessModifiers() { return _accessModifiers; }
    ConstantPool &getConstantPool() { return _constantPool; }
    std::vector<MethodInfo> &getMethods() { return _methods; }
    std::vector<u2> &getInterfaces() { return _interfaces; }
    MainClassInfo &getMainClassInfo() { return _mainClassInfo; }
    std::vector<attributes::AttributeInfo> &getAttributes() { return _attributes; }
    std::vector<FieldInfo> &getFields() { return _fields; }

    [[nodiscard]] std::wstring getMainClassname() const {
        std::wstring result = _constantPool.getClassInfoName(_mainClassInfo.thisClass);
        return result;
    }

private:
    Options _options;
    stats::Results &_results;
    readers::ClassFileReader& _reader;

    ConstantPool _constantPool;
    MainClassInfo _mainClassInfo{};
    std::vector<u2> _interfaces;
    std::vector<FieldInfo> _fields;
    std::vector<MethodInfo> _methods;
    std::vector<attributes::AttributeInfo> _attributes;

    attributes::AttributesManager _attributesManager;
    accessModifiers::AccessModifiers _accessModifiers;


    [[nodiscard]] std::wstring getClassInfoName(const u2 index) const;

    void initialize();
    void readConstPoolEntry( int& index);
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
