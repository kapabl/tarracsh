#include <string>
#include <vector>
#include "../utils/StringUtils.h"
#include "HtmlGen.h"

using namespace org::kapa::tarracsh;
using namespace nav;
using namespace std;
// using namespace inja;


inja::Template HtmlGen::_cpoolTemplate;
inja::Template HtmlGen::_implementationsTemplate;
inja::Environment HtmlGen::_environment;


HtmlGen::HtmlGen(const ClassFileAnalyzer &classFileAnalyzer)
    : ConstantPoolPrinter(classFileAnalyzer) {

    _mainClassname = classFileAnalyzer.getMainClassname();
    _implementation = filesystem::path(_classFileAnalyzer.getContainingFile()).filename().string();
    _classRelDir = fsUtils::classnameToPath(_mainClassname);
    _classRootDir = getClassRootDir();
}


void HtmlGen::printTitle() {
    _currentLine += std::format("Constant Pool for: {}", _mainClassname);
}

filesystem::path HtmlGen::getClassRootDir() const {
    auto result = filesystem::path(TarracshApp::getOptions().outputDir) /
                  "nav" /
        _classRelDir;
    return result;
}

filesystem::path HtmlGen::getClassHtmlIndexFilename() const {
    auto result = _classRootDir / "index.html";
    return result;
}

vector<string> HtmlGen::getImplementations() const {
    vector<string> result;
    for (auto const &dirEntry : filesystem::directory_iterator(_classRootDir)) {
        if (dirEntry.is_directory()) {
            result.emplace_back(dirEntry.path().filename().string());
        }
    }
    return result;
}

std::vector<std::string> HtmlGen::renderHtmlClassIndex() {
    vector<string> result;
    inja::json data;
    data["classname"] = _mainClassname;
    data["implementations"] = getImplementations();
    result.emplace_back(render(_implementationsTemplate, data));
    return result;
}

void HtmlGen::mainClassToHtmlIndex() {
    const auto filename = getClassHtmlIndexFilename();
    const auto htmlLines = renderHtmlClassIndex();
    fsUtils::writeLines(filename.string(), htmlLines);
}


std::string HtmlGen::render(const inja::Template& compiledTemplate, const inja::json& json) {
    const auto result = _environment.render(compiledTemplate, json);
    return result;
}

filesystem::path HtmlGen::getHtmlCPoolFilename() const {
    const auto dir = filesystem::path(TarracshApp::getOptions().outputDir) /
                     "nav" /
                      _classRelDir /
                     _implementation;

    fsUtils::ensureDir(dir);

    auto result = dir / "cpool.html";
    return result;
}

vector<string> HtmlGen::renderCPoolHtml(const vector<string> &lines) const {
    vector<string> result;
    inja::json data;
    data["classname"] = _mainClassname;
    data["implementation"] = _implementation;
    data["cpoolEntries"] = stringUtils::join(lines, string("\n"));

    result.emplace_back(render(_cpoolTemplate, data));
    return result;
}

void HtmlGen::linesToHtmlFile(const vector<string> &lines) const {
    const auto filename = getHtmlCPoolFilename();
    const auto htmlLines = renderCPoolHtml(lines);
    fsUtils::writeLines(filename.string(), htmlLines);
}

void HtmlGen::print() {
    vector<string> lines;

    for (u2 index = 1u; index < _constantPool.getPoolSize(); index = _constantPool.getNextIndex(index)) {
        const auto &entry = _constantPool.getEntry(index);
        _currentLine.clear();
        printHeader(entry.base, index);
        printEntry(entry, index);
        lines.emplace_back(std::format("<div id='entry-{}' class='entry-line'>{}</div>", index, _currentLine));
    }
    linesToHtmlFile(lines);
    mainClassToHtmlIndex();
}

void HtmlGen::init() {
    _cpoolTemplate = _environment.parse(fsUtils::readFileContent("./cpool-tmpl.html"));
    _implementationsTemplate = _environment.parse(fsUtils::readFileContent("./impl-index-tmpl.html"));
}

