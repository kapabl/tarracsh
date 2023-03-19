
// Generated from JvmSignature.g4 by ANTLR 4.12.0

#pragma once


#include "antlr4-runtime.h"
#include "JvmSignatureParser.h"


namespace kapa::tarracsh::domain::classfile::signature::parser {

/**
 * This interface defines an abstract listener for a parse tree produced by JvmSignatureParser.
 */
class  JvmSignatureListener : public antlr4::tree::ParseTreeListener {
public:

  virtual void enterClassSignature(JvmSignatureParser::ClassSignatureContext *ctx) = 0;
  virtual void exitClassSignature(JvmSignatureParser::ClassSignatureContext *ctx) = 0;

  virtual void enterSimpleClassTypeSignature(JvmSignatureParser::SimpleClassTypeSignatureContext *ctx) = 0;
  virtual void exitSimpleClassTypeSignature(JvmSignatureParser::SimpleClassTypeSignatureContext *ctx) = 0;

  virtual void enterSuperclassSignature(JvmSignatureParser::SuperclassSignatureContext *ctx) = 0;
  virtual void exitSuperclassSignature(JvmSignatureParser::SuperclassSignatureContext *ctx) = 0;

  virtual void enterInterfaceSignature(JvmSignatureParser::InterfaceSignatureContext *ctx) = 0;
  virtual void exitInterfaceSignature(JvmSignatureParser::InterfaceSignatureContext *ctx) = 0;

  virtual void enterTypeArguments(JvmSignatureParser::TypeArgumentsContext *ctx) = 0;
  virtual void exitTypeArguments(JvmSignatureParser::TypeArgumentsContext *ctx) = 0;

  virtual void enterTypeArgument(JvmSignatureParser::TypeArgumentContext *ctx) = 0;
  virtual void exitTypeArgument(JvmSignatureParser::TypeArgumentContext *ctx) = 0;

  virtual void enterWildcardIndicator(JvmSignatureParser::WildcardIndicatorContext *ctx) = 0;
  virtual void exitWildcardIndicator(JvmSignatureParser::WildcardIndicatorContext *ctx) = 0;

  virtual void enterWildcardBound(JvmSignatureParser::WildcardBoundContext *ctx) = 0;
  virtual void exitWildcardBound(JvmSignatureParser::WildcardBoundContext *ctx) = 0;

  virtual void enterTypeSignature(JvmSignatureParser::TypeSignatureContext *ctx) = 0;
  virtual void exitTypeSignature(JvmSignatureParser::TypeSignatureContext *ctx) = 0;

  virtual void enterClassTypeSignature(JvmSignatureParser::ClassTypeSignatureContext *ctx) = 0;
  virtual void exitClassTypeSignature(JvmSignatureParser::ClassTypeSignatureContext *ctx) = 0;

  virtual void enterFieldSignature(JvmSignatureParser::FieldSignatureContext *ctx) = 0;
  virtual void exitFieldSignature(JvmSignatureParser::FieldSignatureContext *ctx) = 0;

  virtual void enterMethodSignature(JvmSignatureParser::MethodSignatureContext *ctx) = 0;
  virtual void exitMethodSignature(JvmSignatureParser::MethodSignatureContext *ctx) = 0;

  virtual void enterPackageSpecifier(JvmSignatureParser::PackageSpecifierContext *ctx) = 0;
  virtual void exitPackageSpecifier(JvmSignatureParser::PackageSpecifierContext *ctx) = 0;

  virtual void enterArrayTypeSignature(JvmSignatureParser::ArrayTypeSignatureContext *ctx) = 0;
  virtual void exitArrayTypeSignature(JvmSignatureParser::ArrayTypeSignatureContext *ctx) = 0;

  virtual void enterFieldTypeSignature(JvmSignatureParser::FieldTypeSignatureContext *ctx) = 0;
  virtual void exitFieldTypeSignature(JvmSignatureParser::FieldTypeSignatureContext *ctx) = 0;

  virtual void enterMethodTypeSignature(JvmSignatureParser::MethodTypeSignatureContext *ctx) = 0;
  virtual void exitMethodTypeSignature(JvmSignatureParser::MethodTypeSignatureContext *ctx) = 0;

  virtual void enterParameterTypeSignature(JvmSignatureParser::ParameterTypeSignatureContext *ctx) = 0;
  virtual void exitParameterTypeSignature(JvmSignatureParser::ParameterTypeSignatureContext *ctx) = 0;

  virtual void enterReturnType(JvmSignatureParser::ReturnTypeContext *ctx) = 0;
  virtual void exitReturnType(JvmSignatureParser::ReturnTypeContext *ctx) = 0;

  virtual void enterThrowsSignature(JvmSignatureParser::ThrowsSignatureContext *ctx) = 0;
  virtual void exitThrowsSignature(JvmSignatureParser::ThrowsSignatureContext *ctx) = 0;

  virtual void enterPrimitiveType(JvmSignatureParser::PrimitiveTypeContext *ctx) = 0;
  virtual void exitPrimitiveType(JvmSignatureParser::PrimitiveTypeContext *ctx) = 0;


};

}  // namespace kapa::tarracsh::domain::classfile::signature::parser
