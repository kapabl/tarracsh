#include "SignatureParser.h"

using namespace org::kapa::tarrash::signatures;
using namespace org::kapa::tarrash::attributes;
using namespace std;

void SignatureParser::readSignature(ClassSignature &classSignature) const {
    classSignature.nameIndex = _attribute.nameIndex;
    classSignature.length = _attribute.length;
    classSignature.signatureIndex = _reader.readU2();

}

void SignatureParser::readSignature(MethodSignature &signature) const {}

void SignatureParser::readSignature(FieldSignature &signature) const {}

wstring SignatureParser::toString(const ClassSignature &signature) const {
    wstring result;//TODO
    return result;
}

std::wstring SignatureParser::toString(const MethodSignature &signature) const {
    wstring result; // TODO
    return result;
}

std::wstring SignatureParser::toString(const FieldSignature &signature) const {
    wstring result; // TODO
    return result;
}

wstring SignatureParser::toString() const {
    wstring result;

    switch (_attribute.owner) {

        case AttributeOwner::ClassFile: {
            ClassSignature signature;
            readSignature(signature);
            result = toString(signature);
            break;
        }
        case AttributeOwner::Method: {
            ClassSignature signature;
            readSignature(signature);
            result = toString(signature);
            break;
        }
        case AttributeOwner::Field: {
            FieldSignature signature;
            readSignature(signature);
            result = toString(signature);
            break;
        }
    }
    return result;
}
