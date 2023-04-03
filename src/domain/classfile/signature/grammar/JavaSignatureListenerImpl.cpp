#include <format>
#include "../../../../infrastructure/string/StringUtils.h"
#include "JavaSignatureListenerImpl.h"

using namespace kapa::tarracsh::domain::classfile::signature::parser;

using kapa::infrastructure::string::stringUtils::join;

std::unordered_map<char, std::string> JavaSignatureListenerImpl::_baseTypeToString = {
    {'B', "byte"},
    {'C', "char"},
    {'D', "double"},
    {'F', "float"},
    {'I', "int"},
    {'J', "long"},
    {'S', "short"},
    {'Z', "boolean"}
};

void JavaSignatureListenerImpl::visitTerminal(
    antlr4::tree::TerminalNode *node) {
}

void JavaSignatureListenerImpl::visitErrorNode(
    antlr4::tree::ErrorNode *node) {
}

void JavaSignatureListenerImpl::enterEveryRule(
    antlr4::ParserRuleContext *ctx) {
}

void JavaSignatureListenerImpl::exitEveryRule(
    antlr4::ParserRuleContext *ctx) {
}

void JavaSignatureListenerImpl::enterClassSignature(
    JvmSignatureParser::ClassSignatureContext *ctx) {
    _result = "";
    _ruleContextToJava.clear();
}

void JavaSignatureListenerImpl::exitClassSignature(
    JvmSignatureParser::ClassSignatureContext *ctx) {

    // _result = "";
    // if ( ctx->packageSpecifier() != nullptr) {
    //     _result = std::format("{} ", _ruleContextToJava[ctx->packageSpecifier()]);
    // }
    // _result += _ruleContextToJava[ctx->simpleClassTypeSignature()];
    //
    // if ( ctx->superclassSignature() != nullptr) {
    //     _result += std::format(" {}", _ruleContextToJava[ctx->superclassSignature()]);
    // }
    //
    // std::vector<std::string> parts;
    // for(const auto interfaceSignatureContext: ctx->interfaceSignature()) {
    //     parts.push_back(_ruleContextToJava[interfaceSignatureContext]);
    // }
    //
    // for (const auto fieldSignatureContext : ctx->fieldSignature()) {
    //     parts.push_back(_ruleContextToJava[fieldSignatureContext]);
    // }
    //
    // for (const auto methodSignatureContext : ctx->methodSignature()) {
    //     parts.push_back(_ruleContextToJava[methodSignatureContext]);
    // }
    //
    // _result += join(parts, std::string(" "));
}

void JavaSignatureListenerImpl::enterSimpleClassTypeSignature(
    JvmSignatureParser::SimpleClassTypeSignatureContext *ctx) {
}

void JavaSignatureListenerImpl::exitSimpleClassTypeSignature(
    JvmSignatureParser::SimpleClassTypeSignatureContext *ctx) {

    auto simpleClassTypeSignature = ctx->Identifier()->getText();
    if ( ctx->typeArguments() != nullptr ) {
        simpleClassTypeSignature += " " + _ruleContextToJava[ctx->typeArguments()];
    }
    _ruleContextToJava[ctx] = simpleClassTypeSignature;
}

void JavaSignatureListenerImpl::enterSuperclassSignature(
    JvmSignatureParser::SuperclassSignatureContext *ctx) {
}

void JavaSignatureListenerImpl::exitSuperclassSignature(
    JvmSignatureParser::SuperclassSignatureContext *ctx) {
    _ruleContextToJava[ctx] = _ruleContextToJava[ctx->classTypeSignature()];
}

void JavaSignatureListenerImpl::enterTypeArguments(
    JvmSignatureParser::TypeArgumentsContext *ctx) {
}

void JavaSignatureListenerImpl::exitTypeArguments(
    JvmSignatureParser::TypeArgumentsContext *ctx) {

    std::vector<std::string> typeArguments;
    for (const auto typeArgumentContext : ctx->typeArgument()) {
        typeArguments.push_back(_ruleContextToJava[typeArgumentContext]);
    }
    _ruleContextToJava[ctx] = join(typeArguments, std::string(", "));
}

void JavaSignatureListenerImpl::enterTypeArgument(
    JvmSignatureParser::TypeArgumentContext *ctx) {
}

// void JavaSignatureListenerImpl::exitTypeArgument(
//     JvmSignatureParser::TypeArgumentContext *ctx) {
//     std::string typeArgument;
//     if (ctx->wildcardIndicator() != nullptr) {
//         typeArgument = std::format("? {} ", _ruleContextToJava[ctx->wildcardIndicator()->wildcardBound()]);
//     }
//     typeArgument += _ruleContextToJava[ctx->typeSignature()];
//     _ruleContextToJava[ctx] = typeArgument;
// }


