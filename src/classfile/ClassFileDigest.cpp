#include "ClassFileDigest.h"

using namespace org::kapa::tarracsh;
using namespace attributes;
using namespace std;

ClassFileDigest::ClassFileDigest(ClassFileAnalyzer &classFileAnalyzer)
    : _classFileAnalyzer(classFileAnalyzer),
      _constantPool(classFileAnalyzer.getConstantPool()) {
}


string ClassFileDigest::digestUtf8Entry(u2 index) const {
    const auto& utf8Info = _constantPool[index].utf8Info;
    auto digest = digestUtils::md5( reinterpret_cast<const char*>(utf8Info.bytes), utf8Info.length);

    auto result = digestUtils::digestToString(digest);
    return result;
}

string ClassFileDigest::digestClassInfo(u2 classInfoIndex) const {
    const auto& classInfo = _constantPool[classInfoIndex].classInfo;
    auto result = digestUtf8Entry(classInfo.nameIndex);
    return result;
}

std::string ClassFileDigest::digest(const attributes::AttributeInfo &attributeInfo) const {
    Poco::MD5Engine md5;
    Poco::DigestOutputStream stream(md5);
    stream.write(reinterpret_cast<const char *>(&*attributeInfo.info.begin()), attributeInfo.length);
    stream.close();
    auto digest = md5.digest();
    auto result = digestUtils::digestToString(digest);
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

    Poco::MD5Engine md5;
    Poco::DigestOutputStream stream(md5);
    stream << digestUtf8Entry(fieldInfo.nameIndex)
        << fieldInfo.accessFlags
        << digest(fieldInfo.attributes);

    writeUtf8InfoToStream(fieldInfo.descriptorIndex, stream);

    stream.close();
    auto digest = md5.digest();
    auto result = digestUtils::digestToString(digest);
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

void ClassFileDigest::writeUtf8InfoToStream(const u2 constPoolIndex, Poco::DigestOutputStream &stream) const {
    const auto &utf8Info = _constantPool[constPoolIndex].utf8Info;
    stream.write(reinterpret_cast<const char *>(utf8Info.bytes), utf8Info.length);
}

string ClassFileDigest::digest(const MethodInfo &methodInfo) const {

    Poco::MD5Engine md5;
    Poco::DigestOutputStream stream(md5);
    stream << digestUtf8Entry(methodInfo.nameIndex)
        << methodInfo.accessFlags
        << digest(methodInfo.attributes);

    writeUtf8InfoToStream(methodInfo.descriptorIndex, stream);

    stream.close();
    auto digest = md5.digest();
    auto result = digestUtils::digestToString(digest);
    return result;

}

tables::Md5Column ClassFileDigest::digest() const {

    Poco::MD5Engine md5;
    Poco::DigestOutputStream stream(md5);
    const auto& mainClassInfo = _classFileAnalyzer.getMainClassInfo();
    const auto& attributes = _classFileAnalyzer.getAttributes();
    stream << digestClassInfo(mainClassInfo.thisClass)
        << mainClassInfo.accessFlags
        << digestClassInfo(mainClassInfo.superClass)
        << digest(attributes)
        << digestPublicMethods()
        << digestPublicFields()
        << digestInterfaces();

    stream.close();

    tables::Md5Column result;
    memcpy(result.buf, &*md5.digest().begin(), MD5_DIGEST_LENGTH);
    return result;
}
