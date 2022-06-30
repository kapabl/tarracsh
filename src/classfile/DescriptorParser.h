//
// Created by xman on 6/28/2022.
//

#ifndef TARRASH_DESCRIPTORPARSER_H
#define TARRASH_DESCRIPTORPARSER_H

#include "DescriptorScanner.h"
#include <memory>

using namespace std;

namespace org {
namespace kapa {
namespace tarrash {

class DescriptorParser {
public:
    DescriptorParser(shared_ptr<DescriptorScanner> scanner)
        : _scanner(std::move(scanner)) { parse(); }

    DescriptorParser(const wstring &value)
        : _scanner(new DescriptorScanner(value)) {
        _scanner->step();
        parse();
    }

    const Descriptor &getDescriptor() { return _descriptor; }

private:
    Descriptor _descriptor;
    shared_ptr<DescriptorScanner> _scanner;

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
                    getFQNClassname();
                    break;
                }

                case JVM_SIGNATURE_SLASH:
                case JVM_SIGNATURE_DOT:
                case JVM_SIGNATURE_SPECIAL:
                case JVM_SIGNATURE_ENDSPECIAL: {
                    break;
                }

                case JVM_SIGNATURE_BYTE:
                    _descriptor.type = L"byte";
                    break;

                case JVM_SIGNATURE_CHAR:
                    _descriptor.type = L"char";
                    break;

                case JVM_SIGNATURE_ENUM:
                    break;

                case JVM_SIGNATURE_FLOAT:
                    _descriptor.type = L"float";
                    break;

                case JVM_SIGNATURE_BOOLEAN:
                    _descriptor.type = L"boolean";
                    break;

                case JVM_SIGNATURE_DOUBLE:
                    _descriptor.type = L"double";
                    break;

                case JVM_SIGNATURE_FUNC:
                    break;

                case JVM_SIGNATURE_ENDFUNC:
                    break;

                case JVM_SIGNATURE_INT:
                    _descriptor.type = L"int";
                    break;

                case JVM_SIGNATURE_LONG:
                    _descriptor.type = L"long";
                    break;

                case JVM_SIGNATURE_SHORT:
                    _descriptor.type = L"short";
                    break;

                case JVM_SIGNATURE_VOID:
                    _descriptor.type = L"void";
                    break;

                default:
                    assert(false);
                    break;
            }
            character = _scanner->getNextChar();
        }
    }

    void getFQNClassname() {
        auto character = _scanner->getNextChar();
        while (character != JVM_SIGNATURE_ENDCLASS) {
            _descriptor.type.push_back(character != L'/' ? character : L'.');
            character = _scanner->getNextChar();
        }
    }
};

} // namespace tarrash
} // namespace kapa
} // namespace org
#endif // TARRASH_DESCRIPTORPARSER_H
