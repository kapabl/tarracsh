#include "ClassFileDigest.h"

using namespace org::kapa::tarracsh;
using namespace attributes;
using namespace std;

ClassFileDigest::ClassFileDigest(ClassFileAnalyzer &classFileAnalyzer)
    : _classFileAnalyzer(classFileAnalyzer),
      _constantPool(classFileAnalyzer.getConstantPool()) {
}


string ClassFileDigest::digestUtf8Entry(u2 index) const {
    const auto &utf8Info = _constantPool[index].utf8Info;
    auto digest = digestUtils::digest(reinterpret_cast<const char *>(utf8Info.bytes), utf8Info.length);

    auto result = digestUtils::digestToString(digest);
    return result;
}

string ClassFileDigest::digestClassInfo(u2 classInfoIndex) const {
    const auto &classInfo = _constantPool[classInfoIndex].classInfo;
    auto result = digestUtf8Entry(classInfo.nameIndex);
    return result;
}

std::string ClassFileDigest::digest(const attributes::AttributeInfo &attributeInfo) {
    auto result = digestUtils::digestToString(attributeInfo.info);
    return result;
}

string ClassFileDigest::digest(
    const vector<attributes::AttributeInfo> &attributeInfos) const {
    set<string> md5Set;
    for (auto &attributeInfo : attributeInfos) {
        if (attributeInfo.info.size() > 0) {
            md5Set.insert(digest(attributeInfo));
        }
    }
    auto result = digestUtils::md5SetAsString(md5Set);

    return result;
}

string ClassFileDigest::digestPublicMethods() const {
    set<string> md5Set;

    for (auto &method : _classFileAnalyzer.getMethods()) {
        if (accessModifiers::AccessModifiers::isPublic(method.accessFlags)) {
            md5Set.insert(digest(method));
        }
    }
    auto result = digestUtils::md5SetAsString(md5Set);
    return result;
}

string ClassFileDigest::digestPublicFields() const {
    set<string> md5Set;

    for (auto &fieldInfo : _classFileAnalyzer.getFields()) {
        if (accessModifiers::AccessModifiers::isPublic(fieldInfo.accessFlags)) {
            md5Set.insert(digest(fieldInfo));
        }
    }
    auto result = digestUtils::md5SetAsString(md5Set);
    return result;
}

string ClassFileDigest::digest(const FieldInfo &fieldInfo) const {

    vector<char> buffer;
    ranges::copy(digestUtf8Entry(fieldInfo.nameIndex), buffer.end());
    ranges::copy(digest(fieldInfo.attributes), buffer.end());
    ranges::copy(_constantPool[fieldInfo.descriptorIndex].utf8Info.bytes, buffer.end());
    buffer.push_back(static_cast<char>(fieldInfo.accessFlags & 0x0ff));
    buffer.push_back(static_cast<char>(fieldInfo.accessFlags >> 8));
    auto result = digestUtils::digestToString(buffer);
    return result;

}

string ClassFileDigest::digestInterfaces() const {
    set<string> md5Set;

    for (const auto interfaceIndex : _classFileAnalyzer.getInterfaces()) {
        md5Set.insert(digestInterface(interfaceIndex));
    }
    auto result = digestUtils::md5SetAsString(md5Set);
    return result;
}

std::string ClassFileDigest::digestInterface(const u2 interfaceIndex) const {
    const auto &classInfo = _constantPool[interfaceIndex].classInfo;
    const auto encodedName = _constantPool[classInfo.nameIndex].utf8Info.getValue();
    auto result = digestUtils::md5AsString(encodedName);
    return result;
}

string ClassFileDigest::digest(const MethodInfo &methodInfo) const {
    vector<char> buffer;
    ranges::copy(digestUtf8Entry(methodInfo.nameIndex), buffer.end());
    ranges::copy(digest(methodInfo.attributes), buffer.end());
    ranges::copy(_constantPool[methodInfo.descriptorIndex].utf8Info.bytes, buffer.end());
    buffer.push_back(static_cast<char>(methodInfo.accessFlags & 0x0ff));
    buffer.push_back(static_cast<char>(methodInfo.accessFlags >> 8));
    auto result = digestUtils::digestToString(buffer);
    return result;
}

tables::DigestColumn ClassFileDigest::digest() const {

    const auto& mainClassInfo = _classFileAnalyzer.getMainClassInfo();
    const auto& attributes = _classFileAnalyzer.getAttributes();

    vector<char> buffer;
    ranges::copy(digestClassInfo(mainClassInfo.thisClass), buffer.end());
    ranges::copy(digestClassInfo(mainClassInfo.superClass), buffer.end());
    ranges::copy(digest(attributes), buffer.end());
    ranges::copy(digestPublicMethods(), buffer.end());
    ranges::copy(digestPublicFields(), buffer.end());
    ranges::copy(digestInterfaces(), buffer.end());
    buffer.push_back(static_cast<char>(mainClassInfo.accessFlags & 0x0ff));
    buffer.push_back(static_cast<char>(mainClassInfo.accessFlags >> 8));

    const auto digest = digestUtils::digest(&*buffer.begin(), buffer.size());
    tables::DigestColumn result;
    memcpy(result.buf, &*digest.begin(), MD5_DIGEST_LENGTH);
    return result;
    
}
