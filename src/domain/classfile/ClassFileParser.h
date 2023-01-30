#ifndef TARRACSH_CLASSFILE_ANALYZER_H
#define TARRACSH_CLASSFILE_ANALYZER_H
#include <string>
#include <vector>

#include "constantpool/ConstantPool.h"
#include "AttributesManager.h"
#include "AccessModifiers.h"
#include "../stats/Results.h"
#include "reader/ClassFileReader.h"

namespace kapa::tarracsh::domain::classfile {

    using kapa::tarracsh::domain::stats::Results;

class ClassFileParser final {
   

public:
    explicit ClassFileParser(reader::ClassFileReader &reader, Options &options, stats::Results &results);

    ClassFileParser(const ClassFileParser &) = delete;
    ClassFileParser(const ClassFileParser &&) = delete;
    ClassFileParser &operator=(const ClassFileParser &) = delete;
    ClassFileParser &operator=(const ClassFileParser &&) = delete;

    [[nodiscard]] bool isValidHeader() const { return _reader.isValidHeader(); }
    [[nodiscard]] bool succeeded() const { return _parseSucceed; }

    ~ClassFileParser() = default;

    [[nodiscard]] bool parse();
    [[nodiscard]] attribute::AttributesManager &getAttributesManager() { return _attributesManager; }
    [[nodiscard]] accessmodifier::AccessModifiers &getAccessModifiers() { return _accessModifiers; }
    [[nodiscard]] constantpool::ConstantPool &getConstantPool() { return _constantPool; }
    [[nodiscard]] const constantpool::ConstantPool &getConstantPool() const { return _constantPool; }
    [[nodiscard]] std::vector<constantpool::MethodInfo> &getMethods() { return _methods; }
    [[nodiscard]] std::vector<constantpool::u2> &getInterfaces() { return _interfaces; }
    [[nodiscard]] constantpool::MainClassInfo &getMainClassInfo() { return _mainClassInfo; }
    [[nodiscard]] constantpool::u2 getSuperClassIndex() const { return _mainClassInfo.superClass; }
    [[nodiscard]] std::vector<attribute::AttributeInfo> &getAttributes() { return _attributes; }
    [[nodiscard]] std::vector<constantpool::FieldInfo> &getFields() { return _fields; }

    [[nodiscard]] std::string getMainClassname() const;

    Options& getOptions() { return _options; }
    [[nodiscard]] std::string getContainingFile();

private:
    Options _options;
    Results &_results;
    reader::ClassFileReader &_reader;
    bool _parseSucceed{ false };

    constantpool::ConstantPool _constantPool;
    constantpool::MainClassInfo _mainClassInfo{};
    std::vector<constantpool::u2> _interfaces;
    std::vector<constantpool::FieldInfo> _fields;
    std::vector<constantpool::MethodInfo> _methods;
    std::vector<attribute::AttributeInfo> _attributes;

    attribute::AttributesManager _attributesManager;
    accessmodifier::AccessModifiers _accessModifiers;

    bool internalParse();

    void initialize();
    void readConstPoolEntry(int &index);
    void readConstantsPool();
    void readMainClassInfo();
    void readInterfaces();
    void readFields();
    void readAttributesSection(std::vector<attribute::AttributeInfo> &attributes, const int count,
                               const attribute::AttributeOwner owner);
    void readMethods();
    void readAttributes();
    void processFile();

};


}
#endif
