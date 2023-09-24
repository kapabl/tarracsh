#include <string>
#include <vector>
#include <iostream>
#include <ostream>
#include "ConstantPoolPrinter.h"

#include <mutex>

#include "app/Context.h"

// #include "app/App.h"


using namespace kapa::tarracsh::app::classfile::constantpool::printer;
using namespace kapa::tarracsh::domain::classfile::constantpool;
using namespace std;


vector<vector<string>> ConstantPoolPrinter::_poolTagToString;
std::unordered_set<ConstantPoolTag> ConstantPoolPrinter::_filter;
std::unordered_map<std::string, ConstantPoolTag> ConstantPoolPrinter::_stringToPoolTag;
vector<string> ConstantPoolPrinter::_refKindToString;
int ConstantPoolPrinter::_cpoolStringIndex = 1;
mutex ConstantPoolPrinter::_cpoolStdoutMutex;


ConstantPoolPrinter::ConstantPoolPrinter(ClassFileParser &classFileParser)
        : _classFileParser(classFileParser),
          _constantPool(classFileParser.getConstantPool()) {
}


void ConstantPoolPrinter::printTitle() {
    _currentLine += fmt::format("Constant Pool for: {}", _classFileParser.getMainClassname());
}

void ConstantPoolPrinter::print() {
    {
        unique_lock lock(_cpoolStdoutMutex);
        try {
            printTitle();
            cout << _currentLine << endl;
            for (u2 index = 1u; index < _constantPool.getPoolSize(); index = _constantPool.getNextIndex(index)) {

                const auto &entry = _constantPool.getEntry(index);
                if (!applyFilter(entry.base.tag)) continue;

                _currentLine.clear();
                printHeader(entry.base, index);
                printEntry(entry, index);
                cout << _currentLine << endl;
            }
        }
        catch (...) {
            cout << "Exception printing constant pool" << endl;
        }
    }
}


string ConstantPoolPrinter::tagToString(ConstantPoolTag tag) {
    if (tag < _poolTagToString.size()) {
        return _poolTagToString[tag][_cpoolStringIndex];
    }
    return fmt::format("Invalid pool tag:{}", static_cast<unsigned char>(tag));
}

string ConstantPoolPrinter::refKindToString(MethodHandleSubtypes tag) {
    if (tag < _refKindToString.size()) return _refKindToString[tag];
    return fmt::format("Invalid MethodHandleSubtypes tag:{}", static_cast<unsigned char>(tag));
}

void ConstantPoolPrinter::printHeader(const ConstPoolBase &entry, int index) {
    _currentLine += std::to_string(index) + "\t" + tagToString(entry.tag) + "\t";
}

void ConstantPoolPrinter::printUtf8Info(const Utf8Info &entry, int index) {

    _currentLine += entry.getAsUtf8(true);
}

inline void ConstantPoolPrinter::printStringInfo(const StringInfo &entry, int index) {

    _currentLine += fmt::format("{} {}",
                                entry.stringIndex,
                                _constantPool.getString(entry.stringIndex, true));
}

inline void ConstantPoolPrinter::printLongInfo(const LongInfo &entry, int index) {
    _currentLine += std::to_string(entry.getLongLong());
}

inline void ConstantPoolPrinter::printDoubleInfo(const DoubleInfo &entry, int index) {
    _currentLine += std::to_string(entry.getDouble());
}

inline void ConstantPoolPrinter::printIntegerInfo(const IntegerInfo &entry, int index) {
    _currentLine += std::to_string(entry.value);
}

inline void ConstantPoolPrinter::printFloatInfo(const FloatInfo &entry, int index) {
    _currentLine += std::to_string(entry.getFloat());
}

inline void ConstantPoolPrinter::printClassInfo(const ClassInfo &entry, int index) {
    _currentLine += fmt::format("idx:{} {}", entry.nameIndex, _constantPool.getClassname(entry.nameIndex));
}

inline void ConstantPoolPrinter::printMethodrefInfo(const MethodrefInfo &entry, int index) {
    printRefExtraInfo(entry);
}


