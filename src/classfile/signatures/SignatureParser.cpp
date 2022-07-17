#include "rules/Rule.h"
#include "rules/Kleene.h"
#include "rules/Or.h"
#include "rules/Terminal.h"
#include "rules/Optional.h"
#include "rules/JvmIdentifier.h"
#include "rules/RuleFuncs.h"

#include "SignatureParser.h"

#include "rules/ParsingRules.h"

using namespace org::kapa::tarrash::signatures;
using namespace org::kapa::tarrash::attributes;
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

ClassSignatureNode SignatureParser::readSignature(ClassSignature &classSignature) const {

    ClassSignatureNode result;
    classSignature.nameIndex = _attribute.nameIndex;
    classSignature.length = _attribute.length;
    classSignature.signatureIndex = _reader.readU2();
    const auto signatureString = _constantPool.getString(classSignature.signatureIndex);
    SignatureScanner scanner(signatureString);

    auto& classRule = ParsingRules::getInstance().getClassRule();
    classRule.match(scanner, result);


    return result;

}

void SignatureParser::readSignature(MethodSignature &signature) const {
    signature.nameIndex = _attribute.nameIndex;
    signature.length = _attribute.length;
    signature.signatureIndex = _reader.readU2();
    const auto signatureString = _constantPool.getString(signature.signatureIndex);
    SignatureScanner scanner(signatureString);

    auto& methodRule = ParsingRules::getInstance().getMethodRule();

    MethodSignatureNode result;
    methodRule.match(scanner, result);
}

void SignatureParser::readSignature(FieldSignature &signature) const {
    signature.nameIndex = _attribute.nameIndex;
    signature.length = _attribute.length;
    signature.signatureIndex = _reader.readU2();
    const auto signatureString = _constantPool.getString(signature.signatureIndex);
    SignatureScanner scanner(signatureString);

    auto& fieldRule = ParsingRules::getInstance().getFieldRule();
    // fieldRule->match(scanner);
    //TODO get structure
}

wstring SignatureParser::toString(const ClassSignature &signature) const {
    wstring result; //TODO
    return result;
}

wstring SignatureParser::toString(const MethodSignature &signature) const {
    wstring result; // TODO
    return result;
}

wstring SignatureParser::toString(const FieldSignature &signature) const {
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
            MethodSignature signature;
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
