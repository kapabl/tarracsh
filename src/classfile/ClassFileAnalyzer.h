#ifndef TARRACSH_CLASSFILEPARSER_H
#define TARRACSH_CLASSFILEPARSER_H
#include "../app/Tarracsh.h"
#include <string>
#include <vector>


#include "constpool/ConstantPool.h"
#include "AttributesManager.h"
#include "AccessModifiers.h"
#include "../app/stats/Stats.h"
#include "readers/ClassFileReader.h"
#include "../tables/ClassfilesTable.h"


namespace org::kapa::tarracsh {

class ClassFileAnalyzer final {

public:
    explicit ClassFileAnalyzer(readers::ClassFileReader &reader, Options &options, stats::Results &results);

    ClassFileAnalyzer(const ClassFileAnalyzer &) = delete;
    ClassFileAnalyzer(const ClassFileAnalyzer &&) = delete;
    ClassFileAnalyzer &operator=(const ClassFileAnalyzer &) = delete;
    ClassFileAnalyzer &operator=(const ClassFileAnalyzer &&) = delete;

    [[nodiscard]] bool isValid() const { return _reader.isValid(); }

    ~ClassFileAnalyzer() = default;
    [[nodiscard]] bool analyze();
    [[nodiscard]] std::optional<db::tables::columns::DigestCol> getPublicDigest();
    [[nodiscard]] attributes::AttributesManager &getAttributesManager() { return _attributesManager; }
    [[nodiscard]] accessModifiers::AccessModifiers &getAccessModifiers() { return _accessModifiers; }
    [[nodiscard]] ConstantPool &getConstantPool() { return _constantPool; }
    [[nodiscard]] const ConstantPool &getConstantPool() const { return _constantPool; }
    [[nodiscard]] std::vector<MethodInfo> &getMethods() { return _methods; }
    [[nodiscard]] std::vector<u2> &getInterfaces() { return _interfaces; }
    [[nodiscard]] MainClassInfo &getMainClassInfo() { return _mainClassInfo; }
    [[nodiscard]] u2 getSuperClassIndex() const { return _mainClassInfo.superClass; }
    [[nodiscard]] std::vector<attributes::AttributeInfo> &getAttributes() { return _attributes; }
    [[nodiscard]] std::vector<FieldInfo> &getFields() { return _fields; }

    [[nodiscard]] std::string getMainClassname() const;

    Options& getOptions() { return _options; }
    [[nodiscard]] std::string getContainingFile() const;

private:
    Options _options;
    stats::Results &_results;
    readers::ClassFileReader &_reader;

    ConstantPool _constantPool;
    MainClassInfo _mainClassInfo{};
    std::vector<u2> _interfaces;
    std::vector<FieldInfo> _fields;
    std::vector<MethodInfo> _methods;
    std::vector<attributes::AttributeInfo> _attributes;

    attributes::AttributesManager _attributesManager;
    accessModifiers::AccessModifiers _accessModifiers;

    bool internalAnalyze();

    void initialize();
    void readConstPoolEntry(int &index);
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
