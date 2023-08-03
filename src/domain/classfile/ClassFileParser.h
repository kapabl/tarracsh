#ifndef TARRACSH_CLASSFILE_PARSER_H
#define TARRACSH_CLASSFILE_PARSER_H
#include <string>
#include <vector>

#include "AccessModifiers.h"
#include "AttributesManager.h"
#include "constantpool/ConstantPool.h"
#include "infrastructure/log/Log.h"
#include "reader/ClassFileReader.h"

namespace kapa::tarracsh::domain::classfile {


class ClassFileParser final {


public:
    explicit ClassFileParser(reader::ClassFileReader &reader, std::string filename, std::shared_ptr<infrastructure::log::Log> log);
    ~ClassFileParser() = default;

    ClassFileParser(const ClassFileParser &) = delete;
    ClassFileParser(const ClassFileParser &&) = delete;
    ClassFileParser &operator=(const ClassFileParser &) = delete;
    ClassFileParser &operator=(const ClassFileParser &&) = delete;

    [[nodiscard]] auto isValidHeader() const -> bool { return _reader.isValidHeader(); }
    [[nodiscard]] auto succeeded() const -> bool { return _parseSucceed; }
    [[nodiscard]] auto parse() -> bool;
    [[nodiscard]] auto getAttributesManager() -> attribute::AttributesManager& { return _attributesManager; }
    [[nodiscard]] auto getConstantPool() -> constantpool::ConstantPool& { return _constantPool; }
    [[nodiscard]] auto getConstantPool() const -> const constantpool::ConstantPool& { return _constantPool; }
    [[nodiscard]] auto getMethods() -> std::vector<constantpool::MethodInfo>& { return _methods; }
    [[nodiscard]] auto getInterfaces() -> std::vector<constantpool::u2>& { return _interfaces; }
    [[nodiscard]] auto getMainClassInfo() -> constantpool::MainClassInfo& { return _mainClassInfo; }
    [[nodiscard]] auto getSuperClassIndex() const -> constantpool::u2 { return _mainClassInfo.superClass; }
    [[nodiscard]] auto getAttributes() -> std::vector<attribute::AttributeInfo>& { return _attributes; }
    [[nodiscard]] auto getFields() -> std::vector<constantpool::FieldInfo>& { return _fields; }

    [[nodiscard]] auto getMainClassname() const -> std::string;

    // Options& getOptions() { return _options; }
    [[nodiscard]] std::string getFilename();

private:
    std::string _filename;
    std::shared_ptr<infrastructure::log::Log> _log;
    reader::ClassFileReader &_reader;
    bool _parseSucceed{ false };

    constantpool::ConstantPool _constantPool;
    constantpool::MainClassInfo _mainClassInfo{};
    std::vector<constantpool::u2> _interfaces;
    std::vector<constantpool::FieldInfo> _fields;
    std::vector<constantpool::MethodInfo> _methods;
    std::vector<attribute::AttributeInfo> _attributes;
    attribute::AttributesManager _attributesManager;


    auto internalParse() -> bool;

    void initialize();
    void readConstPoolEntry(int &index);
    void readConstantsPool();
    void readMainClassInfo();
    void readInterfaces();
    void readFields();
    void readAttributesSection(std::vector<attribute::AttributeInfo> &attributes, int count,
                               attribute::AttributeOwner owner);
    void readMethods();
    void readAttributes();
    void processFile();

};


}
#endif
