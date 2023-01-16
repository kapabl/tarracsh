//
// Created by xman on 6/28/2022.
//

#ifndef METHODDESCRIPTORPARSER_H
#define METHODDESCRIPTORPARSER_H

#include "DescriptorParser.h"
#include "signature/SignatureScanner.h"
#include "ClassFileConstants.h"

namespace kapa::tarracsh::domain::classfile {

class MethodDescriptorParser {
public:
    explicit MethodDescriptorParser(const std::string &descriptorString)
        : _scanner(new signature::SignatureScanner(descriptorString)) {
        parse();
    }

    const constantpool::MethodDescriptor &getDescriptor() { return _methodDescriptor; }

private:
    std::shared_ptr<signature::SignatureScanner> _scanner;
    constantpool::MethodDescriptor _methodDescriptor;

    void parse() {

        auto character = _scanner->getNextChar();

        while (character != 0) {
            switch (character) {
                case JVM_SIGNATURE_FUNC: {
                    parseParameters();
                    parseReturnType();
                    break;
                }
                default:
                    assert(false);
                    break;
            }
            character = _scanner->getNextChar();
        }
    }

    void parseParameters() {
        _scanner->step();
        while (_scanner->currentChar() != JVM_SIGNATURE_ENDFUNC) {
            DescriptorParser parser(_scanner);
            _methodDescriptor.arguments.push_back(parser.getDescriptor());
        }
    }

    void parseReturnType() {
        _scanner->step();
        DescriptorParser parser(_scanner);
        _methodDescriptor.returnType = parser.getDescriptor();
    }
};

}

#endif
