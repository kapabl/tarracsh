#include "SignatureParser.h"
//
// #include "grammar/JvmSignatureLexer.h"
// #include "grammar/JvmSignatureParser.h"
// #include "grammar/JavaSignatureListenerImpl.h"

using namespace kapa::tarracsh::domain::classfile::signature;
using namespace kapa::tarracsh::domain::classfile::attribute;
using namespace std;
using parser::Parser;

/**
 * example: (
 * [C
 *  [
 *      Ljava/lang/Character;
 *      Ljava/util/Map<
 *          Ljava/lang/Character;
 *          Lkotlin/UInt;>;
 *      Ljava/util/List<
 *          +Lorg/kapa/reti/lexers/skippers/Skipper;
 *          >;
 *      Ljava/util/List<
 *          Lorg/kapa/reti/lexers/Keyword;
 *          >;
 *      Ljava/util/List<
 *          Lorg/kapa/reti/lexers/lexemes/Lexeme;
 *          >;
 *  )V
 */

void SignatureParser::read(ClassSignature &classSignature) const {

    classSignature.nameIndex = _attribute.nameIndex;
    classSignature.length = _attribute.length;
    classSignature.signatureIndex = _reader.readU2();

}

void SignatureParser::read(MethodSignature &signature) const {
    signature.nameIndex = _attribute.nameIndex;
    signature.length = _attribute.length;
    signature.signatureIndex = _reader.readU2();

}

void SignatureParser::read(FieldSignature &signature) const {
    signature.nameIndex = _attribute.nameIndex;
    signature.length = _attribute.length;
    signature.signatureIndex = _reader.readU2();
}

string SignatureParser::getString(const ClassSignature &signature) const {

    const auto signatureString = _constantPool.getString(signature.signatureIndex);

    try {
        const auto parser = Parser::make();
        auto tree = parser->parseClassSig(signatureString);
        (void)tree;
    } catch (const std::exception &) {
        // fall back to raw string if parsing fails
    }
    return signatureString;

}

string SignatureParser::getString(const MethodSignature &signature) const {
    const auto signatureString = _constantPool.getString(signature.signatureIndex);

    try {
        const auto parser = Parser::make();
        auto tree = parser->parseMethodSig(signatureString);
        (void)tree;
    } catch (const std::exception &) {
        // ignore parse failures and return original descriptor
    }
    return signatureString;
}

string SignatureParser::getString(const FieldSignature &signature) const {
    const auto signatureString = _constantPool.getString(signature.signatureIndex);

    try {
        const auto parser = Parser::make();
        auto tree = parser->parseTypeSig(signatureString);
        (void)tree;
    } catch (const std::exception &) {
        // ignore parse failures and return original descriptor
    }
    return signatureString;
}

string SignatureParser::toString() const {
    string result;

    switch (_attribute.owner) {

        case AttributeOwner::ClassFile: {
            ClassSignature signature;
            read(signature);
            result = getString(signature);
            break;
        }
        case AttributeOwner::Method: {
            MethodSignature signature;
            read(signature);
            result = getString(signature);
            break;
        }
        case AttributeOwner::Field: {
            FieldSignature signature;
            read(signature);
            result = getString(signature);
            break;
        }
    }
    return result;
}
