
// Generated from JvmSignature.g4 by ANTLR 4.12.0

#pragma once


#include "antlr4-runtime.h"


namespace kapa::tarracsh::domain::classfile::signature::parser {


class  JvmSignatureParser : public antlr4::Parser {
public:
  enum {
    T__0 = 1, T__1 = 2, T__2 = 3, T__3 = 4, T__4 = 5, T__5 = 6, T__6 = 7, 
    T__7 = 8, T__8 = 9, T__9 = 10, T__10 = 11, T__11 = 12, T__12 = 13, T__13 = 14, 
    T__14 = 15, T__15 = 16, T__16 = 17, T__17 = 18, T__18 = 19, T__19 = 20, 
    T__20 = 21, T__21 = 22, T__22 = 23, Identifier = 24
  };

  enum {
    RuleJavaTypeSignature = 0, RuleBaseType = 1, RuleReferenceTypeSignature = 2, 
    RuleClassTypeSignature = 3, RulePackageSpecifier = 4, RuleSimpleClassTypeSignature = 5, 
    RuleTypeArguments = 6, RuleTypeArgument = 7, RuleWildcardIndicator = 8, 
    RuleClassTypeSignatureSuffix = 9, RuleTypeVariableSignature = 10, RuleArrayTypeSignature = 11, 
    RuleClassSignature = 12, RuleTypeParameters = 13, RuleTypeParameter = 14, 
    RuleClassBound = 15, RuleSuperclassSignature = 16, RuleSuperInterfaceSignature = 17, 
    RuleInterfaceBound = 18, RuleMethodSignature = 19, RuleResult = 20, 
    RuleThrowsSignature = 21, RuleVoidDescriptor = 22, RuleFieldSignature = 23
  };

  explicit JvmSignatureParser(antlr4::TokenStream *input);

  JvmSignatureParser(antlr4::TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options);

  ~JvmSignatureParser() override;

  std::string getGrammarFileName() const override;

  const antlr4::atn::ATN& getATN() const override;

  const std::vector<std::string>& getRuleNames() const override;

  const antlr4::dfa::Vocabulary& getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;


  class JavaTypeSignatureContext;
  class BaseTypeContext;
  class ReferenceTypeSignatureContext;
  class ClassTypeSignatureContext;
  class PackageSpecifierContext;
  class SimpleClassTypeSignatureContext;
  class TypeArgumentsContext;
  class TypeArgumentContext;
  class WildcardIndicatorContext;
  class ClassTypeSignatureSuffixContext;
  class TypeVariableSignatureContext;
  class ArrayTypeSignatureContext;
  class ClassSignatureContext;
  class TypeParametersContext;
  class TypeParameterContext;
  class ClassBoundContext;
  class SuperclassSignatureContext;
  class SuperInterfaceSignatureContext;
  class InterfaceBoundContext;
  class MethodSignatureContext;
  class ResultContext;
  class ThrowsSignatureContext;
  class VoidDescriptorContext;
  class FieldSignatureContext; 

  class  JavaTypeSignatureContext : public antlr4::ParserRuleContext {
  public:
    JavaTypeSignatureContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ReferenceTypeSignatureContext *referenceTypeSignature();
    BaseTypeContext *baseType();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  JavaTypeSignatureContext* javaTypeSignature();

  class  BaseTypeContext : public antlr4::ParserRuleContext {
  public:
    BaseTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  BaseTypeContext* baseType();

  class  ReferenceTypeSignatureContext : public antlr4::ParserRuleContext {
  public:
    ReferenceTypeSignatureContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ClassTypeSignatureContext *classTypeSignature();
    TypeVariableSignatureContext *typeVariableSignature();
    ArrayTypeSignatureContext *arrayTypeSignature();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ReferenceTypeSignatureContext* referenceTypeSignature();

