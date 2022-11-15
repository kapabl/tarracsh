#include <string>
#include <vector>

#include "ConstantPoolPrinter.h"
#include <iostream>
#include <ostream>

#include "DescriptorParser.h"


using namespace org::kapa::tarracsh;
using namespace std;


std::vector<std::string> ConstantPoolPrinter::_poolTagToString;
std::vector<std::string> ConstantPoolPrinter::_refKindToString;


ConstantPoolPrinter::ConstantPoolPrinter(const ConstantPool &constantPool)
    : _constantPool(constantPool) {
}


void ConstantPoolPrinter::print() const {
    for (u2 index = 1u; index < _constantPool.getPoolSize(); index++) {
        const auto &entry = _constantPool[index];
        printEntry(entry, index);
        cout << endl;
    }
}

void ConstantPoolPrinter::init() {
    _poolTagToString.resize(JVM_CONSTANT_ExternalMax + 1);

    _poolTagToString[JVM_CONSTANT_Empty] = "empty";
    _poolTagToString[JVM_CONSTANT_Utf8] = "utf8";
    _poolTagToString[JVM_CONSTANT_Unicode] = "unicode";
    _poolTagToString[JVM_CONSTANT_Integer] = "int";
    _poolTagToString[JVM_CONSTANT_Float] = "float";
    _poolTagToString[JVM_CONSTANT_Long] = "long";
    _poolTagToString[JVM_CONSTANT_Double] = "double";
    _poolTagToString[JVM_CONSTANT_Class] = "class";
    _poolTagToString[JVM_CONSTANT_String] = "string";
    _poolTagToString[JVM_CONSTANT_Fieldref] = "fieldref";
    _poolTagToString[JVM_CONSTANT_Methodref] = "methodref";
    _poolTagToString[JVM_CONSTANT_InterfaceMethodref] = "interface-methodref";
    _poolTagToString[JVM_CONSTANT_NameAndType] = "name-and-type";
    _poolTagToString[JVM_CONSTANT_MethodHandle] = "method-handle";
    _poolTagToString[JVM_CONSTANT_MethodType] = "method-type";
    _poolTagToString[JVM_CONSTANT_Dynamic] = "dynamic";
    _poolTagToString[JVM_CONSTANT_InvokeDynamic] = "invoke-dynamic";
    _poolTagToString[JVM_CONSTANT_Module] = "module";
    _poolTagToString[JVM_CONSTANT_Package] = "package/external-max";

    _refKindToString.resize(JVM_REF_LIMIT);
    _refKindToString[JVM_REF_getField] = "REF_getField";
    _refKindToString[JVM_REF_getStatic] = "REF_getStatic";
    _refKindToString[JVM_REF_putField] = "REF_putField";
    _refKindToString[JVM_REF_putStatic] = "REF_putStatic";
    _refKindToString[JVM_REF_invokeVirtual] = "REF_invokeVirtual";
    _refKindToString[JVM_REF_invokeStatic] = "REF_invokeStatic";
    _refKindToString[JVM_REF_invokeSpecial] = "REF_invokeSpecial";
    _refKindToString[JVM_REF_newInvokeSpecial] = "REF_newInvokeSpecial";
    _refKindToString[JVM_REF_invokeInterface] = "REF_invokeInterface";

}

std::string ConstantPoolPrinter::tagToString(ConstantPoolTag tag) {
    if (tag < ConstantPoolPrinter::_poolTagToString.size()) return ConstantPoolPrinter::_poolTagToString[tag];
    return std::format("Invalid pool tag:{}", static_cast<unsigned char>(tag));
}

std::string ConstantPoolPrinter::refKindToString(MethodHandleSubtypes tag) {
    if (tag < ConstantPoolPrinter::_refKindToString.size()) return ConstantPoolPrinter::_refKindToString[tag];
    return std::format("Invalid MethodHandleSubtypes tag:{}", static_cast<unsigned char>(tag));
}

inline void ConstantPoolPrinter::printEntry(const ConstPoolBase &entry, int index) const {
    //cout << endl << std::flush;
    cout << format("{: <6} {: <22}\t", index, ConstantPoolPrinter::tagToString(entry.tag));
}

inline void ConstantPoolPrinter::printEntry(const Utf8Info &entry, int index) const {
    printEntry(static_cast<ConstPoolBase>(entry), index);
    wcout << entry.getValue(true);
}

inline void ConstantPoolPrinter::printEntry(const StringInfo &entry, int index) const {
    printEntry(static_cast<ConstPoolBase>(entry), index);
    const auto &stringValue = _constantPool.getString(entry.stringIndex, true);
    wcout << format(L"idx:{} {}", entry.stringIndex, stringValue);
}

inline void ConstantPoolPrinter::printEntry(const LongInfo &entry, int index) const {
    printEntry(static_cast<ConstPoolBase>(entry), index);
    wcout << entry.getLongLong();
}

inline void ConstantPoolPrinter::printEntry(const DoubleInfo &entry, int index) const {
    printEntry(static_cast<ConstPoolBase>(entry), index);
    wcout << entry.getDouble();
}

inline void ConstantPoolPrinter::printEntry(const IntegerInfo &entry, int index) const {
    printEntry(static_cast<ConstPoolBase>(entry), index);
    wcout << entry.value;
}

inline void ConstantPoolPrinter::printEntry(const FloatInfo &entry, int index) const {
    printEntry(static_cast<ConstPoolBase>(entry), index);
    wcout << entry.getFloat();
}

