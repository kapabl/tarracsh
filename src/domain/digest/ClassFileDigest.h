#ifndef TARRACSH_CLASSFILE_DIGEST_H
#define TARRACSH_CLASSFILE_DIGEST_H

#include <vector>
#include "../classfile/constantpool/ConstantPool.h"
#include "../db/columns/Columns.h"
#include "../classfile/AttributesManager.h"
#include "../classfile/ClassFileParser.h"

namespace kapa::tarracsh::domain::digest {

using classfile::constantpool::u1;
using classfile::constantpool::u2;
using classfile::constantpool::u4;

class ClassFileDigest final {

public:
    explicit ClassFileDigest(classfile::ClassFileParser& classFileParser);

    ClassFileDigest(const ClassFileDigest &) = delete;
    ClassFileDigest(const ClassFileDigest &&) = delete;
    ClassFileDigest &operator=(const ClassFileDigest &) = delete;
    ClassFileDigest &operator=(const ClassFileDigest &&) = delete;


    ~ClassFileDigest() = default;
    [[nodiscard]] infrastructure::db::tables::columns::DigestCol digest() const;


private:
    classfile::ClassFileParser& _classFileParser;
    classfile::constantpool::ConstantPool& _constantPool;
    static std::vector<unsigned char> _emptyDigestVector;
    [[nodiscard]] std::vector<unsigned char> digestClassInfo(u2 classInfoIndex) const;
    [[nodiscard]] std::vector<unsigned char> digestUtf8Entry(u2 index) const;

    [[nodiscard]] std::vector<unsigned char> digestAttribute(const classfile::attribute::AttributeInfo & attributeInfo) const;
    [[nodiscard]] std::vector<unsigned char> digestAttributes(const std::vector<classfile::attribute::AttributeInfo>& attributeInfos) const;
    [[nodiscard]] std::vector<unsigned char> digestMethod(const classfile::constantpool::MethodInfo& methodInfo) const;
    [[nodiscard]] std::vector<unsigned char> digestPublicMethods() const;
    [[nodiscard]] std::vector<unsigned char> digestPublicFields() const;
    [[nodiscard]] std::vector<unsigned char> digestField(const classfile::constantpool::FieldInfo& fieldInfo) const;
    [[nodiscard]] std::vector<unsigned char> digestInterfaces() const;
    [[nodiscard]] std::vector<unsigned char> digestInterface(const u2 interfaceIndex) const;

};


}
#endif