  class  ClassTypeSignatureContext : public antlr4::ParserRuleContext {
  public:
    ClassTypeSignatureContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    SimpleClassTypeSignatureContext *simpleClassTypeSignature();
    PackageSpecifierContext *packageSpecifier();
    std::vector<ClassTypeSignatureSuffixContext *> classTypeSignatureSuffix();
    ClassTypeSignatureSuffixContext* classTypeSignatureSuffix(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ClassTypeSignatureContext* classTypeSignature();

  class  PackageSpecifierContext : public antlr4::ParserRuleContext {
  public:
    PackageSpecifierContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Identifier();
    PackageSpecifierContext *packageSpecifier();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  PackageSpecifierContext* packageSpecifier();

  class  SimpleClassTypeSignatureContext : public antlr4::ParserRuleContext {
  public:
    SimpleClassTypeSignatureContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Identifier();
    TypeArgumentsContext *typeArguments();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  SimpleClassTypeSignatureContext* simpleClassTypeSignature();

  class  TypeArgumentsContext : public antlr4::ParserRuleContext {
  public:
    TypeArgumentsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<TypeArgumentContext *> typeArgument();
    TypeArgumentContext* typeArgument(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  TypeArgumentsContext* typeArguments();

  class  TypeArgumentContext : public antlr4::ParserRuleContext {
  public:
    TypeArgumentContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ReferenceTypeSignatureContext *referenceTypeSignature();
    WildcardIndicatorContext *wildcardIndicator();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  TypeArgumentContext* typeArgument();

  class  WildcardIndicatorContext : public antlr4::ParserRuleContext {
  public:
    WildcardIndicatorContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  WildcardIndicatorContext* wildcardIndicator();

  class  ClassTypeSignatureSuffixContext : public antlr4::ParserRuleContext {
  public:
    ClassTypeSignatureSuffixContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    SimpleClassTypeSignatureContext *simpleClassTypeSignature();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ClassTypeSignatureSuffixContext* classTypeSignatureSuffix();

  class  TypeVariableSignatureContext : public antlr4::ParserRuleContext {
  public:
    TypeVariableSignatureContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Identifier();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  TypeVariableSignatureContext* typeVariableSignature();

  class  ArrayTypeSignatureContext : public antlr4::ParserRuleContext {
  public:
    ArrayTypeSignatureContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    JavaTypeSignatureContext *javaTypeSignature();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ArrayTypeSignatureContext* arrayTypeSignature();

  class  ClassSignatureContext : public antlr4::ParserRuleContext {
  public:
    ClassSignatureContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    SuperclassSignatureContext *superclassSignature();
    TypeParametersContext *typeParameters();
    std::vector<SuperInterfaceSignatureContext *> superInterfaceSignature();
    SuperInterfaceSignatureContext* superInterfaceSignature(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ClassSignatureContext* classSignature();

  class  TypeParametersContext : public antlr4::ParserRuleContext {
  public:
    TypeParametersContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<TypeParameterContext *> typeParameter();
    TypeParameterContext* typeParameter(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  TypeParametersContext* typeParameters();

  class  TypeParameterContext : public antlr4::ParserRuleContext {
  public:
    TypeParameterContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Identifier();
    ClassBoundContext *classBound();
    std::vector<InterfaceBoundContext *> interfaceBound();
    InterfaceBoundContext* interfaceBound(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  TypeParameterContext* typeParameter();

  class  ClassBoundContext : public antlr4::ParserRuleContext {
  public:
    ClassBoundContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ReferenceTypeSignatureContext *referenceTypeSignature();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ClassBoundContext* classBound();

  class  SuperclassSignatureContext : public antlr4::ParserRuleContext {
  public:
    SuperclassSignatureContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ClassTypeSignatureContext *classTypeSignature();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  SuperclassSignatureContext* superclassSignature();

  class  SuperInterfaceSignatureContext : public antlr4::ParserRuleContext {
  public:
    SuperInterfaceSignatureContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ClassTypeSignatureContext *classTypeSignature();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  SuperInterfaceSignatureContext* superInterfaceSignature();

  class  InterfaceBoundContext : public antlr4::ParserRuleContext {
  public:
    InterfaceBoundContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ReferenceTypeSignatureContext *referenceTypeSignature();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  InterfaceBoundContext* interfaceBound();

  class  MethodSignatureContext : public antlr4::ParserRuleContext {
  public:
    MethodSignatureContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ResultContext *result();
    TypeParameterContext *typeParameter();
    std::vector<JavaTypeSignatureContext *> javaTypeSignature();
    JavaTypeSignatureContext* javaTypeSignature(size_t i);
    std::vector<ThrowsSignatureContext *> throwsSignature();
    ThrowsSignatureContext* throwsSignature(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  MethodSignatureContext* methodSignature();

  class  ResultContext : public antlr4::ParserRuleContext {
  public:
    ResultContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    JavaTypeSignatureContext *javaTypeSignature();
    VoidDescriptorContext *voidDescriptor();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ResultContext* result();

  class  ThrowsSignatureContext : public antlr4::ParserRuleContext {
  public:
    ThrowsSignatureContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ClassTypeSignatureContext *classTypeSignature();
    TypeVariableSignatureContext *typeVariableSignature();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ThrowsSignatureContext* throwsSignature();

  class  VoidDescriptorContext : public antlr4::ParserRuleContext {
  public:
    VoidDescriptorContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  VoidDescriptorContext* voidDescriptor();

  class  FieldSignatureContext : public antlr4::ParserRuleContext {
  public:
    FieldSignatureContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ReferenceTypeSignatureContext *referenceTypeSignature();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  FieldSignatureContext* fieldSignature();


  // By default the static state used to implement the parser is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:
};

}  // namespace kapa::tarracsh::domain::classfile::signature::parser
