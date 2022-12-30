#ifndef TARRACSH_CONSTANT_POOL_PRINTER_H
#define TARRACSH_CONSTANT_POOL_PRINTER_H
#include <unordered_map>
#include "../ClassFileAnalyzer.h"


namespace org::kapa::tarracsh {

class ConstantPoolPrinter {
public:
    explicit ConstantPoolPrinter(const ClassFileAnalyzer &classFileAnalyzer);
    virtual ~ConstantPoolPrinter() = default;

    virtual void print();

    static void init();
    typedef std::unordered_map<std::string, std::string> OutputSubTemplates;


protected:
    const ClassFileAnalyzer &_classFileAnalyzer;
    const ConstantPool &_constantPool;
    std::string _currentLine;

    static std::vector<std::string> _poolTagToString;
    static std::vector<std::string> _refKindToString;

    std::vector<std::string> _outputLines;


    static std::string tagToString(ConstantPoolTag tag);
    static std::string refKindToString(MethodHandleSubtypes tag);
    virtual void printHeader(const ConstPoolBase &entry, int index);
    virtual void printUtf8Info(const Utf8Info &entry, int index);
    virtual void printStringInfo(const StringInfo &entry, int index);
    virtual void printLongInfo(const LongInfo &entry, int index);
    virtual void printDoubleInfo(const DoubleInfo &entry, int index);
    virtual void printIntegerInfo(const IntegerInfo &entry, int index);
    virtual void printFloatInfo(const FloatInfo &entry, int index);
    virtual void printClassInfo(const ClassInfo &entry, int index);
    virtual void printMethodrefInfo(const MethodrefInfo &entry, int index);
    virtual void printMethodHandleInfo(const MethodHandleInfo &entry, int index);
    virtual void printMethodTypeInfo(const MethodTypeInfo &entry, int index);
    virtual void printFieldrefInfo(const FieldrefInfo &entry, int index);
    virtual void printModuleInfo(const ModuleInfo &entry, int index);
    virtual void printRefExtraInfo(const MemberInfo &entry);
    virtual void printInterfaceMethodrefInfo(const InterfaceMethodrefInfo &entry, int index);
    virtual void printInvokeDynamicInfo(const InvokeDynamicInfo &entry, int index);
    virtual void printNameAndTypeInfo(const NameAndTypeInfo &entry, int index);
    virtual void printEntry(const ConstantPoolRecord &entry, int index);
    virtual void printTitle();

    static void initStringMaps();


};


}
#endif //TARRACSH_CONSTANT_POOL_PRINTER_H
