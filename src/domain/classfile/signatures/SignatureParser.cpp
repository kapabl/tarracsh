#include "rules/Rule.h"
#include "rules/Kleene.h"
#include "rules/Or.h"
#include "rules/Terminal.h"
#include "rules/Optional.h"
#include "rules/JvmIdentifier.h"
#include "rules/RuleFuncs.h"

#include "SignatureParser.h"

#include "rules/ParsingRules.h"

using namespace kapa::tarracsh::signatures;
using namespace kapa::tarracsh::attributes;
using namespace std;

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

void SignatureParser::parse(const ClassSignature &classSignature, ClassSignatureNode& node) const {

    const auto signatureString = _constantPool.getString(classSignature.signatureIndex);
    SignatureScanner scanner(signatureString);

    const auto &classRule = ParsingRules::getInstance().getClassRule();
    const bool matched = classRule->match(scanner, node);

    assert(matched);

}

void SignatureParser::parse(const MethodSignature &signature, MethodSignatureNode& node) const {
    const auto signatureString = _constantPool.getString(signature.signatureIndex);
    SignatureScanner scanner(signatureString);

    const auto &methodRule = ParsingRules::getInstance().getMethodRule();
    const bool matched = methodRule->match(scanner, node);
    assert(matched);

}

void SignatureParser::parse(const FieldSignature &signature, FieldTypeSignature& node) const {
    const auto signatureString = _constantPool.getString(signature.signatureIndex);
    SignatureScanner scanner(signatureString);
    const auto &fieldRule = ParsingRules::getInstance().getFieldRule();
    const bool matched = fieldRule->match(scanner, node);
    assert(matched);


}

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

std::string SignatureParser::getString(const ClassSignature & signature) const {
    const auto result = _constantPool.getString(signature.signatureIndex);
    return result;

}

std::string SignatureParser::getString(const MethodSignature &signature) const {
    const auto result = _constantPool.getString(signature.signatureIndex);
    return result;
}

std::string SignatureParser::getString(const FieldSignature &signature) const {
    const auto result = _constantPool.getString(signature.signatureIndex);
    return result;

}

string SignatureParser::toString(const ClassSignature &signature) const {
    string result; //TODO
    return result;
}

string SignatureParser::toString(const MethodSignature &signature) const {
    string result; // TODO
    return result;
}

string SignatureParser::toString(const FieldSignature &signature) const {
    string result; // TODO
    return result;
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
