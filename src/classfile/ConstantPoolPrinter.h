#ifndef TARRACSH_CONSTANT_POOL_PRINTER_H
#define TARRACSH_CONSTANT_POOL_PRINTER_H


#include <cstring>

#include "ClassFileStructures.h"
#include "ConstantPool.h"

namespace org::kapa::tarracsh {

class ConstantPoolPrinter {
public:
    ConstantPoolPrinter( const ConstantPool& constantPool);
    ~ConstantPoolPrinter() = default;

    void print() const;
    static void init();


private:

    const ConstantPool& _constantPool;

    static std::vector<std::string> _poolTagToString;
    static std::vector<std::string> _refKindToString;

    static std::string tagToString(ConstantPoolTag tag);
    static std::string refKindToString(MethodHandleSubtypes tag);
    void printEntry(const ConstPoolBase &entry, int index) const;
    void printEntry(const Utf8Info &entry, int index) const;
    void printEntry(const StringInfo &entry, int index) const;
    void printEntry(const LongInfo &entry, int index) const;
    void printEntry(const DoubleInfo &entry, int index) const;
    void printEntry(const IntegerInfo &entry, int index) const;
    void printEntry(const FloatInfo &entry, int index) const;
    void printEntry(const ClassInfo &entry, int index) const;
    void printEntry(const MethodrefInfo &entry, int index) const;
    void printEntry(const MethodHandleInfo &entry, int index) const;
    void printEntry(const MethodTypeInfo &entry, int index) const;
    void printEntry(const FieldrefInfo &entry, int index) const;
    void printEntry(const ModuleInfo &entry, int index) const;
    void printRefExtraInfo(const MemberInfo &entry) const;
    void printEntry(const InterfaceMethodrefInfo &entry, int index) const;
    void printEntry(const InvokeDynamicInfo &entry, int index) const;
    void printEntry(const NameAndTypeInfo &entry, int index) const;
    void printEntry(const ConstantPoolRecord &entry, int index) const;

};


}
#endif //TARRACSH_CONSTANT_POOL_PRINTER_H
