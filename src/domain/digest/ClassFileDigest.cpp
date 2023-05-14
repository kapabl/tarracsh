#include "ClassFileDigest.h"

using namespace kapa::tarracsh::domain;
using namespace digest;
using namespace classfile;
using namespace digestUtils;
using namespace constantpool;


using namespace std;


std::vector<unsigned char> ClassFileDigest::_emptyDigestVector(DIGEST_LENGTH, 0);

ClassFileDigest::ClassFileDigest(classfile::ClassFileParser &classFileParser)
    : _classFileParser(classFileParser),
      _constantPool(classFileParser.getConstantPool()) {
}


DigestVector ClassFileDigest::digestUtf8Entry(u2 index) const {
    const auto &utf8Info = _constantPool.getEntry(index).utf8Info;
    auto result = digestUtils::digest(reinterpret_cast<const char *>(utf8Info.bytes), utf8Info.length);
    return result;
}

DigestVector ClassFileDigest::digestClassInfo(u2 classInfoIndex) const {
    if (classInfoIndex == 0) {
        return _emptyDigestVector;
    }
    const auto &classInfo = _constantPool.getEntry(classInfoIndex).classInfo;
    auto result = digestUtf8Entry(classInfo.nameIndex);

    return result;
}

DigestVector ClassFileDigest::digestAttribute(const classfile::attribute::AttributeInfo &attributeInfo) const {
    auto result = digestUtils::digest(attributeInfo.info);
    return result;
}

DigestVector ClassFileDigest::digestAttributes(
    const vector<attribute::AttributeInfo> &attributeInfos) const {
    set<DigestVector> digests;
    for (auto &attributeInfo : attributeInfos) {
        const auto attributeName = _constantPool.getEntry(attributeInfo.nameIndex).utf8Info.getAsUtf8();
        if (attributeName == CODE) {
            continue;
        }
        if (!attributeInfo.info.empty()) {
            digests.insert(digestAttribute(attributeInfo));
        }
    }
    auto result = digestSet(digests);

    return result;
}

DigestVector ClassFileDigest::digestPublicMethods() const {
    set<DigestVector> disgests;

    for (auto &method : _classFileParser.getMethods()) {
        if (accessmodifier::isPublic(method.accessFlags)) {
            disgests.insert(digestMethod(method));
        }
    }
    auto result = digestSet(disgests);
    return result;
}

DigestVector ClassFileDigest::digestPublicFields() const {
    set<DigestVector> disgests;

    for (auto &fieldInfo : _classFileParser.getFields()) {
        if (accessmodifier::isPublic(fieldInfo.accessFlags)) {
            disgests.insert(digestField(fieldInfo));
        }
    }
    auto result = digestSet(disgests);
    return result;
}

DigestVector ClassFileDigest::digestField(const FieldInfo &fieldInfo) const {

    DigestBuffer buffer;
    buffer.reserve(DIGEST_LENGTH * 4 + 256);

    buffer.append(digestUtf8Entry(fieldInfo.nameIndex))
          .append(digestAttributes(fieldInfo.attributes))
          .append(digestAttributes(fieldInfo.attributes))
          .append(_constantPool.getEntry(fieldInfo.descriptorIndex).utf8Info.bytes)
          .append(fieldInfo.accessFlags);

    auto result = digestUtils::digest(buffer);
    return result;

}

DigestVector ClassFileDigest::digestInterfaces() const {
    set<DigestVector> digests;

    for (const auto interfaceIndex : _classFileParser.getInterfaces()) {
        digests.insert(digestInterface(interfaceIndex));
    }
    auto result = digestSet(digests);
    return result;
}

DigestVector ClassFileDigest::digestInterface(const u2 interfaceIndex) const {
    DigestBuffer buffer;
    const auto &classInfo = _constantPool.getEntry(interfaceIndex).classInfo;
    buffer.append(_constantPool.getEntry(classInfo.nameIndex).utf8Info.bytes);
    auto result = digestUtils::digest(buffer);
    return result;
}

DigestVector ClassFileDigest::digestMethod(const MethodInfo &methodInfo) const {
    DigestBuffer buffer;
    buffer.reserve(DIGEST_LENGTH * 3 + 256);
    buffer.append(digestUtf8Entry(methodInfo.nameIndex))
          .append(digestAttributes(methodInfo.attributes))
          .append(_constantPool.getEntry(methodInfo.descriptorIndex).utf8Info.bytes)
          .append(methodInfo.accessFlags);

    auto result = digestUtils::digest(buffer);
    return result;
}

kapa::infrastructure::db::table::column::DigestCol ClassFileDigest::digest() const {

    const auto &mainClassInfo = _classFileParser.getMainClassInfo();
    const auto &attributes = _classFileParser.getAttributes();

    DigestBuffer buffer;
    buffer.reserve(DIGEST_LENGTH * 6 + 256);
    buffer
        .append(digestClassInfo(mainClassInfo.thisClass))
        .append(digestClassInfo(mainClassInfo.superClass))
        .append(digestAttributes(attributes))
        .append(digestPublicMethods())
        .append(digestPublicFields())
        .append(digestInterfaces())
        .append(mainClassInfo.accessFlags);

    const auto digest = digestUtils::digest(reinterpret_cast<const char *>(&*buffer.begin()), buffer.size());
    infrastructure::db::table::column::DigestCol result;
    memcpy(result.buf, &*digest.begin(), DIGEST_LENGTH);
    return result;

}
