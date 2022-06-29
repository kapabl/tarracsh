//
// Created by xman on 6/28/2022.
//

#ifndef TARRASH_METHODDESCRIPTORPARSER_H
#define TARRASH_METHODDESCRIPTORPARSER_H

#include "DescriptorParser.h"
#include "DescriptorScanner.h"
#include "includes/classfile_constants.h"

namespace org { namespace kapa { namespace tarrash {

class MethodDescriptorParser {
    public:
        MethodDescriptorParser(const wstring &fieldDescriptor) : _scanner(fieldDescriptor) { parse(); }

        const MethodDescriptor &getDescriptor() { return _methodDescriptor; }

    private:
        DescriptorScanner _scanner;
        MethodDescriptor _methodDescriptor;

        void parse() {

            auto character = _scanner.getNextChar();

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
                character = _scanner.getNextChar();
            }
        }

        void parseParameters() {
            _scanner.step();
            while (_scanner.currentChar() != JVM_SIGNATURE_ENDFUNC) {
                DescriptorParser parser(_scanner);
                _methodDescriptor.arguments.push_back(parser.getDescriptor());
            }
        }

        void parseReturnType() {
            _scanner.step();
            DescriptorParser parser(_scanner);
            _methodDescriptor.returnType = parser.getDescriptor();
        }
};

}}}    // namespace org::kapa::tarrash
#endif // TARRASH_METHODDESCRIPTORPARSER_H
