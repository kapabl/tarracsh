
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

  virtual void enterJavaTypeSignature(JvmSignatureParser::JavaTypeSignatureContext * /*ctx*/) override { }
  virtual void exitJavaTypeSignature(JvmSignatureParser::JavaTypeSignatureContext * /*ctx*/) override { }

  virtual void enterBaseType(JvmSignatureParser::BaseTypeContext * /*ctx*/) override { }
  virtual void exitBaseType(JvmSignatureParser::BaseTypeContext * /*ctx*/) override { }

  virtual void enterReferenceTypeSignature(JvmSignatureParser::ReferenceTypeSignatureContext * /*ctx*/) override { }
  virtual void exitReferenceTypeSignature(JvmSignatureParser::ReferenceTypeSignatureContext * /*ctx*/) override { }

  virtual void enterClassTypeSignature(JvmSignatureParser::ClassTypeSignatureContext * /*ctx*/) override { }
  virtual void exitClassTypeSignature(JvmSignatureParser::ClassTypeSignatureContext * /*ctx*/) override { }

  virtual void enterPackageSpecifier(JvmSignatureParser::PackageSpecifierContext * /*ctx*/) override { }
  virtual void exitPackageSpecifier(JvmSignatureParser::PackageSpecifierContext * /*ctx*/) override { }

  virtual void enterSimpleClassTypeSignature(JvmSignatureParser::SimpleClassTypeSignatureContext * /*ctx*/) override { }
  virtual void exitSimpleClassTypeSignature(JvmSignatureParser::SimpleClassTypeSignatureContext * /*ctx*/) override { }

  virtual void enterTypeArguments(JvmSignatureParser::TypeArgumentsContext * /*ctx*/) override { }
  virtual void exitTypeArguments(JvmSignatureParser::TypeArgumentsContext * /*ctx*/) override { }

  virtual void enterTypeArgument(JvmSignatureParser::TypeArgumentContext * /*ctx*/) override { }
  virtual void exitTypeArgument(JvmSignatureParser::TypeArgumentContext * /*ctx*/) override { }

  virtual void enterWildcardIndicator(JvmSignatureParser::WildcardIndicatorContext * /*ctx*/) override { }
  virtual void exitWildcardIndicator(JvmSignatureParser::WildcardIndicatorContext * /*ctx*/) override { }

  virtual void enterClassTypeSignatureSuffix(JvmSignatureParser::ClassTypeSignatureSuffixContext * /*ctx*/) override { }
  virtual void exitClassTypeSignatureSuffix(JvmSignatureParser::ClassTypeSignatureSuffixContext * /*ctx*/) override { }

  virtual void enterTypeVariableSignature(JvmSignatureParser::TypeVariableSignatureContext * /*ctx*/) override { }
  virtual void exitTypeVariableSignature(JvmSignatureParser::TypeVariableSignatureContext * /*ctx*/) override { }

  virtual void enterArrayTypeSignature(JvmSignatureParser::ArrayTypeSignatureContext * /*ctx*/) override { }
  virtual void exitArrayTypeSignature(JvmSignatureParser::ArrayTypeSignatureContext * /*ctx*/) override { }

  virtual void enterClassSignature(JvmSignatureParser::ClassSignatureContext * /*ctx*/) override { }
  virtual void exitClassSignature(JvmSignatureParser::ClassSignatureContext * /*ctx*/) override { }

  virtual void enterTypeParameters(JvmSignatureParser::TypeParametersContext * /*ctx*/) override { }
  virtual void exitTypeParameters(JvmSignatureParser::TypeParametersContext * /*ctx*/) override { }

  virtual void enterTypeParameter(JvmSignatureParser::TypeParameterContext * /*ctx*/) override { }
  virtual void exitTypeParameter(JvmSignatureParser::TypeParameterContext * /*ctx*/) override { }

  virtual void enterClassBound(JvmSignatureParser::ClassBoundContext * /*ctx*/) override { }
  virtual void exitClassBound(JvmSignatureParser::ClassBoundContext * /*ctx*/) override { }

  virtual void enterSuperclassSignature(JvmSignatureParser::SuperclassSignatureContext * /*ctx*/) override { }
  virtual void exitSuperclassSignature(JvmSignatureParser::SuperclassSignatureContext * /*ctx*/) override { }

  virtual void enterSuperInterfaceSignature(JvmSignatureParser::SuperInterfaceSignatureContext * /*ctx*/) override { }
  virtual void exitSuperInterfaceSignature(JvmSignatureParser::SuperInterfaceSignatureContext * /*ctx*/) override { }

  virtual void enterInterfaceBound(JvmSignatureParser::InterfaceBoundContext * /*ctx*/) override { }
  virtual void exitInterfaceBound(JvmSignatureParser::InterfaceBoundContext * /*ctx*/) override { }

  virtual void enterMethodSignature(JvmSignatureParser::MethodSignatureContext * /*ctx*/) override { }
  virtual void exitMethodSignature(JvmSignatureParser::MethodSignatureContext * /*ctx*/) override { }

  virtual void enterResult(JvmSignatureParser::ResultContext * /*ctx*/) override { }
  virtual void exitResult(JvmSignatureParser::ResultContext * /*ctx*/) override { }

  virtual void enterThrowsSignature(JvmSignatureParser::ThrowsSignatureContext * /*ctx*/) override { }
  virtual void exitThrowsSignature(JvmSignatureParser::ThrowsSignatureContext * /*ctx*/) override { }

  virtual void enterVoidDescriptor(JvmSignatureParser::VoidDescriptorContext * /*ctx*/) override { }
  virtual void exitVoidDescriptor(JvmSignatureParser::VoidDescriptorContext * /*ctx*/) override { }

  virtual void enterFieldSignature(JvmSignatureParser::FieldSignatureContext * /*ctx*/) override { }
  virtual void exitFieldSignature(JvmSignatureParser::FieldSignatureContext * /*ctx*/) override { }


  virtual void enterEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void exitEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void visitTerminal(antlr4::tree::TerminalNode * /*node*/) override { }
  virtual void visitErrorNode(antlr4::tree::ErrorNode * /*node*/) override { }

};

}  // namespace kapa::tarracsh::domain::classfile::signature::parser