inline void ConstantPoolPrinter::printMethodHandleInfo(const MethodHandleInfo &entry, int index) {
    _currentLine += fmt::format("ref-kind:{}, idx:{}",
                                refKindToString(entry.referenceKind),
                                entry.referenceIndex);
}

inline void ConstantPoolPrinter::printMethodTypeInfo(const MethodTypeInfo &entry, int index) {
    _currentLine += _constantPool.getEntry(entry.descriptorIndex).utf8Info.getAsUtf8();
}

inline void ConstantPoolPrinter::printFieldrefInfo(const FieldrefInfo &entry, int index) {
    printRefExtraInfo(entry);
}

inline void ConstantPoolPrinter::printModuleInfo(const ModuleInfo &entry, int index) {
    _currentLine += _constantPool.getEntry(entry.nameIndex).utf8Info.getAsUtf8();
}

void ConstantPoolPrinter::printRefExtraInfo(const MemberInfo &entry) {
    const auto &nameAndTypeInfo = _constantPool.getEntry(entry.nameAndTypeIndex).nameAndTypeInfo;

    _currentLine += fmt::format("{}:{} {}: {}:{}", entry.classIndex, entry.nameAndTypeIndex,
                                _constantPool.getClassInfoName(entry.classIndex),
                                _constantPool.getString(nameAndTypeInfo.nameIndex),
                                _constantPool.getString(nameAndTypeInfo.descriptorIndex));
}

inline void ConstantPoolPrinter::printInterfaceMethodrefInfo(const InterfaceMethodrefInfo &entry, int index) {
    printRefExtraInfo(entry);
}

inline void ConstantPoolPrinter::printInvokeDynamicInfo(const InvokeDynamicInfo &entry, int index) {
    const auto &nameAndTypeInfo = _constantPool.getEntry(entry.nameAndTypeIndex).nameAndTypeInfo;

    _currentLine += fmt::format("Bootstrap MT idx:{},N&T:{} {}:{}",
                                entry.bootstrapMethodAttrIndex,
                                entry.nameAndTypeIndex,
                                _constantPool.getString(nameAndTypeInfo.nameIndex),
                                _constantPool.getString(nameAndTypeInfo.descriptorIndex));

}

void ConstantPoolPrinter::printNameAndTypeInfo(const NameAndTypeInfo &entry, int index) {
    _currentLine += fmt::format("{}:{} {}:{}", entry.nameIndex, entry.descriptorIndex,
                                _constantPool.getString(entry.nameIndex),
                                _constantPool.getString(entry.descriptorIndex));
}


void ConstantPoolPrinter::printEntry(const ConstantPoolRecord &entry, int index) {
    switch (entry.base.tag) {
        case JVM_CONSTANT_Utf8:
            printUtf8Info(entry.utf8Info, index);
            break;

        case JVM_CONSTANT_Float:
            printFloatInfo(entry.floatInfo, index);
            break;

        case JVM_CONSTANT_Integer:
            printIntegerInfo(entry.integerInfo, index);
            break;

        case JVM_CONSTANT_Long:
            printLongInfo(entry.longInfo, index);
            break;

        case JVM_CONSTANT_Double:
            printDoubleInfo(entry.doubleInfo, index);
            break;

        case JVM_CONSTANT_Class:
            printClassInfo(entry.classInfo, index);
            break;

        case JVM_CONSTANT_String:
            printStringInfo(entry.stringInfo, index);
            break;

        case JVM_CONSTANT_NameAndType:
            printNameAndTypeInfo(entry.nameAndTypeInfo, index);
            break;

        case JVM_CONSTANT_Methodref:
            printMethodrefInfo(entry.methodrefInfo, index);
            break;

        case JVM_CONSTANT_MethodHandle:
            printMethodHandleInfo(entry.methodHandleInfo, index);
            break;

        case JVM_CONSTANT_InvokeDynamic:
            printInvokeDynamicInfo(entry.invokeDynamicInfo, index);
            break;

        case JVM_CONSTANT_MethodType:
            printMethodTypeInfo(entry.methodTypeInfo, index);
            break;

        case JVM_CONSTANT_Fieldref:
            printFieldrefInfo(entry.fieldrefInfo, index);
            break;

        case JVM_CONSTANT_InterfaceMethodref:
            printInterfaceMethodrefInfo(entry.interfaceMethodrefInfo, index);
            break;

        case JVM_CONSTANT_Module:
            printModuleInfo(entry.moduleInfo, index);
            break;

        case JVM_CONSTANT_Package:
        case JVM_CONSTANT_Dynamic:
            printHeader(entry.base, index);
            break;

            // case JVM_CONSTANT_ExternalMax:
        case JVM_CONSTANT_Unicode:
            _currentLine = "Unused Cool Entry type found: JVM_CONSTANT_Unicode";
            break;

        case JVM_CONSTANT_Empty:
            _currentLine = "empty entry";
            break;
    }
}

