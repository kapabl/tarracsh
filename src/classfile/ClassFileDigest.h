#ifndef TARRACSH_CLASSFILE_DIGEST_H
#define TARRACSH_CLASSFILE_DIGEST_H

#include <vector>
#include "constpool/ConstantPool.h"
#include "AttributesManager.h"
#include "ClassFileAnalyzer.h"

#include "../tables/ClassfilesTable.h"


namespace org::kapa::tarracsh {

class ClassFileDigest final {

public:
    explicit ClassFileDigest(ClassFileAnalyzer& classFileAnalyzer);

    ClassFileDigest(const ClassFileDigest &) = delete;
    ClassFileDigest(const ClassFileDigest &&) = delete;
    ClassFileDigest &operator=(const ClassFileDigest &) = delete;
    ClassFileDigest &operator=(const ClassFileDigest &&) = delete;


    ~ClassFileDigest() = default;
    [[nodiscard]] db::tables::columns::DigestCol digest() const;


private:

    ClassFileAnalyzer& _classFileAnalyzer;
    ConstantPool& _constantPool;
    [[nodiscard]] std::vector<unsigned char> digestClassInfo(u2 classInfoIndex) const;
    [[nodiscard]] std::vector<unsigned char> digestUtf8Entry(u2 index) const;

    [[nodiscard]] std::vector<unsigned char> digestAttribute(const attributes::AttributeInfo & attributeInfo) const;
    [[nodiscard]] std::vector<unsigned char> digestAttributes(const std::vector<attributes::AttributeInfo>& attributeInfos) const;
    [[nodiscard]] std::vector<unsigned char> digestMethod(const MethodInfo& methodInfo) const;
    [[nodiscard]] std::vector<unsigned char> digestPublicMethods() const;
    [[nodiscard]] std::vector<unsigned char> digestPublicFields() const;
    [[nodiscard]] std::vector<unsigned char> digestField(const FieldInfo& fieldInfo) const;
    [[nodiscard]] std::vector<unsigned char> digestInterfaces() const;
    [[nodiscard]] std::vector<unsigned char> digestInterface(const u2 interfaceIndex) const;

};


}
#endif
