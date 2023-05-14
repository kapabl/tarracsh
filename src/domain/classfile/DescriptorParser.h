//
// Created by xman on 6/28/2022.
//

#ifndef TARRACSH_DESCRIPTORPARSER_H
#define TARRACSH_DESCRIPTORPARSER_H

#include "signature/SignatureScanner.h"
#include "constantpool/ConstpoolStructures.h"
#include <memory>


namespace kapa::tarracsh::domain::classfile {

class DescriptorParser {
public:
    explicit DescriptorParser(std::shared_ptr<signature::SignatureScanner> scanner)
        : _scanner(std::move(scanner)) { parse(); }

    explicit DescriptorParser(const std::string &value)
        : _scanner(new signature::SignatureScanner(value)) {
        _scanner->step();
        parse();
    }

    const constantpool::Descriptor &getDescriptor() { return _descriptor; }

private:
    constantpool::Descriptor _descriptor;
    std::shared_ptr<signature::SignatureScanner> _scanner;

    void parse() {

        auto character = _scanner->currentChar();

        while (character != 0 && _descriptor.type.empty() && character != JVM_SIGNATURE_ENDFUNC) {
            switch (character) {
                case JVM_SIGNATURE_ARRAY: {
                    _descriptor.isArray = true;
                    _descriptor.dimensions++;
                    break;
                }

                case JVM_SIGNATURE_CLASS: {
                    _descriptor.isClass = true;
                    getFQClassname();
                    break;
                }

                case JVM_SIGNATURE_FUNC:
                case JVM_SIGNATURE_ENDFUNC:
                case JVM_SIGNATURE_SLASH:
                case JVM_SIGNATURE_DOT:
                case JVM_SIGNATURE_ENUM:
                case JVM_SIGNATURE_SPECIAL:
                case JVM_SIGNATURE_ENDSPECIAL: {
                    break;
                }

                case JVM_SIGNATURE_BYTE:
                    _descriptor.type = "byte";
                    break;

                case JVM_SIGNATURE_CHAR:
                    _descriptor.type = "char";
                    break;

                case JVM_SIGNATURE_FLOAT:
                    _descriptor.type = "float";
                    break;

                case JVM_SIGNATURE_BOOLEAN:
                    _descriptor.type = "boolean";
                    break;

                case JVM_SIGNATURE_DOUBLE:
                    _descriptor.type = "double";
                    break;

                case JVM_SIGNATURE_INT:
                    _descriptor.type = "int";
                    break;

                case JVM_SIGNATURE_LONG:
                    _descriptor.type = "long";
                    break;

                case JVM_SIGNATURE_SHORT:
                    _descriptor.type = "short";
                    break;

                case JVM_SIGNATURE_VOID:
                    _descriptor.type = "void";
                    break;

                default:
                    _descriptor.type = _scanner->getSignatureString();
                    break;
            }
            character = _scanner->getNextChar();
        }
    }

    void getFQClassname() {
        auto character = _scanner->getNextChar();
        while (character != JVM_SIGNATURE_ENDCLASS) {
            _descriptor.type.push_back(character != '/' ? character : '.');
            character = _scanner->getNextChar();
        }
    }
};

}

#endif // TARRASH_DESCRIPTORPARSER_H
