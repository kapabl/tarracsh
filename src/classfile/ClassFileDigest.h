#ifndef TARRACSH_CLASSFILE_DIGEST_H
#define TARRACSH_CLASSFILE_DIGEST_H

#include <filesystem>
#include <string>
#include <vector>
#include "Tarracsh.h"

#include "ClassFileStructures.h"
#include "ConstantPool.h"
#include "AttributesManager.h"
#include "ClassFileAnalyzer.h"

#include "tables/DigestTable.h"


namespace org::kapa::tarracsh {

class ClassFileDigest final {

public:
    explicit ClassFileDigest(ClassFileAnalyzer& classFileAnalyzer);

    ClassFileDigest(const ClassFileDigest &) = delete;
    ClassFileDigest(const ClassFileDigest &&) = delete;
    ClassFileDigest &operator=(const ClassFileDigest &) = delete;
    ClassFileDigest &operator=(const ClassFileDigest &&) = delete;


    ~ClassFileDigest() = default;
    [[nodiscard]] tables::MD5 digest() const;


private:

    ClassFileAnalyzer& _classFileAnalyzer;
    ConstantPool& _constantPool;
  
    [[nodiscard]] std::string digestUtf8Entry(u2 index) const;
    [[nodiscard]] std::string digestClassInfo(u2 classInfoIndex) const;
    [[nodiscard]] std::string digest(const attributes::AttributeInfo & attributeInfo) const;
    [[nodiscard]] std::string digest(const std::vector<attributes::AttributeInfo>& attributeInfos) const;
    [[nodiscard]] std::string digest(const MethodInfo& methodInfo) const;
    [[nodiscard]] std::string digestPublicMethods() const;
    [[nodiscard]] std::string digestPublicFields() const;
    [[nodiscard]] std::string digest(const FieldInfo& fieldInfo) const;
    [[nodiscard]] std::string digestInterfaces() const;
    [[nodiscard]] std::string digestInterface(const u2 interfaceIndex) const;
    void writeUtf8InfoToStream(const u2 constPoolIndex, Poco::DigestOutputStream& stream) const;
 
};


}
#endif
