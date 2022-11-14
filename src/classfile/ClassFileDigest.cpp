#include "ClassFileDigest.h"

using namespace org::kapa::tarracsh;
using namespace attributes;
using namespace digestUtils;
using namespace std;

ClassFileDigest::ClassFileDigest(ClassFileAnalyzer &classFileAnalyzer)
    : _classFileAnalyzer(classFileAnalyzer),
      _constantPool(classFileAnalyzer.getConstantPool()) {
}


DigestVector ClassFileDigest::digestUtf8Entry(u2 index) const {
    const auto &utf8Info = _constantPool[index].utf8Info;
    auto result = digestUtils::digest(reinterpret_cast<const char*>(utf8Info.bytes), utf8Info.length);
    return result;
}

DigestVector ClassFileDigest::digestClassInfo(u2 classInfoIndex) const {
    const auto &classInfo = _constantPool[classInfoIndex].classInfo;
    auto result = digestUtf8Entry(classInfo.nameIndex);
    return result;
}

DigestVector ClassFileDigest::digestAttribute(const AttributeInfo &attributeInfo) const {
    auto result = digestUtils::digest(attributeInfo.info);
    return result;
}

DigestVector ClassFileDigest::digest(
    const vector<AttributeInfo> &attributeInfos) const {
    set<DigestVector> disgests;
    for (auto &attributeInfo : attributeInfos) {
        if (!attributeInfo.info.empty()) {
            disgests.insert(digestAttribute(attributeInfo));
        }
    }
    auto result = digestSet(disgests);

    return result;
}

DigestVector ClassFileDigest::digestPublicMethods() const {
    set<DigestVector> disgests;

    for (auto &method : _classFileAnalyzer.getMethods()) {
        if (accessModifiers::AccessModifiers::isPublic(method.accessFlags)) {
            disgests.insert(digestMethod(method));
        }
    }
    auto result = digestSet(disgests);
    return result;
}

DigestVector ClassFileDigest::digestPublicFields() const {
    set<DigestVector> disgests;

    for (auto &fieldInfo : _classFileAnalyzer.getFields()) {
        if (accessModifiers::AccessModifiers::isPublic(fieldInfo.accessFlags)) {
            disgests.insert(digestField(fieldInfo));
        }
    }
    auto result = digestSet(disgests);
    return result;
}

DigestVector ClassFileDigest::digestField(const FieldInfo &fieldInfo) const {

    vector<char> buffer;
    ranges::copy(digestUtf8Entry(fieldInfo.nameIndex), buffer.end());
    ranges::copy(digest(fieldInfo.attributes), buffer.end());
    ranges::copy(_constantPool[fieldInfo.descriptorIndex].utf8Info.bytes, buffer.end());
    buffer.push_back(static_cast<char>(fieldInfo.accessFlags & 0x0ff));
    buffer.push_back(static_cast<char>(fieldInfo.accessFlags >> 8));
    auto result = digestUtils::digest(buffer);
    return result;

}

DigestVector ClassFileDigest::digestInterfaces() const {
    set<DigestVector> digests;

    for (const auto interfaceIndex : _classFileAnalyzer.getInterfaces()) {
        digests.insert(digestInterface(interfaceIndex));
    }
    auto result = digestSet(digests);
    return result;
}

DigestVector ClassFileDigest::digestInterface(const u2 interfaceIndex) const {
    DigestBuffer buffer;
    const auto &classInfo = _constantPool[interfaceIndex].classInfo;
    ranges::copy(_constantPool[classInfo.nameIndex].utf8Info.bytes, buffer.end());
    auto result = digestUtils::digest(buffer);
    return result;
}

DigestVector ClassFileDigest::digestMethod(const MethodInfo &methodInfo) const {
    vector<char> buffer;
    ranges::copy(digestUtf8Entry(methodInfo.nameIndex), buffer.end());
    ranges::copy(digest(methodInfo.attributes), buffer.end());
    ranges::copy(_constantPool[methodInfo.descriptorIndex].utf8Info.bytes, buffer.end());
    buffer.push_back(static_cast<char>(methodInfo.accessFlags & 0x0ff));
    buffer.push_back(static_cast<char>(methodInfo.accessFlags >> 8));
    auto result = digestUtils::digest(buffer);
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
