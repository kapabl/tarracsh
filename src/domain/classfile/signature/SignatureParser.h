
#ifndef SIGNATURE_PARSER_H
#define SIGNATURE_PARSER_H

// #include "grammar/antlr4-runtime.h"

// #include "grammar/JvmSignatureParser.h"

#include <functional>
#include <string>


#include "parser/Parser.h"
#include "src/domain/classfile/reader/VectorReader.h"
#include "src/domain/classfile/constantpool/ConstantPool.h"
#include "src/domain/classfile/types/SignatureStructures.h"

namespace antlr4::tree {
    class ParseTree;
}

namespace kapa::tarracsh::domain::classfile::signature {

namespace parser {
class JvmSignatureParser;
}

class SignatureParser {
public:
    SignatureParser(const constantpool::ConstantPool &constantPool, const attribute::AttributeInfo &attribute, reader::VectorReader &reader)
        : _constantPool(constantPool), _attribute(attribute), _reader(reader) {}

    void read(ClassSignature &signature) const;
    void read(MethodSignature &signature) const;
    void read(FieldSignature &signature) const;


    [[nodiscard]] std::string getString(const ClassSignature& signature) const;
    [[nodiscard]] std::string getString(const MethodSignature& signature) const;
    [[nodiscard]] std::string getString(const FieldSignature& signature) const;
    [[nodiscard]] std::string toString() const;


private:
    const constantpool::ConstantPool &_constantPool;
    const attribute::AttributeInfo &_attribute;
    reader::VectorReader &_reader;
    



};

}

#endif
