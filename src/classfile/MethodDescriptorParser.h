//
// Created by xman on 6/28/2022.
//

#ifndef METHODDESCRIPTORPARSER_H
#define METHODDESCRIPTORPARSER_H

#include "DescriptorParser.h"
#include "signatures/SignatureScanner.h"
#include "classfile_constants.h"

namespace org::kapa::tarrash {

class MethodDescriptorParser {
public:
    MethodDescriptorParser(const std::wstring &descriptorString) : _scanner( new signatures::SignatureScanner(descriptorString)) { parse(); }

    const MethodDescriptor &getDescriptor() { return _methodDescriptor; }

private:
    std::shared_ptr<signatures::SignatureScanner> _scanner;
    MethodDescriptor _methodDescriptor;

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