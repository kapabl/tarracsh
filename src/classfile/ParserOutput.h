#ifndef TARRACSH_PARSER_OUTPUT_H
#define TARRACSH_PARSER_OUTPUT_H


#include <filesystem>

#include <string>
#include <vector>
#include "../app/Tarracsh.h"

#include "ClassFileStructures.h"
#include "ClassFileAnalyzer.h"
#include "ConstantPool.h"
#include "AttributesManager.h"
#include "AccessModifiers.h"




namespace org::kapa::tarracsh {

class ParserOutput final {

public:
    explicit ParserOutput(ClassFileAnalyzer& classFileAnalyzer);

    void run();

    ~ParserOutput() = default;


private:
    ClassFileAnalyzer& _classFileAnalyzer;
    accessModifiers::AccessModifiers& _accessModifiers;
    ConstantPool& _constantPool;

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


};
}
#endif 
