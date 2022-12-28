#include <string>
#include <vector>
#include <iostream>
#include <ostream>
#include <regex>
#include <yaml-cpp/yaml.h>
#include "../utils/StringUtils.h"
#include "ConstantPoolPrinter.h"

using namespace org::kapa::tarracsh;
using namespace std;
// using namespace inja;


vector<string> ConstantPoolPrinter::_poolTagToString;
vector<string> ConstantPoolPrinter::_refKindToString;
unordered_map<string, inja::Template> ConstantPoolPrinter::_templateFragments;

inja::Environment ConstantPoolPrinter::_environment;
string ConstantPoolPrinter::_templateType;


ConstantPoolPrinter::ConstantPoolPrinter(const ClassFileAnalyzer &classFileAnalyzer)
    : _classFileAnalyzer(classFileAnalyzer),
      _constantPool(classFileAnalyzer.getConstantPool()) {
}


void ConstantPoolPrinter::printTitle() {
    _currentLine += std::format("Constant Pool for: {}", _classFileAnalyzer.getMainClassname());
}

void ConstantPoolPrinter::print() {
    printTitle();
    cout << _currentLine << endl;
    for (u2 index = 1u; index < _constantPool.getPoolSize(); index++) {
        const auto &entry = _constantPool[index];
        _currentLine.clear();
        printHeader(entry.base, index);
        printEntry(entry, index);
        cout << _currentLine << endl;
    }
}


std::string ConstantPoolPrinter::render(const inja::Template &compiledTemplate, const inja::json &json) {
    const auto result = _environment.render(compiledTemplate, json);
    // const auto result = inja::Environment().render(compiledTemplate, json);
    return result;
}

void ConstantPoolPrinter::readSubTemplates() {
    auto subTemplates = YAML::LoadFile("sub-templates.yaml");
    const auto &outputTemplates = subTemplates["templates"][_templateType];

    for (auto it = outputTemplates.begin(); it != outputTemplates.end(); ++it) {
        auto templateFragment = it->second.as<string>();
        templateFragment = regex_replace(templateFragment, regex("\\t"), "\t");
        _templateFragments[it->first.as<string>()] = _environment.parse(templateFragment);
    }
}


string ConstantPoolPrinter::tagToString(ConstantPoolTag tag) {
    if (tag < _poolTagToString.size()) return _poolTagToString[tag];
    return format("Invalid pool tag:{}", static_cast<unsigned char>(tag));
}

string ConstantPoolPrinter::refKindToString(MethodHandleSubtypes tag) {
    if (tag < _refKindToString.size()) return _refKindToString[tag];
    return format("Invalid MethodHandleSubtypes tag:{}", static_cast<unsigned char>(tag));
}

void ConstantPoolPrinter::printHeader(const ConstPoolBase &entry, int index) {
    // std::stringstream stream;
    // stream << index << "\t" << tagToString(entry.tag) << "\t";
    // _currentLine += stream.str();
    // _currentLine += std::format("{}\t{}\t", index, tagToString(entry.tag));
    _currentLine += std::to_string(index) + "\t" + tagToString(entry.tag) + "\t";
}

void ConstantPoolPrinter::printUtf8Info(const Utf8Info &entry, int index) {

    _currentLine += entry.getAsUtf8(true);
}

inline void ConstantPoolPrinter::printStringInfo(const StringInfo &entry, int index) {

    _currentLine += std::format("{} {}",
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
    _currentLine += std::format("idx:{} {}", entry.nameIndex, _constantPool.getClassname(entry.nameIndex));
}

inline void ConstantPoolPrinter::printMethodrefInfo(const MethodrefInfo &entry, int index) {
    printRefExtraInfo(entry);
}


inline void ConstantPoolPrinter::printMethodHandleInfo(const MethodHandleInfo &entry, int index) {
    _currentLine += std::format("ref-kind:{}, idx:{}",
                                refKindToString(entry.referenceKind),
                                entry.referenceIndex);
}

inline void ConstantPoolPrinter::printMethodTypeInfo(const MethodTypeInfo &entry, int index) {
    _currentLine += _constantPool[entry.descriptorIndex].utf8Info.getAsUtf8();
}

inline void ConstantPoolPrinter::printFieldrefInfo(const FieldrefInfo &entry, int index) {
    printRefExtraInfo(entry);
}

inline void ConstantPoolPrinter::printModuleInfo(const ModuleInfo &entry, int index) {
    _currentLine += _constantPool[entry.nameIndex].utf8Info.getAsUtf8();
}

void ConstantPoolPrinter::printRefExtraInfo(const MemberInfo &entry) {
    const auto &nameAndTypeInfo = _constantPool[entry.nameAndTypeIndex].nameAndTypeInfo;

    _currentLine += std::format("{}:{} {}: {}:{}", entry.classIndex, entry.nameAndTypeIndex,
                                _constantPool.getClassInfoName(entry.classIndex),
                                _constantPool.getString(nameAndTypeInfo.nameIndex),
                                _constantPool.getString(nameAndTypeInfo.descriptorIndex));
}

inline void ConstantPoolPrinter::printInterfaceMethodrefInfo(const InterfaceMethodrefInfo &entry, int index) {
    printRefExtraInfo(entry);
}

inline void ConstantPoolPrinter::printInvokeDynamicInfo(const InvokeDynamicInfo &entry, int index) {
    const auto &nameAndTypeInfo = _constantPool[entry.nameAndTypeIndex].nameAndTypeInfo;

    _currentLine += std::format("Bootstrap MT idx:{},N&T:{} {}:{}",
                                entry.bootstrapMethodAttrIndex,
                                entry.nameAndTypeIndex,
                                _constantPool.getString(nameAndTypeInfo.nameIndex),
                                _constantPool.getString(nameAndTypeInfo.descriptorIndex));

}

inline void ConstantPoolPrinter::printNameAndTypeInfo(const NameAndTypeInfo &entry, int index) {
    _currentLine += std::format("{}:{} {}:{}", entry.nameIndex, entry.descriptorIndex,
                                _constantPool.getString(entry.nameIndex),
                                _constantPool.getString(entry.descriptorIndex));
}


inline void ConstantPoolPrinter::printEntry(const ConstantPoolRecord &entry, int index) {
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

void ConstantPoolPrinter::initStringMaps() {
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
    _poolTagToString[JVM_CONSTANT_InterfaceMethodref] = "iface-methodref";
    _poolTagToString[JVM_CONSTANT_NameAndType] = "name-type";
    _poolTagToString[JVM_CONSTANT_MethodHandle] = "method-handle";
    _poolTagToString[JVM_CONSTANT_MethodType] = "method-type";
    _poolTagToString[JVM_CONSTANT_Dynamic] = "dynamic";
    _poolTagToString[JVM_CONSTANT_InvokeDynamic] = "invk-dynamic";
    _poolTagToString[JVM_CONSTANT_Module] = "module";
    _poolTagToString[JVM_CONSTANT_Package] = "package";

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

void ConstantPoolPrinter::init(const string &templateType) {
    _templateType = templateType;
    initStringMaps();
    readSubTemplates();

}
