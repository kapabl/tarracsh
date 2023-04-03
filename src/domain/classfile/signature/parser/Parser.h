#ifndef KAPA_SIGNATURE_PARSER_H
#define KAPA_SIGNATURE_PARSER_H
/**
 * This is an adaptation of the class found in the JVM to parse signatures
 * ~kapa April-2-2023
 */
#include <cassert>
#include <format>
#include <stdexcept>
#include <string>
#include <unordered_set>
#include "../nodes/Nodes.h"

namespace kapa::tarracsh::domain::classfile::signature::parser {

class Parser {

private:
    std::string _input;
    uint64_t _position = 0;

    static char endOfInput() {
        static char eoiValue = ':';
        return eoiValue;
    }

    std::vector<std::string> _errors;

    char getNext();
    [[nodiscard]] char current() const;
    [[nodiscard]] bool isEof() const;
    void advance();
    // static auto matches(char c, const std::unordered_set<char> &chars) -> bool;
    void progress(uint64_t startingPosition);


public:
    Parser();
    static std::shared_ptr<Parser> make();
    std::shared_ptr<ClassSignature> parseClassSig(const std::string &signature);
    std::shared_ptr<MethodTypeSignature> parseMethodSig(const std::string &signature);
    std::shared_ptr<TypeSignature> parseTypeSig(const std::string &signature);
    std::shared_ptr<ClassSignature> parseClassSignature();
    std::vector<std::shared_ptr<FormalTypeParameter>> parseZeroOrMoreFormalTypeParameters();
    std::vector<std::shared_ptr<FormalTypeParameter>> parseFormalTypeParameters();
    std::shared_ptr<FormalTypeParameter> parseFormalTypeParameter();
    std::string parseIdentifier();
    std::shared_ptr<FieldTypeSignature> parseFieldTypeSignature();
    std::shared_ptr<FieldTypeSignature> parseFieldTypeSignature(bool allowArrays);
    std::shared_ptr<ClassTypeSignature> parseClassTypeSignature();
    std::shared_ptr<SimpleClassTypeSignature> parsePackageNameAndSimpleClassTypeSignature();
    std::shared_ptr<SimpleClassTypeSignature> parseSimpleClassTypeSignature(bool dollar);
    void parseClassTypeSignatureSuffix(std::vector<std::shared_ptr<SimpleClassTypeSignature>> simpleClassTypeSignatures);
    std::vector<std::shared_ptr<TypeArgument>> parseTypeArgumentsOpt();
    std::vector<std::shared_ptr<TypeArgument>> parseTypeArguments();
    std::shared_ptr<TypeArgument> parseTypeArgument();
    std::shared_ptr<TypeVariableSignature> parseTypeVariableSignature();
    std::shared_ptr<ArrayTypeSignature> parseArrayTypeSignature();
    std::shared_ptr<TypeSignature> parseTypeSignature();
    std::shared_ptr<BaseType> parseBaseType();
    std::vector<std::shared_ptr<FieldTypeSignature>>
    parseBounds();
    std::vector<std::shared_ptr<ClassTypeSignature>> parseSuperInterfaces();
    std::shared_ptr<MethodTypeSignature> parseMethodTypeSignature();
    std::vector<std::shared_ptr<TypeSignature>> parseFormalParameters();
    std::vector<std::shared_ptr<TypeSignature>> parseZeroOrMoreTypeSignatures();
    std::shared_ptr<ReturnType> parseReturnType();
    std::vector<std::shared_ptr<FieldTypeSignature>> parseZeroOrMoreThrowsSignatures();
    std::shared_ptr<FieldTypeSignature> parseThrowsSignature();
    void reset();
};


}
#endif