void HtmlGen::printUtf8Info(const Utf8Info &entry, int index) {

    _currentLine += entry.getAsUtf8(true);
}

inline void HtmlGen::printStringInfo(const StringInfo &entry, int index) {

    _currentLine += std::format("{} {}",
                                generateEntryLink(entry.stringIndex),
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

std::string HtmlGen::generateLink(const std::string &classname) const {
    std::string result = std::format("{}", classname);
    return result;
}

inline void HtmlGen::printClassInfo(const ClassInfo &entry, int index) {

    const auto classname = _constantPool.getClassname(entry.nameIndex);
    _currentLine += std::format("{} <span class='link' onclick='navigateTo(\"{}\")'>{}</span>",
                                generateEntryLink(entry.nameIndex),
                                classname,
                                classname);
}

inline void HtmlGen::printMethodrefInfo(const MethodrefInfo &entry, int index) {
    printRefExtraInfo(entry);
}

inline void HtmlGen::printMethodHandleInfo(const MethodHandleInfo &entry, int index) {
    _currentLine += std::format("ref-kind:{}, {}",
                                refKindToString(entry.referenceKind),
                                generateEntryLink(entry.referenceIndex));
}

inline void HtmlGen::printMethodTypeInfo(const MethodTypeInfo &entry, int index) {
    _currentLine += _constantPool.getEntry(entry.descriptorIndex).utf8Info.getAsUtf8();
}

inline void HtmlGen::printFieldrefInfo(const FieldrefInfo &entry, int index) {
    printRefExtraInfo(entry);
}

inline void HtmlGen::printModuleInfo(const ModuleInfo &entry, int index) {
    _currentLine += _constantPool.getEntry(entry.nameIndex).utf8Info.getAsUtf8();
}

void HtmlGen::printRefExtraInfo(const MemberInfo &entry) {
    const auto &nameAndTypeInfo = _constantPool.getEntry(entry.nameAndTypeIndex).nameAndTypeInfo;

    _currentLine += std::format("{}:{} {}: {}:{}",
                                generateEntryLink(entry.classIndex),
                                generateEntryLink(entry.nameAndTypeIndex),
                                _constantPool.getClassInfoName(entry.classIndex),
                                _constantPool.getString(nameAndTypeInfo.nameIndex),
                                _constantPool.getString(nameAndTypeInfo.descriptorIndex));
}

inline void HtmlGen::printInterfaceMethodrefInfo(const InterfaceMethodrefInfo &entry, int index) {
    printRefExtraInfo(entry);
}

std::string HtmlGen::generateEntryLink(int index) const {
    auto result = std::format("<a class='entry-link' href='#entry-{}' onclick='selectEntryById(\"entry-{}\");'>{}</a>", 
        index, 
        index, 
        index);
    return result;

}

inline void HtmlGen::printInvokeDynamicInfo(const InvokeDynamicInfo &entry, int index) {
    const auto &nameAndTypeInfo = _constantPool.getEntry(entry.nameAndTypeIndex).nameAndTypeInfo;
    _currentLine += std::format("Bootstrap MT {}, N&T:{} {}:{}",
                                generateEntryLink(entry.bootstrapMethodAttrIndex),
                                generateEntryLink(entry.nameAndTypeIndex),
                                _constantPool.getString(nameAndTypeInfo.nameIndex),
                                _constantPool.getString(nameAndTypeInfo.descriptorIndex));

}

inline void HtmlGen::printNameAndTypeInfo(const NameAndTypeInfo &entry, int index) {
    _currentLine += std::format("{}:{} {}:{}",
                                generateEntryLink(entry.nameIndex),
                                generateEntryLink(entry.descriptorIndex),
                                _constantPool.getString(entry.nameIndex),
                                _constantPool.getString(entry.descriptorIndex));
}

void HtmlGen::printHeader(const ConstPoolBase &entry, int index) {
    _currentLine += std::format("<span class='index'>{}</span><span class='type'>{}</span>",
                                index,
                                tagToString(entry.tag));
}
