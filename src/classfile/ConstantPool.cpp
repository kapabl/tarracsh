


#include <string>
#include <vector>

#include "ConstantPool.h"

#include <cassert>
#include <iostream>
#include <ostream>


using namespace org::kapa::tarracsh;
using namespace std;

std::vector<std::string> ConstantPool::_poolTagToString;
std::vector<std::string> ConstantPool::_refKindToString;

void ConstantPool::relocate() {
    const auto baseAddress = reinterpret_cast<u1 *>(_buffer.data());
    for (auto &pConstantPoolRecord : _constantPoolIndex) {
        const auto offset = reinterpret_cast<std::intptr_t>(pConstantPoolRecord);
        pConstantPoolRecord = reinterpret_cast<ConstantPoolRecord *>(baseAddress + offset);
    }
}


void ConstantPool::print() const {
    for (u2 index = 1u; index < _count; index++) {
        const auto &entry = getEntry(index);
        printEntry(entry, index);
    }
}

void ConstantPool::init() {
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

std::string ConstantPool::tagToString(ConstantPoolTag tag) {
    if (tag < ConstantPool::_poolTagToString.size()) return ConstantPool::_poolTagToString[tag];
    return std::format("Invalid pool tag:{}", static_cast<unsigned char>(tag));
}

std::string ConstantPool::refKindToString(MethodHandleSubtypes tag) {
    if (tag < ConstantPool::_refKindToString.size()) return ConstantPool::_refKindToString[tag];
    return std::format("Invalid MethodHandleSubtypes tag:{}", static_cast<unsigned char>(tag));
}

ConstantPoolRecord & ConstantPool::getEntry(const u2 index) const {
    assert(index - 1u < _constantPoolIndex.size());
    auto &result = *_constantPoolIndex[index - 1];
    return result;
}


inline void ConstantPool::printEntry(const ConstPoolBase& entry, int index) const {
    cout << endl << std::flush;
    cout << format("{: <6} {: <22}\t", index, ConstantPool::tagToString(entry.tag));
}

inline void ConstantPool::printEntry(const Utf8Info& entry, int index) const {
    printEntry(static_cast<ConstPoolBase>(entry), index);
    wcout << entry.getValue(true);
}

inline void ConstantPool::printEntry(const StringInfo& entry, int index) const {
    printEntry(static_cast<ConstPoolBase>(entry), index);
    const auto& stringValue = getString(entry.stringIndex, true);
    wcout << format(L"idx:{} {}", entry.stringIndex, stringValue);
}

inline void ConstantPool::printEntry(const LongInfo& entry, int index) const {
    printEntry(static_cast<ConstPoolBase>(entry), index);
    wcout << entry.getLongLong();
}

inline void ConstantPool::printEntry(const DoubleInfo& entry, int index) const {
    printEntry(static_cast<ConstPoolBase>(entry), index);
    wcout << entry.getDouble();
}

inline void ConstantPool::printEntry(const IntegerInfo& entry, int index) const {
    printEntry(static_cast<ConstPoolBase>(entry), index);
    wcout << entry.value;
}
inline void ConstantPool::printEntry(const FloatInfo& entry, int index) const {
    printEntry(static_cast<ConstPoolBase>(entry), index);
    wcout << entry.getFloat();
}

inline void ConstantPool::printEntry(const ClassInfo& entry, int index) const {
    printEntry(static_cast<ConstPoolBase>(entry), index);
    wcout << format(L"idx:{} {}", entry.nameIndex, getClassname(entry.nameIndex));
}

inline void ConstantPool::printEntry(const MethodrefInfo& entry, int index) const {
    printEntry(static_cast<ConstPoolBase>(entry), index);
    printRefExtraInfo(entry);
}


inline void ConstantPool::printEntry(const MethodHandleInfo& entry, int index) const {
    printEntry(static_cast<ConstPoolBase>(entry), index);
    cout << format("ref kind:{}, ref idx:{}",
        ConstantPool::refKindToString(entry.referenceKind),
        entry.referenceIndex
    );
}

inline void ConstantPool::printEntry(const MethodTypeInfo& entry, int index) const {
    printEntry(static_cast<ConstPoolBase>(entry), index);
    wcout << getEntry(entry.descriptorIndex).utf8Info.getValue();
}

inline void ConstantPool::printEntry(const FieldrefInfo& entry, int index) const {
    printEntry(static_cast<ConstPoolBase>(entry), index);
    printRefExtraInfo(entry);
}

inline void ConstantPool::printEntry(const ModuleInfo& entry, int index) const {
    printEntry(static_cast<ConstPoolBase>(entry), index);
    wcout << getEntry(entry.nameIndex).utf8Info.getValue();
}

void ConstantPool::printRefExtraInfo(const MemberInfo& entry) const {
    const auto& nameAndTypeInfo = getEntry(entry.nameAndTypeIndex).nameAndTypeInfo;
    wcout << format(L"idx:{}/{} {}: {}:{}",
        entry.classIndex,
        entry.nameAndTypeIndex,
        getClassInfoName(entry.classIndex),
        getString(nameAndTypeInfo.nameIndex),
        getString(nameAndTypeInfo.descriptorIndex)
    );
}

inline void ConstantPool::printEntry(const InterfaceMethodrefInfo& entry, int index) const {
    printEntry(static_cast<ConstPoolBase>(entry), index);
    printRefExtraInfo(entry);
}

inline void ConstantPool::printEntry(const InvokeDynamicInfo& entry, int index) const {
    printEntry(static_cast<ConstPoolBase>(entry), index);

    const auto& nameAndType = getEntry(entry.nameAndTypeIndex).nameAndTypeInfo;

    wcout << format(L"Bootstrap MT idx:{},N&T:{} {}{}",
        entry.bootstrapMethodAttrIndex,
        entry.nameAndTypeIndex,
        getString(nameAndType.nameIndex),
        getString(nameAndType.descriptorIndex)
    );

}

inline void ConstantPool::printEntry(const NameAndTypeInfo& entry, int index) const {
    printEntry(static_cast<ConstPoolBase>(entry), index);
    wcout << format(L"idx:{}/{} {}:{}",
        entry.nameIndex,
        entry.descriptorIndex,
        getString(entry.nameIndex),
        getString(entry.descriptorIndex)
    );
}


inline void ConstantPool::printEntry(const ConstantPoolRecord& entry, int index) const {
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
