#include "SignatureParser.h"

#include "grammar/JvmSignatureLexer.h"

using namespace kapa::tarracsh::domain::classfile::signature;
using namespace kapa::tarracsh::domain::classfile::attribute;
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

std::string SignatureParser::getString(const ClassSignature &signature) const {
    auto result = getStringInternal(signature, [](parser::JvmSignatureParser &parser)-> antlr4::tree::ParseTree* {
        const auto parseTree = parser.classSignature();
        return parseTree;
    });

    return result;

}

std::string SignatureParser::getString(const MethodSignature &signature) const {
    auto result = getStringInternal(signature, [](parser::JvmSignatureParser &parser)-> antlr4::tree::ParseTree* {
        const auto parseTree = parser.fieldSignature();
        return parseTree;
    });

    return result;

}

std::string SignatureParser::getString(const FieldSignature &signature) const {

    auto result = getStringInternal(signature, [](parser::JvmSignatureParser &parser)-> antlr4::tree::ParseTree* {
        const auto parseTree = parser.methodSignature();
        return parseTree;
    });

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

std::string SignatureParser::getStringInternal(const SignatureBase &signature,
                                               std::function<antlr4::tree::ParseTree*(parser::JvmSignatureParser &)>
                                               parserFunc) const {
    // const auto signatureString = _constantPool.getString(signature.signatureIndex);
    //
    // std::string result;
    // antlr4::ANTLRInputStream input(signatureString.c_str());
    //
    // parser::JvmSignatureLexer lexer(&input);
    // antlr4::CommonTokenStream tokens(&lexer);
    // parser::JvmSignatureParser parser(&tokens);
    //
    // auto parseTree = parserFunc(parser);
    // auto parsed = parser.getNumberOfSyntaxErrors() == 0;
    //
    // if (parsed) {
    //     parser::JavaSignatureListenerImpl listener;
    //     antlr4::tree::ParseTreeWalker::DEFAULT.walk(&listener, parseTree);
    //     result = listener.getResult();
    // } else {
    //     std::cout << std::format("Invalid java signature: {}", signatureString) << std::endl;
    // }

    std::string result;
    //TODO
    return result;
}
