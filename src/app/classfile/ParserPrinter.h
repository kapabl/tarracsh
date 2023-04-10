#ifndef TARRACSH_PARSER_OUTPUT_H
#define TARRACSH_PARSER_OUTPUT_H


#include <filesystem>

#include <string>
#include <vector>

#include "./domain/classfile/ClassFileParser.h"
#include "./domain/classfile/AttributesManager.h"
#include "./domain/classfile/AccessModifiers.h"
#include "./domain/classfile/constantpool/ConstantPool.h"


namespace kapa::tarracsh::app::classfile {

using domain::classfile::constantpool::u1;
using domain::classfile::constantpool::u2;
using domain::classfile::constantpool::u4;
using domain::classfile::constantpool::MethodInfo;
using domain::classfile::constantpool::FieldInfo;
using domain::classfile::attribute::AttributeInfo;

class ParserPrinter final {

public:
    explicit ParserPrinter(domain::classfile::ClassFileParser& classFileParser);

    void print();

    ~ParserPrinter() = default;


private:
    domain::classfile::ClassFileParser& _classFileParser;
    domain::classfile::accessmodifier::AccessModifiers& _accessModifiers;
    domain::classfile::constantpool::ConstantPool& _constantPool;

    void outputAccessModifiers(const u2 accessFlags) const;
    void outputMethod(MethodInfo &methodInfo);
    void outputMethods();

    [[nodiscard]] std::string getClassInfoName(const u2 index) const;

    void outputClass(const std::string &type);
    void outputClass();
    std::string attributesToString(std::vector<domain::classfile::attribute::AttributeInfo> &attributes);
    void outputField(FieldInfo &fieldInfo);
    void outputFields();
    void outputInterfaces();


};
}
#endif 
