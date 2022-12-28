#include <string>
#include <vector>
#include "../utils/StringUtils.h"
#include "HtmlGen.h"

using namespace org::kapa::tarracsh;
using namespace nav;
using namespace std;
// using namespace inja;



HtmlGen::HtmlGen(const ClassFileAnalyzer &classFileAnalyzer)
    : ConstantPoolPrinter(classFileAnalyzer) {
}


void HtmlGen::printTitle() {
    _currentLine += std::format("Constant Pool for: {}", _classFileAnalyzer.getMainClassname());
}

filesystem::path HtmlGen::getClassHtmlIndexFilename() const {
    const auto dir = filesystem::path(TarracshApp::getOptions().outputDir) /
                     "nav" /
                     getNavClassRelDir();

    auto result = dir / "index.html";
    return result;
}

std::string HtmlGen::getImplLinks() {
    return "TODO impls. links";
}

std::vector<std::string> HtmlGen::renderHtmlClassIndex() {
    vector<string> result;
    inja::json data;
    data["classname"] = _classFileAnalyzer.getMainClassname();
    data["implementations"] = getImplLinks();
    result.emplace_back(render(_templateFragments["index-html"], data));
    return result;
}

void HtmlGen::mainClassToHtmlIndex() {
    const auto filename = getClassHtmlIndexFilename();
    const auto htmlLines = renderHtmlClassIndex();
    fsUtils::writeLines(filename.string(), htmlLines);
}

string HtmlGen::getNavClassRelDir() const {
    auto result = _classFileAnalyzer.getMainClassname();
    return result;
}

string HtmlGen::getNavImplRelDir() const {
    auto result = filesystem::path(_classFileAnalyzer.getContainingFile()).filename().string();
    return result;
}

filesystem::path HtmlGen::getHtmlCPoolFilename() const {
    const auto dir = filesystem::path(TarracshApp::getOptions().outputDir) /
                     "nav" /
                     getNavClassRelDir() /
                     getNavImplRelDir();
    fsUtils::ensureDir(dir);

    auto result = dir / "cpool.html";
    return result;
}

vector<string> HtmlGen::renderCPoolHtml(const vector<string> &lines) const {
    vector<string> result;
    inja::json data;
    data["classname"] = _classFileAnalyzer.getMainClassname();
    data["cpoolEntries"] = stringUtils::join(lines, string("<br>"));
    result.emplace_back(render(_templateFragments["cpool-html"], data));
    return result;
}

void HtmlGen::linesToHtmlFile(const vector<string> &lines) {
    const auto filename = getHtmlCPoolFilename();
    const auto htmlLines = renderCPoolHtml(lines);
    fsUtils::writeLines(filename.string(), htmlLines);
}

void HtmlGen::print() {
    vector<string> lines;

    for (u2 index = 1u; index < _constantPool.getPoolSize(); index++) {
        const auto &entry = _constantPool[index];
        _currentLine.clear();
        printHeader(entry.base, index);
        printEntry(entry, index);
        lines.emplace_back(_currentLine);
    }
    linesToHtmlFile(lines);
    mainClassToHtmlIndex();

}

void HtmlGen::printUtf8Info(const Utf8Info &entry, int index) {

    _currentLine += entry.getAsUtf8(true);
}

inline void HtmlGen::printStringInfo(const StringInfo &entry, int index) {

    _currentLine += std::format("{} {}",
                                entry.stringIndex,
                                _constantPool.getString(entry.stringIndex, true));
}

inline void HtmlGen::printLongInfo(const LongInfo &entry, int index) {
    _currentLine += std::to_string(entry.getLongLong());
}

inline void HtmlGen::printDoubleInfo(const DoubleInfo &entry, int index) {
    _currentLine += std::to_string(entry.getDouble());
}

inline void HtmlGen::printIntegerInfo(const IntegerInfo &entry, int index) {
    _currentLine += std::to_string(entry.value);
}

inline void HtmlGen::printFloatInfo(const FloatInfo &entry, int index) {
    _currentLine += std::to_string(entry.getFloat());
}

std::string HtmlGen::generateLink(const std::string& classname) {
    std::string result;
    //TODO
    return result;
}

inline void HtmlGen::printClassInfo(const ClassInfo &entry, int index) {
    const auto classname = _constantPool.getClassname(entry.nameIndex);
    _currentLine += std::format("idx:{} <a href={}>{}</a>",
        entry.nameIndex,
        generateLink( classname ), 
        classname);
}

inline void HtmlGen::printMethodrefInfo(const MethodrefInfo &entry, int index) {
    printRefExtraInfo(entry);
}


inline void HtmlGen::printMethodHandleInfo(const MethodHandleInfo &entry, int index) {
    _currentLine += std::format("ref-kind:{}, idx:{}",
                                refKindToString(entry.referenceKind),
                                entry.referenceIndex);
}

inline void HtmlGen::printMethodTypeInfo(const MethodTypeInfo &entry, int index) {
    _currentLine += _constantPool[entry.descriptorIndex].utf8Info.getAsUtf8();
}

inline void HtmlGen::printFieldrefInfo(const FieldrefInfo &entry, int index) {
    printRefExtraInfo(entry);
}

inline void HtmlGen::printModuleInfo(const ModuleInfo &entry, int index) {
    _currentLine += _constantPool[entry.nameIndex].utf8Info.getAsUtf8();
}

void HtmlGen::printRefExtraInfo(const MemberInfo &entry) {
    const auto &nameAndTypeInfo = _constantPool[entry.nameAndTypeIndex].nameAndTypeInfo;

    _currentLine += std::format("{}:{} {}: {}:{}", entry.classIndex, entry.nameAndTypeIndex,
                                _constantPool.getClassInfoName(entry.classIndex),
                                _constantPool.getString(nameAndTypeInfo.nameIndex),
                                _constantPool.getString(nameAndTypeInfo.descriptorIndex));
}

inline void HtmlGen::printInterfaceMethodrefInfo(const InterfaceMethodrefInfo &entry, int index) {
    printRefExtraInfo(entry);
}

inline void HtmlGen::printInvokeDynamicInfo(const InvokeDynamicInfo &entry, int index) {
    const auto &nameAndTypeInfo = _constantPool[entry.nameAndTypeIndex].nameAndTypeInfo;

    _currentLine += std::format("Bootstrap MT idx:{},N&T:{} {}:{}",
                                entry.bootstrapMethodAttrIndex,
                                entry.nameAndTypeIndex,
                                _constantPool.getString(nameAndTypeInfo.nameIndex),
                                _constantPool.getString(nameAndTypeInfo.descriptorIndex));

}

inline void HtmlGen::printNameAndTypeInfo(const NameAndTypeInfo &entry, int index) {
    _currentLine += std::format("{}:{} {}:{}", entry.nameIndex, entry.descriptorIndex,
                                _constantPool.getString(entry.nameIndex),
                                _constantPool.getString(entry.descriptorIndex));
}

void HtmlGen::printHeader(const ConstPoolBase& entry, int index) {
    _currentLine += std::to_string(index) + "\t" + tagToString(entry.tag) + "\t";
}