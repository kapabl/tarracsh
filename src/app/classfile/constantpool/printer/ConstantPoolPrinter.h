#ifndef TARRACSH_CONSTANT_POOL_PRINTER_H
#define TARRACSH_CONSTANT_POOL_PRINTER_H
#include <unordered_map>
#include "domain/classfile/ClassFileParser.h"


namespace kapa::tarracsh::app::classfile::constantpool::printer {

using kapa::tarracsh::domain::classfile::ClassFileParser;

class ConstantPoolPrinter {
public:
    explicit ConstantPoolPrinter(ClassFileParser &classFileParser);
    virtual ~ConstantPoolPrinter() = default;

    virtual void print();

    static void init();
    typedef std::unordered_map<std::string, std::string> OutputSubTemplates;


protected:
    domain::classfile::ClassFileParser &_classFileParser;
    const domain::classfile::constantpool::ConstantPool &_constantPool;
    std::string _currentLine;
    static std::mutex _cpoolStdoutMutex;

    static std::vector<std::vector<std::string>> _poolTagToString;
    static std::vector<std::string> _refKindToString;
    static int _cpoolStringIndex;

    std::vector<std::string> _outputLines{};


    static std::string tagToString(ConstantPoolTag tag);
    static std::string refKindToString(MethodHandleSubtypes tag);
    virtual void printHeader(const domain::classfile::constantpool::ConstPoolBase &entry, int index);
    virtual void printUtf8Info(const domain::classfile::constantpool::Utf8Info &entry, int index);
    virtual void printStringInfo(const domain::classfile::constantpool::StringInfo &entry, int index);
    virtual void printLongInfo(const domain::classfile::constantpool::LongInfo &entry, int index);
    virtual void printDoubleInfo(const domain::classfile::constantpool::DoubleInfo &entry, int index);
    virtual void printIntegerInfo(const domain::classfile::constantpool::IntegerInfo &entry, int index);
    virtual void printFloatInfo(const domain::classfile::constantpool::FloatInfo &entry, int index);
    virtual void printClassInfo(const domain::classfile::constantpool::ClassInfo &entry, int index);
    virtual void printMethodrefInfo(const domain::classfile::constantpool::MethodrefInfo &entry, int index);
    virtual void printMethodHandleInfo(const domain::classfile::constantpool::MethodHandleInfo &entry, int index);
    virtual void printMethodTypeInfo(const domain::classfile::constantpool::MethodTypeInfo &entry, int index);
    virtual void printFieldrefInfo(const domain::classfile::constantpool::FieldrefInfo &entry, int index);
    virtual void printModuleInfo(const domain::classfile::constantpool::ModuleInfo &entry, int index);
    virtual void printRefExtraInfo(const domain::classfile::constantpool::MemberInfo &entry);
    virtual void printInterfaceMethodrefInfo(const domain::classfile::constantpool::InterfaceMethodrefInfo &entry, int index);
    virtual void printInvokeDynamicInfo(const domain::classfile::constantpool::InvokeDynamicInfo &entry, int index);
    virtual void printNameAndTypeInfo(const domain::classfile::constantpool::NameAndTypeInfo &entry, int index);
    virtual void printEntry(const domain::classfile::constantpool::ConstantPoolRecord &entry, int index);
    virtual void printTitle();

    static void initStringMaps();


};


}
#endif //TARRACSH_CONSTANT_POOL_PRINTER_H
