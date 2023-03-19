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
    void enterInterfaceSignature(JvmSignatureParser::InterfaceSignatureContext *ctx) override;
    void exitInterfaceSignature(JvmSignatureParser::InterfaceSignatureContext *ctx) override;
    void enterTypeArguments(JvmSignatureParser::TypeArgumentsContext *ctx) override;
    void exitTypeArguments(JvmSignatureParser::TypeArgumentsContext *ctx) override;
    void enterTypeArgument(JvmSignatureParser::TypeArgumentContext *ctx) override;
    void exitTypeArgument(JvmSignatureParser::TypeArgumentContext *ctx) override;
    void enterWildcardIndicator(JvmSignatureParser::WildcardIndicatorContext *ctx) override;
    void exitWildcardIndicator(JvmSignatureParser::WildcardIndicatorContext *ctx) override;
    void enterWildcardBound(JvmSignatureParser::WildcardBoundContext *ctx) override;
    void exitWildcardBound(JvmSignatureParser::WildcardBoundContext *ctx) override;
    void enterTypeSignature(JvmSignatureParser::TypeSignatureContext *ctx) override;
    void exitTypeSignature(JvmSignatureParser::TypeSignatureContext *ctx) override;
    void enterClassTypeSignature(JvmSignatureParser::ClassTypeSignatureContext *ctx) override;
    void exitClassTypeSignature(JvmSignatureParser::ClassTypeSignatureContext *ctx) override;
    void enterFieldSignature(JvmSignatureParser::FieldSignatureContext *ctx) override;
    void exitFieldSignature(JvmSignatureParser::FieldSignatureContext *ctx) override;
    void enterMethodSignature(JvmSignatureParser::MethodSignatureContext *ctx) override;
    void exitMethodSignature(JvmSignatureParser::MethodSignatureContext *ctx) override;
    void enterPackageSpecifier(JvmSignatureParser::PackageSpecifierContext *ctx) override;
    void exitPackageSpecifier(JvmSignatureParser::PackageSpecifierContext *ctx) override;
    void enterArrayTypeSignature(JvmSignatureParser::ArrayTypeSignatureContext *ctx) override;
    void exitArrayTypeSignature(JvmSignatureParser::ArrayTypeSignatureContext *ctx) override;
    void enterFieldTypeSignature(JvmSignatureParser::FieldTypeSignatureContext *ctx) override;
    void exitFieldTypeSignature(JvmSignatureParser::FieldTypeSignatureContext *ctx) override;
    void enterMethodTypeSignature(JvmSignatureParser::MethodTypeSignatureContext *ctx) override;
    void exitMethodTypeSignature(JvmSignatureParser::MethodTypeSignatureContext *ctx) override;
    void enterParameterTypeSignature(JvmSignatureParser::ParameterTypeSignatureContext *ctx) override;
    void exitParameterTypeSignature(JvmSignatureParser::ParameterTypeSignatureContext *ctx) override;
    void enterReturnType(JvmSignatureParser::ReturnTypeContext *ctx) override;
    void exitReturnType(JvmSignatureParser::ReturnTypeContext *ctx) override;
    void enterThrowsSignature(JvmSignatureParser::ThrowsSignatureContext *ctx) override;
    void exitThrowsSignature(JvmSignatureParser::ThrowsSignatureContext *ctx) override;
    void enterPrimitiveType(JvmSignatureParser::PrimitiveTypeContext *ctx) override;
    void exitPrimitiveType(JvmSignatureParser::PrimitiveTypeContext *ctx) override;
    ~JavaSignatureListenerImpl() override;


    [[nodiscard]] std::string getResult();

private:
    std::string _result;
    std::unordered_map<antlr4::ParserRuleContext*, std::string> _ruleContextToJava;
    static std::unordered_map<char, std::string> _primitiveTypeToString;

};

}
#endif