// void JavaSignatureListenerImpl::exitWildcardBound(
//     JvmSignatureParser::WildcardBoundContext *ctx) {
//     const auto extendsOrSuper = ctx->getText()[0] == 'e' ? "extends" : "super";
//     _ruleContextToJava[ctx] = std::format("{} {}", extendsOrSuper,
//                                           _ruleContextToJava[ctx->typeSignature()]);
//
// }
//
// void JavaSignatureListenerImpl::enterTypeSignature(
//     JvmSignatureParser::TypeSignatureContext *ctx) {
// }
//
// void JavaSignatureListenerImpl::exitTypeSignature(
//     JvmSignatureParser::TypeSignatureContext *ctx) {
//     if (ctx->arrayTypeSignature() != nullptr) {
//         _ruleContextToJava[ctx] = _ruleContextToJava[ctx->arrayTypeSignature()];
//     } else if (ctx->classTypeSignature() != nullptr) {
//         _ruleContextToJava[ctx] = _ruleContextToJava[ctx->classTypeSignature()];
//     } else {
//         _ruleContextToJava[ctx] = _ruleContextToJava[ctx->primitiveType()];
//     }
// }


// void JavaSignatureListenerImpl::exitClassTypeSignature(
//     JvmSignatureParser::ClassTypeSignatureContext *ctx) {
//
//     std::string classTypeSignature;
//
//     if (ctx->packageSpecifier() != nullptr) {
//         classTypeSignature += _ruleContextToJava[ctx->packageSpecifier()];
//     }
//     const auto identifiers = ctx->Identifier();
//     const auto typeArguments = ctx->typeArguments();
//
//     classTypeSignature += identifiers[0]->getText();
//
//     if (typeArguments[0] != nullptr) {
//         classTypeSignature += std::format("<{}>", _ruleContextToJava[typeArguments[0]]);
//     }
//
//     if (identifiers.size() > 1) {
//         auto index = 1u;
//         while (index < identifiers.size()) {
//             classTypeSignature += std::format(".{}", identifiers[index]->getText());
//
//             if (typeArguments[index] != nullptr) {
//                 classTypeSignature += std::format("<{}>", _ruleContextToJava[typeArguments[index]]);
//             }
//             ++index;
//         }
//
//     }
//
//     _ruleContextToJava[ctx] = classTypeSignature;
// }


void JavaSignatureListenerImpl::enterFieldSignature(
    JvmSignatureParser::FieldSignatureContext *ctx) {
    _result = "";
    _ruleContextToJava.clear();
}

// void JavaSignatureListenerImpl::exitFieldSignature(
//     JvmSignatureParser::FieldSignatureContext *ctx) {
//     _result = _ruleContextToJava[ctx->fieldTypeSignature()->typeSignature()];
// }

void JavaSignatureListenerImpl::enterMethodSignature(
    JvmSignatureParser::MethodSignatureContext *ctx) {
    _result = "";
    _ruleContextToJava.clear();
}

void JavaSignatureListenerImpl::exitMethodSignature(
    JvmSignatureParser::MethodSignatureContext *ctx) {
}

void JavaSignatureListenerImpl::enterPackageSpecifier(
    JvmSignatureParser::PackageSpecifierContext *ctx) {
}

void JavaSignatureListenerImpl::exitPackageSpecifier(
    JvmSignatureParser::PackageSpecifierContext *ctx) {
    _ruleContextToJava[ctx] = infrastructure::string::stringUtils::pathToClassname(ctx->getText());
}

void JavaSignatureListenerImpl::enterArrayTypeSignature(
    JvmSignatureParser::ArrayTypeSignatureContext *ctx) {
}

// void JavaSignatureListenerImpl::exitArrayTypeSignature(
//     JvmSignatureParser::ArrayTypeSignatureContext *ctx) {
//     _ruleContextToJava[ctx] = std::format("{}[]", _ruleContextToJava[ctx->typeSignature()]);
// }


// void JavaSignatureListenerImpl::exitMethodTypeSignature(
//     JvmSignatureParser::MethodTypeSignatureContext *ctx) {
//
//     const auto parameterTypeSignatures = ctx->parameterTypeSignature();
//     std::vector<std::string> parameters;
//     for (auto &parameter : parameterTypeSignatures) {
//         if (_ruleContextToJava.contains(parameter)) {
//             parameters.push_back(_ruleContextToJava[parameter]);
//         }
//     }
//
//     const auto throwSignatures = ctx->throwsSignature();
//     std::vector<std::string> throws;
//     for (auto &throwSignature : throwSignatures) {
//         if (_ruleContextToJava.contains(throwSignature)) {
//             throws.push_back(_ruleContextToJava[throwSignature]);
//         }
//     }
//
//     _result = std::format("({}) {} {}",
//                           join(parameters, std::string(", ")),
//                           _ruleContextToJava[ctx->returnType()],
//                           join(throws, std::string(", ")));
//
// }


