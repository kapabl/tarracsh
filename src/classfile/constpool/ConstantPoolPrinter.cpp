#include <inja/inja.hpp>

#include <string>
#include <vector>

#include "ConstantPoolPrinter.h"

#include <iostream>
#include <ostream>
#include <regex>
#include <yaml-cpp/yaml.h>
#include "../utils/StringUtils.h"


using namespace org::kapa::tarracsh;
using namespace std;
using namespace inja;


vector<string> ConstantPoolPrinter::_poolTagToString;
vector<string> ConstantPoolPrinter::_refKindToString;
unordered_map<string, string> ConstantPoolPrinter::_templateFragments;
string ConstantPoolPrinter::_templateType;



ConstantPoolPrinter::ConstantPoolPrinter(const ClassFileAnalyzer &classFileAnalyzer)
    : _classFileAnalyzer(classFileAnalyzer), _constantPool(classFileAnalyzer.getConstantPool()) {
}


void ConstantPoolPrinter::printTitle() {
    json data;
    data["title"] = _classFileAnalyzer.getMainClassname();
    _currentLine += render(_templateFragments["header"], data);
}

void ConstantPoolPrinter::printToConsole() {
    printTitle();
    cout << _currentLine << endl;
    for (u2 index = 1u; index < _constantPool.getPoolSize(); index++) {
        const auto &entry = _constantPool[index];
        _currentLine.clear();
        printEntry(entry, index);
        cout << _currentLine << endl;
    }
}

filesystem::path ConstantPoolPrinter::getClassHtmlIndexFilename() const {
    const auto dir = filesystem::path(TarracshApp::getOptions().outputDir) /
        "nav" /
        getNavClassRelDir();

    auto result = dir / "index.html";
    return result;
}

json::string_t::const_pointer ConstantPoolPrinter::getImplLinks() {
    return "TODO impls. links";
}

std::vector<std::string> ConstantPoolPrinter::renderHtmlClassIndex() {
    vector<string> result;
    json data;
    data["classname"] = _classFileAnalyzer.getMainClassname();
    data["implementations"] = getImplLinks();
    result.emplace_back( render(_templateFragments["index-html"], data));
    return result;
}

void ConstantPoolPrinter::mainClassToHtmlIndex() {
    const auto filename = getClassHtmlIndexFilename();
    const auto htmlLines = renderHtmlClassIndex();
    fsUtils::writeLines(filename.string(), htmlLines);
}

string ConstantPoolPrinter::getNavClassRelDir() const {
    auto result = _classFileAnalyzer.getMainClassname();
    return result;
}

string ConstantPoolPrinter::getNavImplRelDir() const {
    auto result = filesystem::path(_classFileAnalyzer.getContainingFile()).filename().string();
    return result;
}

filesystem::path ConstantPoolPrinter::getHtmlCPoolFilename() const {
    const auto dir = filesystem::path(TarracshApp::getOptions().outputDir) /
                     "nav" /
                     getNavClassRelDir() /
                     getNavImplRelDir();
    fsUtils::ensureDir(dir);

    auto result = dir / "cpool.html";
    return result;
}

vector<string> ConstantPoolPrinter::renderCPoolHtml(const vector<string> &lines) const {
    vector<string> result;
    json data;
    data["classname"] = _classFileAnalyzer.getMainClassname();
    data["cpoolEntries"] = stringUtils::join(lines, string("<br>"));
    result.emplace_back(render(_templateFragments["cpool-html"], data));
    return result;
}

void ConstantPoolPrinter::linesToHtmlFile(const vector<string> &lines) {
    const auto filename = getHtmlCPoolFilename();
    const auto htmlLines = renderCPoolHtml(lines);
    fsUtils::writeLines(filename.string(), htmlLines);
}

void ConstantPoolPrinter::printToHtmlNav() {
    printTitle();
    vector<string> lines;
    lines.emplace_back(_currentLine);

    for (u2 index = 1u; index < _constantPool.getPoolSize(); index++) {
        const auto &entry = _constantPool[index];
        _currentLine.clear();
        printEntry(entry, index);
        lines.emplace_back(_currentLine);
    }
    linesToHtmlFile(lines);
    mainClassToHtmlIndex();

}

