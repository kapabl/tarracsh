#ifndef TARRACSH_CONSTANT_POOL_PRINTER_H
#define TARRACSH_CONSTANT_POOL_PRINTER_H


#include <cstring>
#include <map>
#include <unordered_map>
#include <yaml-cpp/node/node.h>
#include "../ClassFileAnalyzer.h"

namespace org::kapa::tarracsh {
class ClassFileAnalyzer;

class ConstantPoolPrinter {
public:
    explicit ConstantPoolPrinter(const ClassFileAnalyzer &classFileAnalyzer);
    void printTitle();
    ~ConstantPoolPrinter() = default;

    void printToConsole();
    void printToHtmlNav();

    static void init(const std::string& templateType);
    typedef std::unordered_map<std::string, std::string> OutputSubTemplates;


private:
    const ClassFileAnalyzer &_classFileAnalyzer;
    const ConstantPool &_constantPool;
    std::string _currentLine;
    static std::string _templateType;

    static std::vector<std::string> _poolTagToString;
    static std::vector<std::string> _refKindToString;

    std::vector<std::string> _outputLines;


    static std::unordered_map<std::string, std::string> _templateFragments;

    static void readSubTemplates();

    static std::string tagToString(ConstantPoolTag tag);
    static std::string refKindToString(MethodHandleSubtypes tag);
    void printHeader(const ConstPoolBase &entry, int index);
    void printUtf8Info(const Utf8Info &entry, int index);
    void printStringInfo(const StringInfo &entry, int index);
    void printLongInfo(const LongInfo &entry, int index);
    void printDoubleInfo(const DoubleInfo &entry, int index);
    void printIntegerInfo(const IntegerInfo &entry, int index);
    void printFloatInfo(const FloatInfo &entry, int index);
    void printClassInfo(const ClassInfo &entry, int index);
    void printMethodrefInfo(const MethodrefInfo &entry, int index);
    void printMethodHandleInfo(const MethodHandleInfo &entry, int index);
    void printMethodTypeInfo(const MethodTypeInfo &entry, int index);
    void printFieldrefInfo(const FieldrefInfo &entry, int index);
    void printModuleInfo(const ModuleInfo &entry, int index);
    void printRefExtraInfo(const MemberInfo &entry);
    void printInterfaceMethodrefInfo(const InterfaceMethodrefInfo &entry, int index);
    void printInvokeDynamicInfo(const InvokeDynamicInfo &entry, int index);
    void printNameAndTypeInfo(const NameAndTypeInfo &entry, int index);
    void printEntry(const ConstantPoolRecord &entry, int index);
    static void initStringMaps();

    [[nodiscard]] std::filesystem::path getClassHtmlIndexFilename() const;
    const char *getImplLinks();
    std::vector<std::string> renderHtmlClassIndex();
    void mainClassToHtmlIndex();
    [[nodiscard]] std::string getNavClassRelDir() const;
    [[nodiscard]] std::string getNavImplRelDir() const;
    [[nodiscard]] std::filesystem::path getHtmlCPoolFilename() const;
    [[nodiscard]] std::vector<std::string> renderCPoolHtml(const std::vector<std::string>& lines) const;
    void linesToHtmlFile(const std::vector<std::string>& lines);


};


}
#endif //TARRACSH_CONSTANT_POOL_PRINTER_H
