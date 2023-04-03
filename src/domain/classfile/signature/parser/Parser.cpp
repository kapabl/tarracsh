#include "Parser.h"


using namespace std;
using namespace kapa::tarracsh::domain::classfile::signature::parser;

char Parser::getNext() {
    try {
        return _input[_position++];
    } catch (...) { return endOfInput(); }
}

char Parser::current() const {
    try {
        return _input[_position];
    } catch (...) { return endOfInput(); }
}

bool Parser::isEof() const {
    return _position > _input.length();
}

void Parser::advance() {
    _position++;
}

//
// auto Parser::matches(char c,
//     const unordered_set<char> &chars) -> bool {
//     return chars.contains(c);
// }

void Parser::progress(const uint64_t startingPosition) {
    if (_position <= startingPosition)
        _errors.emplace_back("Failure to make progress!");
}

Parser::Parser() = default;

unique_ptr<Parser> Parser::make() {
    return make_unique<Parser>();
}

std::shared_ptr<ClassSignature> Parser::parseClassSig(const string &signature) {
    _input = signature;
    return parseClassSignature();
}

std::shared_ptr<MethodTypeSignature> Parser::parseMethodSig(const string &signature) {
    _input = signature;
    return parseMethodTypeSignature();
}

std::shared_ptr<TypeSignature> Parser::parseTypeSig(const string &signature) {
    _input = signature;
    return parseTypeSignature();
}

std::shared_ptr<ClassSignature> Parser::parseClassSignature() {
    // parse a class signature based on the implicit input.
    return ClassSignature::make(parseZeroOrMoreFormalTypeParameters(),
                                parseClassTypeSignature(), // Only rule for SuperclassSignature
                                parseSuperInterfaces());
}

vector<shared_ptr<FormalTypeParameter>> Parser::parseZeroOrMoreFormalTypeParameters() {
    if (current() == '<') {
        return parseFormalTypeParameters();
    } else {
        return {};
    }
}

vector<shared_ptr<FormalTypeParameter>> Parser::parseFormalTypeParameters() {
    vector<shared_ptr<FormalTypeParameter>> ftps;
    if (current() != '<') { throw runtime_error("expected '<'"); }
    advance();
    ftps.emplace_back(parseFormalTypeParameter());
    while (current() != '>') {
        const auto startingPosition = _position;
        ftps.emplace_back(parseFormalTypeParameter());
        progress(startingPosition);
    }
    advance();
    return ftps;
}

shared_ptr<FormalTypeParameter> Parser::parseFormalTypeParameter() {
    const auto id = parseIdentifier();
    const auto bs = parseBounds();
    return FormalTypeParameter::make(id, bs);
}

string Parser::parseIdentifier() {
    string result;
    while (!isEof()) {
        const char c = current();
        switch (c) {
            case ';':
            case '.':
            case '/':
            case '[':
            case ':':
            case '>':
            case '<':
                return result;
            default: {
                result.push_back(c);
                advance();
            }

        }
    }
    return result;
}

shared_ptr<FieldTypeSignature> Parser::parseFieldTypeSignature() {
    return parseFieldTypeSignature(true);
}

shared_ptr<FieldTypeSignature> Parser::parseFieldTypeSignature(bool allowArrays) {
    const auto currentChar = current();
    switch (currentChar) {
        case 'L':
            return parseClassTypeSignature();
        case 'T':
            return parseTypeVariableSignature();
        case '[':
            if (allowArrays)
                return parseArrayTypeSignature();
            throw std::runtime_error("Array signature not allowed here.");
        default:
            throw std::runtime_error("Expected Field Type Signature");
    }

}

shared_ptr<SimpleClassTypeSignature> Parser::parsePackageNameAndSimpleClassTypeSignature() {
    // Parse both any optional leading PackageSpecifier as well as
    // the following SimpleClassTypeSignature.

    string id = parseIdentifier();

    if (current() == '/') {
        // package name
        string idBuild = id;

        while (current() == '/') {
            advance();
            idBuild.push_back('.');
            idBuild += parseIdentifier();
        }
        id = idBuild;
    }

    switch (current()) {
        case ';':
            return SimpleClassTypeSignature::make(id, false, {}); // all done!
        case '<':
            return SimpleClassTypeSignature::make(id, false, parseTypeArguments());
        default:
            throw runtime_error(format("expected '<' or ';' but got {}", current()));
    }
}

