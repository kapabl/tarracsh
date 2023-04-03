
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

  virtual void enterJavaTypeSignature(JvmSignatureParser::JavaTypeSignatureContext *ctx) = 0;
  virtual void exitJavaTypeSignature(JvmSignatureParser::JavaTypeSignatureContext *ctx) = 0;

  virtual void enterBaseType(JvmSignatureParser::BaseTypeContext *ctx) = 0;
  virtual void exitBaseType(JvmSignatureParser::BaseTypeContext *ctx) = 0;

  virtual void enterReferenceTypeSignature(JvmSignatureParser::ReferenceTypeSignatureContext *ctx) = 0;
  virtual void exitReferenceTypeSignature(JvmSignatureParser::ReferenceTypeSignatureContext *ctx) = 0;

  virtual void enterClassTypeSignature(JvmSignatureParser::ClassTypeSignatureContext *ctx) = 0;
  virtual void exitClassTypeSignature(JvmSignatureParser::ClassTypeSignatureContext *ctx) = 0;

  virtual void enterPackageSpecifier(JvmSignatureParser::PackageSpecifierContext *ctx) = 0;
  virtual void exitPackageSpecifier(JvmSignatureParser::PackageSpecifierContext *ctx) = 0;

  virtual void enterSimpleClassTypeSignature(JvmSignatureParser::SimpleClassTypeSignatureContext *ctx) = 0;
  virtual void exitSimpleClassTypeSignature(JvmSignatureParser::SimpleClassTypeSignatureContext *ctx) = 0;

  virtual void enterTypeArguments(JvmSignatureParser::TypeArgumentsContext *ctx) = 0;
  virtual void exitTypeArguments(JvmSignatureParser::TypeArgumentsContext *ctx) = 0;

  virtual void enterTypeArgument(JvmSignatureParser::TypeArgumentContext *ctx) = 0;
  virtual void exitTypeArgument(JvmSignatureParser::TypeArgumentContext *ctx) = 0;

  virtual void enterWildcardIndicator(JvmSignatureParser::WildcardIndicatorContext *ctx) = 0;
  virtual void exitWildcardIndicator(JvmSignatureParser::WildcardIndicatorContext *ctx) = 0;

  virtual void enterClassTypeSignatureSuffix(JvmSignatureParser::ClassTypeSignatureSuffixContext *ctx) = 0;
  virtual void exitClassTypeSignatureSuffix(JvmSignatureParser::ClassTypeSignatureSuffixContext *ctx) = 0;

  virtual void enterTypeVariableSignature(JvmSignatureParser::TypeVariableSignatureContext *ctx) = 0;
  virtual void exitTypeVariableSignature(JvmSignatureParser::TypeVariableSignatureContext *ctx) = 0;

  virtual void enterArrayTypeSignature(JvmSignatureParser::ArrayTypeSignatureContext *ctx) = 0;
  virtual void exitArrayTypeSignature(JvmSignatureParser::ArrayTypeSignatureContext *ctx) = 0;

  virtual void enterClassSignature(JvmSignatureParser::ClassSignatureContext *ctx) = 0;
  virtual void exitClassSignature(JvmSignatureParser::ClassSignatureContext *ctx) = 0;

  virtual void enterTypeParameters(JvmSignatureParser::TypeParametersContext *ctx) = 0;
  virtual void exitTypeParameters(JvmSignatureParser::TypeParametersContext *ctx) = 0;

  virtual void enterTypeParameter(JvmSignatureParser::TypeParameterContext *ctx) = 0;
  virtual void exitTypeParameter(JvmSignatureParser::TypeParameterContext *ctx) = 0;

  virtual void enterClassBound(JvmSignatureParser::ClassBoundContext *ctx) = 0;
  virtual void exitClassBound(JvmSignatureParser::ClassBoundContext *ctx) = 0;

  virtual void enterSuperclassSignature(JvmSignatureParser::SuperclassSignatureContext *ctx) = 0;
  virtual void exitSuperclassSignature(JvmSignatureParser::SuperclassSignatureContext *ctx) = 0;

  virtual void enterSuperInterfaceSignature(JvmSignatureParser::SuperInterfaceSignatureContext *ctx) = 0;
  virtual void exitSuperInterfaceSignature(JvmSignatureParser::SuperInterfaceSignatureContext *ctx) = 0;

  virtual void enterInterfaceBound(JvmSignatureParser::InterfaceBoundContext *ctx) = 0;
  virtual void exitInterfaceBound(JvmSignatureParser::InterfaceBoundContext *ctx) = 0;

  virtual void enterMethodSignature(JvmSignatureParser::MethodSignatureContext *ctx) = 0;
  virtual void exitMethodSignature(JvmSignatureParser::MethodSignatureContext *ctx) = 0;

  virtual void enterResult(JvmSignatureParser::ResultContext *ctx) = 0;
  virtual void exitResult(JvmSignatureParser::ResultContext *ctx) = 0;

  virtual void enterThrowsSignature(JvmSignatureParser::ThrowsSignatureContext *ctx) = 0;
  virtual void exitThrowsSignature(JvmSignatureParser::ThrowsSignatureContext *ctx) = 0;

  virtual void enterVoidDescriptor(JvmSignatureParser::VoidDescriptorContext *ctx) = 0;
  virtual void exitVoidDescriptor(JvmSignatureParser::VoidDescriptorContext *ctx) = 0;

  virtual void enterFieldSignature(JvmSignatureParser::FieldSignatureContext *ctx) = 0;
  virtual void exitFieldSignature(JvmSignatureParser::FieldSignatureContext *ctx) = 0;


};

}  // namespace kapa::tarracsh::domain::classfile::signature::parser
