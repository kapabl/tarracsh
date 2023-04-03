#ifndef JAVA_SIGNATURE_LISTENER_IMPL
#define JAVA_SIGNATURE_LISTENER_IMPL

#include "JvmSignatureListener.h"

namespace kapa::tarracsh::domain::classfile::signature::parser {
class JavaSignatureListenerImpl : public JvmSignatureListener {
public:
    void visitTerminal(antlr4::tree::TerminalNode *node) override;
    void visitErrorNode(antlr4::tree::ErrorNode *node) override;
    void enterEveryRule(antlr4::ParserRuleContext *ctx) override;
    void exitEveryRule(antlr4::ParserRuleContext *ctx) override;
    void enterClassSignature(JvmSignatureParser::ClassSignatureContext *ctx) override;
    void exitClassSignature(JvmSignatureParser::ClassSignatureContext *ctx) override;
    void enterSimpleClassTypeSignature(JvmSignatureParser::SimpleClassTypeSignatureContext *ctx) override;
    void exitSimpleClassTypeSignature(JvmSignatureParser::SimpleClassTypeSignatureContext *ctx) override;
    void enterSuperclassSignature(JvmSignatureParser::SuperclassSignatureContext *ctx) override;
    void exitSuperclassSignature(JvmSignatureParser::SuperclassSignatureContext *ctx) override;
    void enterTypeArguments(JvmSignatureParser::TypeArgumentsContext *ctx) override;
    void exitTypeArguments(JvmSignatureParser::TypeArgumentsContext *ctx) override;
    void enterTypeArgument(JvmSignatureParser::TypeArgumentContext *ctx) override;
    void enterFieldSignature(JvmSignatureParser::FieldSignatureContext *ctx) override;
    void enterMethodSignature(JvmSignatureParser::MethodSignatureContext *ctx) override;
    void exitMethodSignature(JvmSignatureParser::MethodSignatureContext *ctx) override;
    void enterPackageSpecifier(JvmSignatureParser::PackageSpecifierContext *ctx) override;
    void exitPackageSpecifier(JvmSignatureParser::PackageSpecifierContext *ctx) override;
    void enterArrayTypeSignature(JvmSignatureParser::ArrayTypeSignatureContext *ctx) override;
    void enterThrowsSignature(JvmSignatureParser::ThrowsSignatureContext *ctx) override;
    void exitThrowsSignature(JvmSignatureParser::ThrowsSignatureContext *ctx) override;
    void enterJavaTypeSignature(JvmSignatureParser::JavaTypeSignatureContext *ctx) override;
    void exitJavaTypeSignature(JvmSignatureParser::JavaTypeSignatureContext *ctx) override;
    void enterBaseType(JvmSignatureParser::BaseTypeContext *ctx) override;
    void exitBaseType(JvmSignatureParser::BaseTypeContext *ctx) override;
    void enterReferenceTypeSignature(JvmSignatureParser::ReferenceTypeSignatureContext *ctx) override;
    void exitReferenceTypeSignature(JvmSignatureParser::ReferenceTypeSignatureContext *ctx) override;
    void enterClassTypeSignatureSuffix(JvmSignatureParser::ClassTypeSignatureSuffixContext *ctx) override;
    void exitClassTypeSignatureSuffix(JvmSignatureParser::ClassTypeSignatureSuffixContext *ctx) override;
    void enterTypeVariableSignature(JvmSignatureParser::TypeVariableSignatureContext *ctx) override;
    void exitTypeVariableSignature(JvmSignatureParser::TypeVariableSignatureContext *ctx) override;
    void enterTypeParameters(JvmSignatureParser::TypeParametersContext *ctx) override;
    void exitTypeParameters(JvmSignatureParser::TypeParametersContext *ctx) override;
    void enterTypeParameter(JvmSignatureParser::TypeParameterContext *ctx) override;
    void exitTypeParameter(JvmSignatureParser::TypeParameterContext *ctx) override;
    void enterClassBound(JvmSignatureParser::ClassBoundContext *ctx) override;
    void exitClassBound(JvmSignatureParser::ClassBoundContext *ctx) override;
    void enterSuperInterfaceSignature(JvmSignatureParser::SuperInterfaceSignatureContext *ctx) override;
    void exitSuperInterfaceSignature(JvmSignatureParser::SuperInterfaceSignatureContext *ctx) override;
    void enterInterfaceBound(JvmSignatureParser::InterfaceBoundContext *ctx) override;
    void exitInterfaceBound(JvmSignatureParser::InterfaceBoundContext *ctx) override;
    void enterResult(JvmSignatureParser::ResultContext *ctx) override;
    void exitResult(JvmSignatureParser::ResultContext *ctx) override;
    void enterVoidDescriptor(JvmSignatureParser::VoidDescriptorContext *ctx) override;
    void exitVoidDescriptor(JvmSignatureParser::VoidDescriptorContext *ctx) override;

    ~JavaSignatureListenerImpl() override;
    
    [[nodiscard]] std::string getResult();
    void enterClassTypeSignature(JvmSignatureParser::ClassTypeSignatureContext *ctx) override;
    void exitClassTypeSignature(JvmSignatureParser::ClassTypeSignatureContext *ctx) override;
    void exitTypeArgument(JvmSignatureParser::TypeArgumentContext *ctx) override;
    void enterWildcardIndicator(JvmSignatureParser::WildcardIndicatorContext *ctx) override;
    void exitWildcardIndicator(JvmSignatureParser::WildcardIndicatorContext *ctx) override;
    void exitArrayTypeSignature(JvmSignatureParser::ArrayTypeSignatureContext *ctx) override;
    void exitFieldSignature(JvmSignatureParser::FieldSignatureContext *ctx) override;

private:
    std::string _result;
    std::unordered_map<antlr4::ParserRuleContext*, std::string> _ruleContextToJava;
    static std::unordered_map<char, std::string> _baseTypeToString;

};

}
#endif
