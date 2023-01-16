
#ifndef SIGNATURE_PARSER_H
#define SIGNATURE_PARSER_H

#include <string>
#include "../reader/VectorReader.h"
#include "../constantpool/ConstantPool.h"
#include "SignatureStructures.h"

namespace kapa::tarracsh::domain::classfile::signature {
class SignatureParser {
public:
    SignatureParser(const constantpool::ConstantPool &constantPool, const attribute::AttributeInfo &attribute, reader::VectorReader &reader)
    : _constantPool(constantPool), _attribute(attribute), _reader(reader) {}

    void read(ClassSignature &signature) const;
    void read(MethodSignature &signature) const;
    void read(FieldSignature &signature) const;

    void parse(const ClassSignature& signature, ClassSignatureNode& node ) const;
    void parse(const MethodSignature& signature, MethodSignatureNode& node) const;
    void parse(const FieldSignature& signature, FieldTypeSignature& node) const;


    std::string getString(const ClassSignature& signature) const;
    std::string getString(const MethodSignature& signature) const;
    std::string getString(const FieldSignature& signature) const;

    std::string toString(const ClassSignature &signature) const;
    std::string toString(const MethodSignature &signature) const;
    std::string toString(const FieldSignature &signature) const;
    std::string toString() const;

private:
    const constantpool::ConstantPool &_constantPool;
    const attribute::AttributeInfo &_attribute;
    reader::VectorReader &_reader;





};

}

#endif