shared_ptr<SimpleClassTypeSignature> Parser::parseSimpleClassTypeSignature(const bool dollar) {
    const string id = parseIdentifier();
    char c = current();

    switch (c) {
        case ';':
        case '.':
            return SimpleClassTypeSignature::make(id, dollar, {});
        case '<':
            return SimpleClassTypeSignature::make(id, dollar, parseTypeArguments());
        default:
            throw runtime_error(format("expected '<' or ';' or '.', got '{}'.", c));
    }
}

void Parser::parseClassTypeSignatureSuffix(vector<shared_ptr<SimpleClassTypeSignature>> simpleClassTypeSignatures) {
    while (current() == '.') {
        advance();
        simpleClassTypeSignatures.emplace_back(parseSimpleClassTypeSignature(true));
    }
}

vector<shared_ptr<TypeArgument>> Parser::parseTypeArgumentsOpt() {
    if (current() == '<') { return parseTypeArguments(); } else { return {}; }
}

vector<shared_ptr<TypeArgument>> Parser::parseTypeArguments() {
    vector<shared_ptr<TypeArgument>> tas;
    if (current() != '<') { throw runtime_error("expected '<'"); }
    advance();
    tas.emplace_back(parseTypeArgument());
    while (current() != '>') {
        //(matches(current(),  '+', '-', 'L', '[', 'T', '*')) {
        tas.emplace_back(parseTypeArgument());
    }
    advance();
    return tas;
}

shared_ptr<TypeArgument> Parser::parseTypeArgument() {
    vector<shared_ptr<FieldTypeSignature>> ub(1), lb(1);
    const vector<shared_ptr<TypeArgument>> typeArguments;
    const char c = current();
    switch (c) {
        case '+': {
            advance();
            ub[0] = parseFieldTypeSignature();
            lb[0] = BottomSignature::make(); // bottom
            return Wildcard::make(ub, lb);
        }
        case '*': {
            advance();
            ub[0] = SimpleClassTypeSignature::make("java.lang.Object", false, typeArguments);
            lb[0] = BottomSignature::make(); // bottom
            return Wildcard::make(ub, lb);
        }
        case '-': {
            advance();
            lb[0] = parseFieldTypeSignature();
            ub[0] = SimpleClassTypeSignature::make("java.lang.Object", false, typeArguments);
            return Wildcard::make(ub, lb);
        }
        default:
            return parseFieldTypeSignature();
    }
}

std::shared_ptr<TypeVariableSignature> Parser::parseTypeVariableSignature() {
    if (current() != 'T') { throw runtime_error("expected a type variable usage"); }
    advance();
    auto ts = TypeVariableSignature::make(parseIdentifier());
    if (current() != ';') {
        throw runtime_error(format("; expected in signature of type variable named {}", ts->getIdentifier()));
    }
    advance();
    return ts;
}

std::shared_ptr<ArrayTypeSignature> Parser::parseArrayTypeSignature() {
    if (current() != '[') { throw runtime_error("expected array type signature"); }
    advance();
    return ArrayTypeSignature::make(parseTypeSignature());
}

std::shared_ptr<TypeSignature> Parser::parseTypeSignature() {
    switch (current()) {
        case 'B':
        case 'C':
        case 'D':
        case 'F':
        case 'I':
        case 'J':
        case 'S':
        case 'Z':
            return parseBaseType();

        default:
            return parseFieldTypeSignature();
    }
}