inline void ConstantPoolPrinter::printEntry(const ClassInfo &entry, int index) const {
    printEntry(static_cast<ConstPoolBase>(entry), index);
    wcout << format(L"idx:{} {}", entry.nameIndex, _constantPool.getClassname(entry.nameIndex));
}

inline void ConstantPoolPrinter::printEntry(const MethodrefInfo &entry, int index) const {
    printEntry(static_cast<ConstPoolBase>(entry), index);
    printRefExtraInfo(entry);
}


inline void ConstantPoolPrinter::printEntry(const MethodHandleInfo &entry, int index) const {
    printEntry(static_cast<ConstPoolBase>(entry), index);
    cout << format("ref kind:{}, ref idx:{}",
                   ConstantPoolPrinter::refKindToString(entry.referenceKind),
                   entry.referenceIndex
        );
}

inline void ConstantPoolPrinter::printEntry(const MethodTypeInfo &entry, int index) const {
    printEntry(static_cast<ConstPoolBase>(entry), index);
    wcout << _constantPool[entry.descriptorIndex].utf8Info.getValue();
}

inline void ConstantPoolPrinter::printEntry(const FieldrefInfo &entry, int index) const {
    printEntry(static_cast<ConstPoolBase>(entry), index);
    printRefExtraInfo(entry);
}

inline void ConstantPoolPrinter::printEntry(const ModuleInfo &entry, int index) const {
    printEntry(static_cast<ConstPoolBase>(entry), index);
    wcout << _constantPool[entry.nameIndex].utf8Info.getValue();
}

void ConstantPoolPrinter::printRefExtraInfo(const MemberInfo &entry) const {
    const auto &nameAndTypeInfo = _constantPool[entry.nameAndTypeIndex].nameAndTypeInfo;
    wcout << format(L"idx:{}/{} {}: {}:{}",
                    entry.classIndex,
                    entry.nameAndTypeIndex,
                    _constantPool.getClassInfoName(entry.classIndex),
                    _constantPool.getString(nameAndTypeInfo.nameIndex),
                    _constantPool.getString(nameAndTypeInfo.descriptorIndex)
        );
}

inline void ConstantPoolPrinter::printEntry(const InterfaceMethodrefInfo &entry, int index) const {
    printEntry(static_cast<ConstPoolBase>(entry), index);
    printRefExtraInfo(entry);
}

inline void ConstantPoolPrinter::printEntry(const InvokeDynamicInfo &entry, int index) const {
    printEntry(static_cast<ConstPoolBase>(entry), index);

    const auto &nameAndType = _constantPool[entry.nameAndTypeIndex].nameAndTypeInfo;

    wcout << format(L"Bootstrap MT idx:{},N&T:{} {}{}",
                    entry.bootstrapMethodAttrIndex,
                    entry.nameAndTypeIndex,
                    _constantPool.getString(nameAndType.nameIndex),
                    _constantPool.getString(nameAndType.descriptorIndex)
        );

}

inline void ConstantPoolPrinter::printEntry(const NameAndTypeInfo &entry, int index) const {
    printEntry(static_cast<ConstPoolBase>(entry), index);
    wcout << format(L"idx:{}/{} {}:{}",
                    entry.nameIndex,
                    entry.descriptorIndex,
                    _constantPool.getString(entry.nameIndex),
                    _constantPool.getString(entry.descriptorIndex)
        );
}


inline void ConstantPoolPrinter::printEntry(const ConstantPoolRecord &entry, int index) const {
    switch (entry.base.tag) {
        case JVM_CONSTANT_Utf8:
            printEntry(entry.utf8Info, index);
            break;

        case JVM_CONSTANT_Float:
            printEntry(entry.floatInfo, index);
            break;

        case JVM_CONSTANT_Integer:
            printEntry(entry.integerInfo, index);
            break;

        case JVM_CONSTANT_Long:
            printEntry(entry.longInfo, index);
            break;

        case JVM_CONSTANT_Double:
            printEntry(entry.doubleInfo, index);
            break;

        case JVM_CONSTANT_Class:
            printEntry(entry.classInfo, index);
            break;

        case JVM_CONSTANT_String:
            printEntry(entry.stringInfo, index);
            break;

        case JVM_CONSTANT_NameAndType:
            printEntry(entry.nameAndTypeInfo, index);
            break;

        case JVM_CONSTANT_Methodref:
            printEntry(entry.methodrefInfo, index);
            break;

        case JVM_CONSTANT_MethodHandle:
            printEntry(entry.methodHandleInfo, index);
            break;

        case JVM_CONSTANT_InvokeDynamic:
            printEntry(entry.invokeDynamicInfo, index);
            break;

        case JVM_CONSTANT_MethodType:
            printEntry(entry.methodTypeInfo, index);
            break;

        case JVM_CONSTANT_Fieldref:
            printEntry(entry.fieldrefInfo, index);
            break;

        case JVM_CONSTANT_InterfaceMethodref:
            printEntry(entry.interfaceMethodrefInfo, index);
            break;

        case JVM_CONSTANT_Module:
            printEntry(entry.moduleInfo, index);
            break;

        case JVM_CONSTANT_Package:
        case JVM_CONSTANT_Dynamic:
            printEntry(entry.base, index);
            break;

        // case JVM_CONSTANT_ExternalMax:
        case JVM_CONSTANT_Unicode:
            cout << "Unused Cool Entry type found: JVM_CONSTANT_Unicode";
            break;

        case JVM_CONSTANT_Empty:
            cout << "empty entry";
            break;
    }
}
