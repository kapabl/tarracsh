
#ifndef SIGNATURE_PARSER_H
#define SIGNATURE_PARSER_H

#include <string>
#include "../VectorReader.h"
#include "../ConstantPool.h"
#include "SignatureStructures.h"

namespace org::kapa::tarracsh::signatures {
class SignatureParser {
public:
    SignatureParser(const ConstantPool &constantPool, const attributes::AttributeInfo &attribute, readers::VectorReader &reader)
    : _constantPool(constantPool), _attribute(attribute), _reader(reader) {}

    void read(ClassSignature &signature) const;
    void read(MethodSignature &signature) const;
    void read(FieldSignature &signature) const;

    void parse(const ClassSignature& signature, ClassSignatureNode& node ) const;
    void parse(const MethodSignature& signature, MethodSignatureNode& node) const;
    void parse(const FieldSignature& signature, FieldTypeSignature& node) const;


    std::wstring getString(const ClassSignature& signature) const;
    std::wstring getString(const MethodSignature& signature) const;
    std::wstring getString(const FieldSignature& signature) const;

    std::wstring toString(const ClassSignature &signature) const;
    std::wstring toString(const MethodSignature &signature) const;
    std::wstring toString(const FieldSignature &signature) const;
    std::wstring toString() const;

private:
    const ConstantPool &_constantPool;
    const attributes::AttributeInfo &_attribute;
    readers::VectorReader &_reader;





};

}

#endif
