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
    ConstantPoolPrinter( const ClassFileAnalyzer& classFileAnalyzer);
    ~ConstantPoolPrinter() = default;

    void print() const;
    static void readSubTemplates();
    static void init();


    typedef std::unordered_map< std::string, std::string> OutputSubTemplates;


private:

    const ClassFileAnalyzer& _classFileAnalyzer;
    const ConstantPool& _constantPool;

    static std::vector<std::string> _poolTagToString;
    static std::vector<std::string> _refKindToString;


    // static YAML::Node  _outputTemplates;
    static std::unordered_map<std::string,std::string>  _templateFragments;
    
    // static std::unordered_map<std::string, OutputSubTemplates > _outputTemplates;

    static std::string tagToString(ConstantPoolTag tag);
    static std::string refKindToString(MethodHandleSubtypes tag);
    static void printHeader(const ConstPoolBase &entry, int index);
    static void printUtf8Info(const Utf8Info &entry, int index);
    void printStringInfo(const StringInfo &entry, int index) const;
    static void printLongInfo(const LongInfo &entry, int index);
    static void printDoubleInfo(const DoubleInfo &entry, int index);
    static void printIntegerInfo(const IntegerInfo &entry, int index);
    static void printFloatInfo(const FloatInfo &entry, int index);
    void printClassInfo(const ClassInfo &entry, int index) const;
    void printMethodrefInfo(const MethodrefInfo &entry, int index) const;
    static void printMethodHandleInfo(const MethodHandleInfo &entry, int index);
    void printMethodTypeInfo(const MethodTypeInfo &entry, int index) const;
    void printEntry(const FieldrefInfo &entry, int index) const;
    void printModuleInfo(const ModuleInfo &entry, int index) const;
    void printRefExtraInfo(const MemberInfo &entry) const;
    void printInterfaceMethodrefInfo(const InterfaceMethodrefInfo &entry, int index) const;
    void printInvokeDynamicInfo(const InvokeDynamicInfo &entry, int index) const;
    void printNameAndTypeInfo(const NameAndTypeInfo &entry, int index) const;
    void printEntry(const ConstantPoolRecord &entry, int index) const;


};


}
#endif //TARRACSH_CONSTANT_POOL_PRINTER_H
