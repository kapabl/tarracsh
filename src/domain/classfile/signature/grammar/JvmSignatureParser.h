
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
    T__20 = 21, T__21 = 22, Identifier = 23, WS = 24
  };

  enum {
    RuleClassSignature = 0, RuleSimpleClassTypeSignature = 1, RuleSuperclassSignature = 2, 
    RuleInterfaceSignature = 3, RuleTypeArguments = 4, RuleTypeArgument = 5, 
    RuleWildcardIndicator = 6, RuleWildcardBound = 7, RuleTypeSignature = 8, 
    RuleClassTypeSignature = 9, RuleFieldSignature = 10, RuleMethodSignature = 11, 
    RulePackageSpecifier = 12, RuleArrayTypeSignature = 13, RuleFieldTypeSignature = 14, 
    RuleMethodTypeSignature = 15, RuleParameterTypeSignature = 16, RuleReturnType = 17, 
    RuleThrowsSignature = 18, RulePrimitiveType = 19
  };

  explicit JvmSignatureParser(antlr4::TokenStream *input);

  JvmSignatureParser(antlr4::TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options);

  ~JvmSignatureParser() override;

  std::string getGrammarFileName() const override;

  const antlr4::atn::ATN& getATN() const override;

  const std::vector<std::string>& getRuleNames() const override;

  const antlr4::dfa::Vocabulary& getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;


  class ClassSignatureContext;
  class SimpleClassTypeSignatureContext;
  class SuperclassSignatureContext;
  class InterfaceSignatureContext;
  class TypeArgumentsContext;
  class TypeArgumentContext;
  class WildcardIndicatorContext;
  class WildcardBoundContext;
  class TypeSignatureContext;
  class ClassTypeSignatureContext;
  class FieldSignatureContext;
  class MethodSignatureContext;
  class PackageSpecifierContext;
  class ArrayTypeSignatureContext;
  class FieldTypeSignatureContext;
  class MethodTypeSignatureContext;
  class ParameterTypeSignatureContext;
  class ReturnTypeContext;
  class ThrowsSignatureContext;
  class PrimitiveTypeContext; 

  class  ClassSignatureContext : public antlr4::ParserRuleContext {
  public:
    ClassSignatureContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    SimpleClassTypeSignatureContext *simpleClassTypeSignature();
    PackageSpecifierContext *packageSpecifier();
    SuperclassSignatureContext *superclassSignature();
    std::vector<InterfaceSignatureContext *> interfaceSignature();
    InterfaceSignatureContext* interfaceSignature(size_t i);
    std::vector<FieldSignatureContext *> fieldSignature();
    FieldSignatureContext* fieldSignature(size_t i);
    std::vector<MethodSignatureContext *> methodSignature();
    MethodSignatureContext* methodSignature(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ClassSignatureContext* classSignature();

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

  class  SuperclassSignatureContext : public antlr4::ParserRuleContext {
  public:
    SuperclassSignatureContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ClassTypeSignatureContext *classTypeSignature();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  SuperclassSignatureContext* superclassSignature();

  class  InterfaceSignatureContext : public antlr4::ParserRuleContext {
  public:
    InterfaceSignatureContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ClassTypeSignatureContext *classTypeSignature();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  InterfaceSignatureContext* interfaceSignature();

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
    TypeSignatureContext *typeSignature();
    WildcardIndicatorContext *wildcardIndicator();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  TypeArgumentContext* typeArgument();

  class  WildcardIndicatorContext : public antlr4::ParserRuleContext {
  public:
    WildcardIndicatorContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    WildcardBoundContext *wildcardBound();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  WildcardIndicatorContext* wildcardIndicator();

  class  WildcardBoundContext : public antlr4::ParserRuleContext {
  public:
    WildcardBoundContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    TypeSignatureContext *typeSignature();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  WildcardBoundContext* wildcardBound();

  class  TypeSignatureContext : public antlr4::ParserRuleContext {
  public:
    TypeSignatureContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ClassTypeSignatureContext *classTypeSignature();
    ArrayTypeSignatureContext *arrayTypeSignature();
    PrimitiveTypeContext *primitiveType();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  TypeSignatureContext* typeSignature();

  class  ClassTypeSignatureContext : public antlr4::ParserRuleContext {
  public:
    ClassTypeSignatureContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> Identifier();
    antlr4::tree::TerminalNode* Identifier(size_t i);
    PackageSpecifierContext *packageSpecifier();
    std::vector<TypeArgumentsContext *> typeArguments();
    TypeArgumentsContext* typeArguments(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ClassTypeSignatureContext* classTypeSignature();

  class  FieldSignatureContext : public antlr4::ParserRuleContext {
  public:
    FieldSignatureContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    FieldTypeSignatureContext *fieldTypeSignature();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  FieldSignatureContext* fieldSignature();

  class  MethodSignatureContext : public antlr4::ParserRuleContext {
  public:
    MethodSignatureContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    MethodTypeSignatureContext *methodTypeSignature();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  MethodSignatureContext* methodSignature();

  class  PackageSpecifierContext : public antlr4::ParserRuleContext {
  public:
    PackageSpecifierContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Identifier();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  PackageSpecifierContext* packageSpecifier();

  class  ArrayTypeSignatureContext : public antlr4::ParserRuleContext {
  public:
    ArrayTypeSignatureContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    TypeSignatureContext *typeSignature();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ArrayTypeSignatureContext* arrayTypeSignature();

  class  FieldTypeSignatureContext : public antlr4::ParserRuleContext {
  public:
    FieldTypeSignatureContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    TypeSignatureContext *typeSignature();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  FieldTypeSignatureContext* fieldTypeSignature();

  class  MethodTypeSignatureContext : public antlr4::ParserRuleContext {
  public:
    MethodTypeSignatureContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ReturnTypeContext *returnType();
    std::vector<ParameterTypeSignatureContext *> parameterTypeSignature();
    ParameterTypeSignatureContext* parameterTypeSignature(size_t i);
    std::vector<ThrowsSignatureContext *> throwsSignature();
    ThrowsSignatureContext* throwsSignature(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  MethodTypeSignatureContext* methodTypeSignature();

  class  ParameterTypeSignatureContext : public antlr4::ParserRuleContext {
  public:
    ParameterTypeSignatureContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    TypeSignatureContext *typeSignature();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ParameterTypeSignatureContext* parameterTypeSignature();

  class  ReturnTypeContext : public antlr4::ParserRuleContext {
  public:
    ReturnTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    TypeSignatureContext *typeSignature();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ReturnTypeContext* returnType();

  class  ThrowsSignatureContext : public antlr4::ParserRuleContext {
  public:
    ThrowsSignatureContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ClassTypeSignatureContext *classTypeSignature();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ThrowsSignatureContext* throwsSignature();

  class  PrimitiveTypeContext : public antlr4::ParserRuleContext {
  public:
    PrimitiveTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  PrimitiveTypeContext* primitiveType();


  // By default the static state used to implement the parser is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:
};

}  // namespace kapa::tarracsh::domain::classfile::signature::parser