void ConstantPoolPrinter::readSubTemplates() {
    auto subTemplates = YAML::LoadFile("sub-templates.yaml");
    const auto &outputTemplates = subTemplates["templates"][_templateType];

    for (auto it = outputTemplates.begin(); it != outputTemplates.end(); ++it) {
        auto templateFragment = it->second.as<string>();
        templateFragment = regex_replace(templateFragment, regex("\\t"), "\t");
        _templateFragments[it->first.as<string>()] = templateFragment;
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
    json data;
    data["index"] = index;
    data["name"] = tagToString(entry.tag);
    _currentLine += render(_templateFragments["entry"], data);
}

void ConstantPoolPrinter::printUtf8Info(const Utf8Info &entry, int index) {
    printHeader(static_cast<ConstPoolBase>(entry), index);
    json data;
    data["value"] = entry.getAsUtf8(true);
    _currentLine += render(_templateFragments["utf8-info"], data);
}

inline void ConstantPoolPrinter::printStringInfo(const StringInfo &entry, int index) {
    printHeader(static_cast<ConstPoolBase>(entry), index);

    json data;
    data["index"] = entry.stringIndex;
    data["value"] = _constantPool.getString(entry.stringIndex, true);
    _currentLine += render(_templateFragments["string-info"], data);
}

inline void ConstantPoolPrinter::printLongInfo(const LongInfo &entry, int index) {
    printHeader(static_cast<ConstPoolBase>(entry), index);
    json data;
    data["value"] = entry.getLongLong();
    _currentLine += render(_templateFragments["value"], data);
}

inline void ConstantPoolPrinter::printDoubleInfo(const DoubleInfo &entry, int index) {
    printHeader(static_cast<ConstPoolBase>(entry), index);
    json data;
    data["value"] = entry.getDouble();
    _currentLine += render(_templateFragments["value"], data);
}

inline void ConstantPoolPrinter::printIntegerInfo(const IntegerInfo &entry, int index) {
    printHeader(static_cast<ConstPoolBase>(entry), index);
    json data;
    data["value"] = entry.value;
    _currentLine += render(_templateFragments["value"], data);
}

inline void ConstantPoolPrinter::printFloatInfo(const FloatInfo &entry, int index) {
    printHeader(static_cast<ConstPoolBase>(entry), index);
    _outputLines.push_back(to_string(entry.getFloat()));

    json data;
    data["value"] = entry.getFloat();
    _currentLine += render(_templateFragments["value"], data);
}

inline void ConstantPoolPrinter::printClassInfo(const ClassInfo &entry, int index) {
    printHeader(static_cast<ConstPoolBase>(entry), index);

    json data;
    data["index"] = entry.nameIndex;
    data["value"] = _constantPool.getClassname(entry.nameIndex);
    _currentLine += render(_templateFragments["class-info"], data);
}

inline void ConstantPoolPrinter::printMethodrefInfo(const MethodrefInfo &entry, int index) {
    printHeader(static_cast<ConstPoolBase>(entry), index);
    printRefExtraInfo(entry);
}


inline void ConstantPoolPrinter::printMethodHandleInfo(const MethodHandleInfo &entry, int index) {
    printHeader(static_cast<ConstPoolBase>(entry), index);

    json data;
    data["index"] = entry.referenceIndex;
    data["kind"] = refKindToString(entry.referenceKind);
    _currentLine += render(_templateFragments["method-handle-info"], data);
}

inline void ConstantPoolPrinter::printMethodTypeInfo(const MethodTypeInfo &entry, int index) {
    printHeader(static_cast<ConstPoolBase>(entry), index);
    json data;
    data["value"] = _constantPool[entry.descriptorIndex].utf8Info.getAsUtf8();
    _currentLine += render(_templateFragments["value"], data);
}

inline void ConstantPoolPrinter::printFieldrefInfo(const FieldrefInfo &entry, int index) {
    printHeader(static_cast<ConstPoolBase>(entry), index);
    printRefExtraInfo(entry);
}

inline void ConstantPoolPrinter::printModuleInfo(const ModuleInfo &entry, int index) {
    printHeader(static_cast<ConstPoolBase>(entry), index);
    json data;
    data["value"] = _constantPool[entry.nameIndex].utf8Info.getAsUtf8();
    _currentLine += render(_templateFragments["value"], data);
}

void ConstantPoolPrinter::printRefExtraInfo(const MemberInfo &entry) {
    const auto &nameAndTypeInfo = _constantPool[entry.nameAndTypeIndex].nameAndTypeInfo;

    json data;
    data["classIndex"] = entry.classIndex;
    data["nameAndTypeIndex"] = entry.nameAndTypeIndex;

    data["classname"] = _constantPool.getClassInfoName(entry.classIndex);

    data["typeName"] = _constantPool.getString(nameAndTypeInfo.nameIndex);
    data["descriptor"] = _constantPool.getString(nameAndTypeInfo.descriptorIndex);
    _currentLine += render(_templateFragments["ref-extra-info"], data);
}

inline void ConstantPoolPrinter::printInterfaceMethodrefInfo(const InterfaceMethodrefInfo &entry, int index) {
    printHeader(static_cast<ConstPoolBase>(entry), index);
    printRefExtraInfo(entry);
}

inline void ConstantPoolPrinter::printInvokeDynamicInfo(const InvokeDynamicInfo &entry, int index) {
    printHeader(static_cast<ConstPoolBase>(entry), index);

    const auto &nameAndTypeInfo = _constantPool[entry.nameAndTypeIndex].nameAndTypeInfo;
    json data;
    data["index"] = entry.bootstrapMethodAttrIndex;
    data["nameAndTypeIndex"] = entry.nameAndTypeIndex;
    data["typeName"] = _constantPool.getString(nameAndTypeInfo.nameIndex);
    data["descriptor"] = _constantPool.getString(nameAndTypeInfo.descriptorIndex);
    _currentLine += render(_templateFragments["bootstrap"], data);

}

inline void ConstantPoolPrinter::printNameAndTypeInfo(const NameAndTypeInfo &entry, int index) {
    printHeader(static_cast<ConstPoolBase>(entry), index);

    json data;
    data["index"] = entry.nameIndex;
    data["descriptorIndex"] = entry.descriptorIndex;
    data["typeName"] = _constantPool.getString(entry.nameIndex);
    data["descriptor"] = _constantPool.getString(entry.descriptorIndex);
    _currentLine += render(_templateFragments["name-type-info"], data);

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
