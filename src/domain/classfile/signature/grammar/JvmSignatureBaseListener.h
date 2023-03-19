
// Generated from JvmSignature.g4 by ANTLR 4.12.0

#pragma once


#include "antlr4-runtime.h"
#include "JvmSignatureListener.h"


namespace kapa::tarracsh::domain::classfile::signature::parser {

/**
 * This class provides an empty implementation of JvmSignatureListener,
 * which can be extended to create a listener which only needs to handle a subset
 * of the available methods.
 */
class  JvmSignatureBaseListener : public JvmSignatureListener {
public:

  virtual void enterClassSignature(JvmSignatureParser::ClassSignatureContext * /*ctx*/) override { }
  virtual void exitClassSignature(JvmSignatureParser::ClassSignatureContext * /*ctx*/) override { }

  virtual void enterSimpleClassTypeSignature(JvmSignatureParser::SimpleClassTypeSignatureContext * /*ctx*/) override { }
  virtual void exitSimpleClassTypeSignature(JvmSignatureParser::SimpleClassTypeSignatureContext * /*ctx*/) override { }

  virtual void enterSuperclassSignature(JvmSignatureParser::SuperclassSignatureContext * /*ctx*/) override { }
  virtual void exitSuperclassSignature(JvmSignatureParser::SuperclassSignatureContext * /*ctx*/) override { }

  virtual void enterInterfaceSignature(JvmSignatureParser::InterfaceSignatureContext * /*ctx*/) override { }
  virtual void exitInterfaceSignature(JvmSignatureParser::InterfaceSignatureContext * /*ctx*/) override { }

  virtual void enterTypeArguments(JvmSignatureParser::TypeArgumentsContext * /*ctx*/) override { }
  virtual void exitTypeArguments(JvmSignatureParser::TypeArgumentsContext * /*ctx*/) override { }

  virtual void enterTypeArgument(JvmSignatureParser::TypeArgumentContext * /*ctx*/) override { }
  virtual void exitTypeArgument(JvmSignatureParser::TypeArgumentContext * /*ctx*/) override { }

  virtual void enterWildcardIndicator(JvmSignatureParser::WildcardIndicatorContext * /*ctx*/) override { }
  virtual void exitWildcardIndicator(JvmSignatureParser::WildcardIndicatorContext * /*ctx*/) override { }

  virtual void enterWildcardBound(JvmSignatureParser::WildcardBoundContext * /*ctx*/) override { }
  virtual void exitWildcardBound(JvmSignatureParser::WildcardBoundContext * /*ctx*/) override { }

  virtual void enterTypeSignature(JvmSignatureParser::TypeSignatureContext * /*ctx*/) override { }
  virtual void exitTypeSignature(JvmSignatureParser::TypeSignatureContext * /*ctx*/) override { }

  virtual void enterClassTypeSignature(JvmSignatureParser::ClassTypeSignatureContext * /*ctx*/) override { }
  virtual void exitClassTypeSignature(JvmSignatureParser::ClassTypeSignatureContext * /*ctx*/) override { }

  virtual void enterFieldSignature(JvmSignatureParser::FieldSignatureContext * /*ctx*/) override { }
  virtual void exitFieldSignature(JvmSignatureParser::FieldSignatureContext * /*ctx*/) override { }

  virtual void enterMethodSignature(JvmSignatureParser::MethodSignatureContext * /*ctx*/) override { }
  virtual void exitMethodSignature(JvmSignatureParser::MethodSignatureContext * /*ctx*/) override { }

  virtual void enterPackageSpecifier(JvmSignatureParser::PackageSpecifierContext * /*ctx*/) override { }
  virtual void exitPackageSpecifier(JvmSignatureParser::PackageSpecifierContext * /*ctx*/) override { }

  virtual void enterArrayTypeSignature(JvmSignatureParser::ArrayTypeSignatureContext * /*ctx*/) override { }
  virtual void exitArrayTypeSignature(JvmSignatureParser::ArrayTypeSignatureContext * /*ctx*/) override { }

  virtual void enterFieldTypeSignature(JvmSignatureParser::FieldTypeSignatureContext * /*ctx*/) override { }
  virtual void exitFieldTypeSignature(JvmSignatureParser::FieldTypeSignatureContext * /*ctx*/) override { }

  virtual void enterMethodTypeSignature(JvmSignatureParser::MethodTypeSignatureContext * /*ctx*/) override { }
  virtual void exitMethodTypeSignature(JvmSignatureParser::MethodTypeSignatureContext * /*ctx*/) override { }

  virtual void enterParameterTypeSignature(JvmSignatureParser::ParameterTypeSignatureContext * /*ctx*/) override { }
  virtual void exitParameterTypeSignature(JvmSignatureParser::ParameterTypeSignatureContext * /*ctx*/) override { }

  virtual void enterReturnType(JvmSignatureParser::ReturnTypeContext * /*ctx*/) override { }
  virtual void exitReturnType(JvmSignatureParser::ReturnTypeContext * /*ctx*/) override { }

  virtual void enterThrowsSignature(JvmSignatureParser::ThrowsSignatureContext * /*ctx*/) override { }
  virtual void exitThrowsSignature(JvmSignatureParser::ThrowsSignatureContext * /*ctx*/) override { }

  virtual void enterPrimitiveType(JvmSignatureParser::PrimitiveTypeContext * /*ctx*/) override { }
  virtual void exitPrimitiveType(JvmSignatureParser::PrimitiveTypeContext * /*ctx*/) override { }


  virtual void enterEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void exitEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void visitTerminal(antlr4::tree::TerminalNode * /*node*/) override { }
  virtual void visitErrorNode(antlr4::tree::ErrorNode * /*node*/) override { }

};

}  // namespace kapa::tarracsh::domain::classfile::signature::parser
