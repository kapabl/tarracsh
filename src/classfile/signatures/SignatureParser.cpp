#include "SignatureParser.h"

#include <iostream>

#include "rules/Rule.h"
#include "rules/Kleene.h"
#include "rules/Optional.h"
#include "rules/Terminal.h"

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

void SignatureParser::readSignature(ClassSignature &classSignature) const {
    classSignature.nameIndex = _attribute.nameIndex;
    classSignature.length = _attribute.length;
    classSignature.signatureIndex = _reader.readU2();
    const auto signatureString = _constantPool.getString(classSignature.signatureIndex);
    SignatureScanner scanner(signatureString);

    //auto classSignatureRule = make_shared<Rule>(scanner);
    auto formalTypeParameter = make_shared<Rule>(scanner);
    auto superclassSignature = make_shared<Rule>(scanner);
    auto superinterfaceSignature = make_shared<Rule>(scanner);

    //TODO identifier is a special rule like a lexeme/regex
    //we can do it manually
    auto identifier = make_shared<Rule>(scanner);
    auto classBound = make_shared<Rule>(scanner);
    auto interfaceBound = make_shared<Rule>(scanner);
    auto fieldTypeSignature = make_shared<Rule>(scanner);

    auto classTypeSignature = make_shared<Rule>(scanner);
    auto arrayTypeSignature = make_shared<Rule>(scanner);
    auto typeVariableSignature = make_shared<Rule>(scanner);

    auto packageSpecifier = make_shared<Rule>(scanner);
    auto simpleClassTypeSignature = make_shared<Rule>(scanner);
    auto classTypeSignatureSuffix = make_shared<Rule>(scanner);
    auto typeArgument = make_shared<Rule>(scanner);
    auto typeArguments = make_shared<Rule>(scanner);
    auto wildcardIndicator = make_shared<Rule>(scanner);
    auto typeSignature = make_shared<Rule>(scanner);
    auto baseType = make_shared<Rule>(scanner);

    auto plusTerminal = make_shared<Terminal>(L"+",scanner);

    
    auto classSignatureRule = 
            'L'
         >> !(+formalTypeParameter)
         >> superclassSignature
         >> *superinterfaceSignature;

    formalTypeParameter >> identifier >> classBound >> *interfaceBound;

    classBound >> ':' >> !fieldTypeSignature;
    interfaceBound >> ':' >> fieldTypeSignature;

    superclassSignature = classTypeSignature;
    superinterfaceSignature >> classTypeSignature;

    fieldTypeSignature >> classTypeSignature | arrayTypeSignature | typeVariableSignature;

    classTypeSignature >> 'L' >> !packageSpecifier >> simpleClassTypeSignature >> *classTypeSignatureSuffix;

    packageSpecifier >> identifier >> '/' >> *packageSpecifier;

    simpleClassTypeSignature >> identifier >> !typeArguments;

    typeArguments >> +typeArgument;

    wildcardIndicator >> plusTerminal | '-';

    typeArgument >> (wildcardIndicator >> fieldTypeSignature) | '*';

    classTypeSignatureSuffix >> '.' >> simpleClassTypeSignature;

    typeVariableSignature >> 'T' >> identifier;

    arrayTypeSignature >> '[' >> typeSignature;

    typeSignature >> fieldTypeSignature | baseType;

    //TODO make all the rules

    classSignatureRule->parse();

    //wcout << signatureString;

}

void SignatureParser::readSignature(MethodSignature &signature) const {
    //TODO
}

void SignatureParser::readSignature(FieldSignature &signature) const {
    //TODO
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