std::shared_ptr<BaseType> Parser::parseBaseType() {
    switch (current()) {
        case 'B':
            advance();
            return ByteSignature::make();
        case 'C':
            advance();
            return CharSignature::make();
        case 'D':
            advance();
            return DoubleSignature::make();
        case 'F':
            advance();
            return FloatSignature::make();
        case 'I':
            advance();
            return IntSignature::make();
        case 'J':
            advance();
            return LongSignature::make();
        case 'S':
            advance();
            return ShortSignature::make();
        case 'Z':
            advance();
            return BooleanSignature::make();
        default: {
            throw runtime_error("expected primitive type");
        }
    }
}

vector<shared_ptr<FieldTypeSignature>> Parser::parseBounds() {
    vector<shared_ptr<FieldTypeSignature>> fts;

    if (current() == ':') {
        advance();
        switch (current()) {
            case ':': // empty class bound
                break;

            default: // parse class bound
                fts.push_back(parseFieldTypeSignature());
        }

        // zero or more interface bounds
        while (current() == ':') {
            advance();
            fts.push_back(parseFieldTypeSignature());
        }
    } else
        _errors.emplace_back("Bound expected");

    return fts;
}

vector<shared_ptr<ClassTypeSignature>> Parser::parseSuperInterfaces() {
    vector<shared_ptr<ClassTypeSignature>> cts;
    while (current() == 'L') {
        cts.push_back(parseClassTypeSignature());
    }
    return cts;
}

shared_ptr<MethodTypeSignature> Parser::parseMethodTypeSignature() {

    assert(_position == 0);
    const auto formalTypeParameters = parseZeroOrMoreFormalTypeParameters();
    const auto formalParameters = parseFormalParameters();
    const auto returnType = parseReturnType();
    const auto fieldTypeSignatures = parseZeroOrMoreThrowsSignatures();
    return MethodTypeSignature::make(formalTypeParameters,
                                     formalParameters,
                                     returnType,
                                     fieldTypeSignatures);
}

vector<shared_ptr<TypeSignature>> Parser::parseFormalParameters() {
    if (current() != '(') { throw runtime_error("expected '('"); }
    advance();
    auto pts = parseZeroOrMoreTypeSignatures();
    if (current() != ')') { throw runtime_error("expected ')'"); }
    advance();
    return pts;
}

vector<shared_ptr<TypeSignature>> Parser::parseZeroOrMoreTypeSignatures() {
    vector<shared_ptr<TypeSignature>> ts;
    bool stop = false;
    while (!stop) {
        switch (current()) {
            case 'B':
            case 'C':
            case 'D':
            case 'F':
            case 'I':
            case 'J':
            case 'S':
            case 'Z':
            case 'L':
            case 'T':
            case '[': {
                ts.push_back(parseTypeSignature());
                break;
            }
            default:
                stop = true;
        }
    }
    return ts;
}

shared_ptr<ReturnType> Parser::parseReturnType() {
    if (current() == 'V') {
        advance();
        return VoidDescriptor::make();
    } else
        return parseTypeSignature();
}

vector<shared_ptr<FieldTypeSignature>> Parser::parseZeroOrMoreThrowsSignatures() {
    vector<shared_ptr<FieldTypeSignature>> ets(3);
    while (current() == '^') {
        ets.push_back(parseThrowsSignature());
    }
    return ets;
}

shared_ptr<FieldTypeSignature> Parser::parseThrowsSignature() {
    if (current() != '^') { throw runtime_error("expected throws signature"); }
    advance();
    return parseFieldTypeSignature(false);
}

void Parser::reset() {
    _position = 0;
    _input = "";
}

shared_ptr<ClassTypeSignature> Parser::parseClassTypeSignature() {
    if (current() != 'L') { throw runtime_error("expected a class type"); }
    advance();
    vector<shared_ptr<SimpleClassTypeSignature>> simpleClassTypeSignatures;
    simpleClassTypeSignatures.push_back(parsePackageNameAndSimpleClassTypeSignature());

    parseClassTypeSignatureSuffix(simpleClassTypeSignatures);
    if (current() != ';')
        throw runtime_error(format("expected ';' got '{}'", current()));

    advance();
    return ClassTypeSignature::make(simpleClassTypeSignatures);
}
