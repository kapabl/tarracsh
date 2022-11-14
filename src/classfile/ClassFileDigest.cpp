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
    auto result = digestUtils::digest(reinterpret_cast<const char *>(utf8Info.bytes), utf8Info.length);
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

    DigestBuffer buffer;
    buffer.append(digestUtf8Entry(fieldInfo.nameIndex))
          .append(digest(fieldInfo.attributes))
          .append(digest(fieldInfo.attributes))
          .append(_constantPool[fieldInfo.descriptorIndex].utf8Info.bytes)
          .append(fieldInfo.accessFlags);

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
    buffer.append(_constantPool[classInfo.nameIndex].utf8Info.bytes);
    auto result = digestUtils::digest(buffer);
    return result;
}

DigestVector ClassFileDigest::digestMethod(const MethodInfo &methodInfo) const {
    DigestBuffer buffer;
    buffer.append(digestUtf8Entry(methodInfo.nameIndex))
          .append(digest(methodInfo.attributes))
          .append(_constantPool[methodInfo.descriptorIndex].utf8Info.bytes)
          .append(methodInfo.accessFlags);

    auto result = digestUtils::digest(buffer);
    return result;
}

tables::DigestColumn ClassFileDigest::digest() const {

    const auto &mainClassInfo = _classFileAnalyzer.getMainClassInfo();
    const auto &attributes = _classFileAnalyzer.getAttributes();

    DigestBuffer buffer;
    buffer
        .append(digestClassInfo(mainClassInfo.thisClass))
        .append(digestClassInfo(mainClassInfo.superClass))
        .append(digest(attributes))
        .append(digestPublicMethods())
        .append(digestPublicFields())
        .append(digestInterfaces())
        .append(mainClassInfo.accessFlags);;

    const auto digest = digestUtils::digest(reinterpret_cast<const char *>(&*buffer.begin()), buffer.size());
    tables::DigestColumn result;
    memcpy(result.buf, &*digest.begin(), MD5_DIGEST_LENGTH);
    return result;

}