// void JavaSignatureListenerImpl::exitReturnType(
//     JvmSignatureParser::ReturnTypeContext *ctx) {
//
//     if (ctx->typeSignature() == nullptr) {
//         _ruleContextToJava[ctx] = "void";
//     } else {
//         _ruleContextToJava[ctx] = _ruleContextToJava[ctx->typeSignature()];
//     }
//
// }

void JavaSignatureListenerImpl::enterThrowsSignature(
    JvmSignatureParser::ThrowsSignatureContext *ctx) {
}

void JavaSignatureListenerImpl::exitThrowsSignature(
    JvmSignatureParser::ThrowsSignatureContext *ctx) {
}



JavaSignatureListenerImpl::~JavaSignatureListenerImpl() = default;

std::string JavaSignatureListenerImpl::getResult() {
    return _result;
}

void JavaSignatureListenerImpl::enterClassTypeSignature(JvmSignatureParser::ClassTypeSignatureContext *ctx) {
}

void JavaSignatureListenerImpl::exitClassTypeSignature(JvmSignatureParser::ClassTypeSignatureContext *ctx) {
}

void JavaSignatureListenerImpl::exitTypeArgument(JvmSignatureParser::TypeArgumentContext *ctx) {
}

void JavaSignatureListenerImpl::enterWildcardIndicator(JvmSignatureParser::WildcardIndicatorContext *ctx) {
}

void JavaSignatureListenerImpl::exitWildcardIndicator(JvmSignatureParser::WildcardIndicatorContext *ctx) {
}

void JavaSignatureListenerImpl::exitArrayTypeSignature(JvmSignatureParser::ArrayTypeSignatureContext *ctx) {
}

void JavaSignatureListenerImpl::exitFieldSignature(JvmSignatureParser::FieldSignatureContext *ctx) {
}

void JavaSignatureListenerImpl::enterJavaTypeSignature(JvmSignatureParser::JavaTypeSignatureContext *ctx) {
}

void JavaSignatureListenerImpl::exitJavaTypeSignature(JvmSignatureParser::JavaTypeSignatureContext *ctx) {
}

void JavaSignatureListenerImpl::enterBaseType(JvmSignatureParser::BaseTypeContext *ctx) {
}

void JavaSignatureListenerImpl::exitBaseType(JvmSignatureParser::BaseTypeContext *ctx) {
}

void JavaSignatureListenerImpl::enterReferenceTypeSignature(JvmSignatureParser::ReferenceTypeSignatureContext *ctx) {
}

void JavaSignatureListenerImpl::exitReferenceTypeSignature(JvmSignatureParser::ReferenceTypeSignatureContext *ctx) {
}

void JavaSignatureListenerImpl::
enterClassTypeSignatureSuffix(JvmSignatureParser::ClassTypeSignatureSuffixContext *ctx) {
}

void JavaSignatureListenerImpl::exitClassTypeSignatureSuffix(JvmSignatureParser::ClassTypeSignatureSuffixContext *ctx) {
}

void JavaSignatureListenerImpl::enterTypeVariableSignature(JvmSignatureParser::TypeVariableSignatureContext *ctx) {
}

void JavaSignatureListenerImpl::exitTypeVariableSignature(JvmSignatureParser::TypeVariableSignatureContext *ctx) {
}

void JavaSignatureListenerImpl::enterTypeParameters(JvmSignatureParser::TypeParametersContext *ctx) {
}

void JavaSignatureListenerImpl::exitTypeParameters(JvmSignatureParser::TypeParametersContext *ctx) {
}

void JavaSignatureListenerImpl::enterTypeParameter(JvmSignatureParser::TypeParameterContext *ctx) {
}

void JavaSignatureListenerImpl::exitTypeParameter(JvmSignatureParser::TypeParameterContext *ctx) {
}

void JavaSignatureListenerImpl::enterClassBound(JvmSignatureParser::ClassBoundContext *ctx) {
}

void JavaSignatureListenerImpl::exitClassBound(JvmSignatureParser::ClassBoundContext *ctx) {
}

void JavaSignatureListenerImpl::enterSuperInterfaceSignature(JvmSignatureParser::SuperInterfaceSignatureContext *ctx) {
}

void JavaSignatureListenerImpl::exitSuperInterfaceSignature(JvmSignatureParser::SuperInterfaceSignatureContext *ctx) {
}

void JavaSignatureListenerImpl::enterInterfaceBound(JvmSignatureParser::InterfaceBoundContext *ctx) {
}

void JavaSignatureListenerImpl::exitInterfaceBound(JvmSignatureParser::InterfaceBoundContext *ctx) {
}

void JavaSignatureListenerImpl::enterResult(JvmSignatureParser::ResultContext *ctx) {
}

void JavaSignatureListenerImpl::exitResult(JvmSignatureParser::ResultContext *ctx) {
}

void JavaSignatureListenerImpl::enterVoidDescriptor(JvmSignatureParser::VoidDescriptorContext *ctx) {
}

void JavaSignatureListenerImpl::exitVoidDescriptor(JvmSignatureParser::VoidDescriptorContext *ctx) {
}