void ConstantPoolPrinter::initStringMaps(Context &context) {
    _poolTagToString.resize(JVM_CONSTANT_ExternalMax + 1);

    _poolTagToString[JVM_CONSTANT_Empty] = {"empty", "Empty"};
    _poolTagToString[JVM_CONSTANT_Utf8] = {"utf8", "UTF8 String"};
    _poolTagToString[JVM_CONSTANT_Unicode] = {"unicode", "Unicode"};
    _poolTagToString[JVM_CONSTANT_Integer] = {"int", "Int"};
    _poolTagToString[JVM_CONSTANT_Float] = {"float", "Float"};
    _poolTagToString[JVM_CONSTANT_Long] = {"long", "Long"};
    _poolTagToString[JVM_CONSTANT_Double] = {"double", "Double"};
    _poolTagToString[JVM_CONSTANT_Class] = {"class", "Class Ref"};
    _poolTagToString[JVM_CONSTANT_String] = {"string", "String"};
    _poolTagToString[JVM_CONSTANT_Fieldref] = {"fieldref", "Field Ref"};
    _poolTagToString[JVM_CONSTANT_Methodref] = {"methodref", "Method Ref"};
    _poolTagToString[JVM_CONSTANT_InterfaceMethodref] = {"iface-methodref", "IFACE Method Ref"};
    _poolTagToString[JVM_CONSTANT_NameAndType] = {"name-type", "Name & Type"};
    _poolTagToString[JVM_CONSTANT_MethodHandle] = {"member-handle", "Method Handle"};
    _poolTagToString[JVM_CONSTANT_MethodType] = {"member-type", "Method Type"};
    _poolTagToString[JVM_CONSTANT_Dynamic] = {"dynamic", "Dynamic"};
    _poolTagToString[JVM_CONSTANT_InvokeDynamic] = {"invk-dynamic", "Invoke Dynamic"};
    _poolTagToString[JVM_CONSTANT_Module] = {"module", "Module"};
    _poolTagToString[JVM_CONSTANT_Package] = {"package", "Package"};

    auto index = -1;
    for( auto &poolTagStrings: _poolTagToString) {
        ++index;
        if (poolTagStrings.empty()) continue;
        _stringToPoolTag[ poolTagStrings[0] ] = static_cast<ConstantPoolTag>(index);
    }

    auto &cpoolFilter = context.getOptions().parse.cpoolFilter;
    std::unordered_set filterSet(cpoolFilter.begin(), cpoolFilter.end() );
    for( auto&[tagString, tag]: _stringToPoolTag) {
        if ( filterSet.contains(tagString) ) {
            _filter.insert(static_cast<ConstantPoolTag>(tag));
        }
    }

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

void ConstantPoolPrinter::init(Context &context) {
    initStringMaps(context);
    initFilter();

    //TODO fix this dependency from the app layer
    _cpoolStringIndex = context.getOptions().parse.descriptiveCPoolEntries ? 1 : 0;
}

auto ConstantPoolPrinter::applyFilter(ConstantPoolTag tag) const -> bool {
    const auto result = _filter.empty() || _filter.contains(tag);
    return result;
}

void ConstantPoolPrinter::initFilter() {

}
