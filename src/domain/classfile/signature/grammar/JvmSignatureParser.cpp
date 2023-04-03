
// Generated from JvmSignature.g4 by ANTLR 4.12.0


#include "JvmSignatureListener.h"

#include "JvmSignatureParser.h"


using namespace antlrcpp;
using namespace kapa::tarracsh::domain::classfile::signature::parser;

using namespace antlr4;

namespace {

struct JvmSignatureParserStaticData final {
  JvmSignatureParserStaticData(std::vector<std::string> ruleNames,
                        std::vector<std::string> literalNames,
                        std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  JvmSignatureParserStaticData(const JvmSignatureParserStaticData&) = delete;
  JvmSignatureParserStaticData(JvmSignatureParserStaticData&&) = delete;
  JvmSignatureParserStaticData& operator=(const JvmSignatureParserStaticData&) = delete;
  JvmSignatureParserStaticData& operator=(JvmSignatureParserStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

::antlr4::internal::OnceFlag jvmsignatureParserOnceFlag;
JvmSignatureParserStaticData *jvmsignatureParserStaticData = nullptr;

void jvmsignatureParserInitialize() {
  assert(jvmsignatureParserStaticData == nullptr);
  auto staticData = std::make_unique<JvmSignatureParserStaticData>(
    std::vector<std::string>{
      "javaTypeSignature", "baseType", "referenceTypeSignature", "classTypeSignature", 
      "packageSpecifier", "simpleClassTypeSignature", "typeArguments", "typeArgument", 
      "wildcardIndicator", "classTypeSignatureSuffix", "typeVariableSignature", 
      "arrayTypeSignature", "classSignature", "typeParameters", "typeParameter", 
      "classBound", "superclassSignature", "superInterfaceSignature", "interfaceBound", 
      "methodSignature", "result", "throwsSignature", "voidDescriptor", 
      "fieldSignature"
    },
    std::vector<std::string>{
      "", "'B'", "'C'", "'D'", "'F'", "'I'", "'J'", "'S'", "'Z'", "'L'", 
      "';'", "'/'", "'<'", "'>'", "'*'", "'+'", "'-'", "'.'", "'T'", "'['", 
      "'('", "')'", "'^'", "'V'"
    },
    std::vector<std::string>{
      "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
      "", "", "", "", "", "", "", "Identifier"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,1,24,179,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,6,2,
  	7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,2,14,7,
  	14,2,15,7,15,2,16,7,16,2,17,7,17,2,18,7,18,2,19,7,19,2,20,7,20,2,21,7,
  	21,2,22,7,22,2,23,7,23,1,0,1,0,3,0,51,8,0,1,1,1,1,1,2,1,2,1,2,3,2,58,
  	8,2,1,3,1,3,3,3,62,8,3,1,3,1,3,5,3,66,8,3,10,3,12,3,69,9,3,1,3,1,3,1,
  	4,1,4,1,4,1,4,1,5,1,5,3,5,79,8,5,1,6,1,6,1,6,5,6,84,8,6,10,6,12,6,87,
  	9,6,1,6,1,6,1,7,3,7,92,8,7,1,7,1,7,1,7,1,8,1,8,1,9,1,9,1,9,1,10,1,10,
  	1,10,1,10,1,11,1,11,1,11,1,12,3,12,110,8,12,1,12,1,12,5,12,114,8,12,10,
  	12,12,12,117,9,12,1,13,1,13,1,13,5,13,122,8,13,10,13,12,13,125,9,13,1,
  	13,1,13,1,14,1,14,1,14,5,14,132,8,14,10,14,12,14,135,9,14,1,15,3,15,138,
  	8,15,1,16,1,16,1,16,1,17,1,17,1,17,1,18,1,18,1,19,3,19,149,8,19,1,19,
  	1,19,5,19,153,8,19,10,19,12,19,156,9,19,1,19,1,19,1,19,5,19,161,8,19,
  	10,19,12,19,164,9,19,1,20,1,20,3,20,168,8,20,1,21,1,21,1,21,1,21,1,21,
  	1,22,1,22,1,23,1,23,1,23,0,0,24,0,2,4,6,8,10,12,14,16,18,20,22,24,26,
  	28,30,32,34,36,38,40,42,44,46,0,2,1,0,1,8,1,0,15,16,171,0,50,1,0,0,0,
  	2,52,1,0,0,0,4,57,1,0,0,0,6,59,1,0,0,0,8,72,1,0,0,0,10,76,1,0,0,0,12,
  	80,1,0,0,0,14,91,1,0,0,0,16,96,1,0,0,0,18,98,1,0,0,0,20,101,1,0,0,0,22,
  	105,1,0,0,0,24,109,1,0,0,0,26,118,1,0,0,0,28,128,1,0,0,0,30,137,1,0,0,
  	0,32,139,1,0,0,0,34,142,1,0,0,0,36,145,1,0,0,0,38,148,1,0,0,0,40,167,
  	1,0,0,0,42,169,1,0,0,0,44,174,1,0,0,0,46,176,1,0,0,0,48,51,3,4,2,0,49,
  	51,3,2,1,0,50,48,1,0,0,0,50,49,1,0,0,0,51,1,1,0,0,0,52,53,7,0,0,0,53,
  	3,1,0,0,0,54,58,3,6,3,0,55,58,3,20,10,0,56,58,3,22,11,0,57,54,1,0,0,0,
  	57,55,1,0,0,0,57,56,1,0,0,0,58,5,1,0,0,0,59,61,5,9,0,0,60,62,3,8,4,0,
  	61,60,1,0,0,0,61,62,1,0,0,0,62,63,1,0,0,0,63,67,3,10,5,0,64,66,3,18,9,
  	0,65,64,1,0,0,0,66,69,1,0,0,0,67,65,1,0,0,0,67,68,1,0,0,0,68,70,1,0,0,
  	0,69,67,1,0,0,0,70,71,5,10,0,0,71,7,1,0,0,0,72,73,5,24,0,0,73,74,5,11,
  	0,0,74,75,3,8,4,0,75,9,1,0,0,0,76,78,5,24,0,0,77,79,3,12,6,0,78,77,1,
  	0,0,0,78,79,1,0,0,0,79,11,1,0,0,0,80,81,5,12,0,0,81,85,3,14,7,0,82,84,
  	3,14,7,0,83,82,1,0,0,0,84,87,1,0,0,0,85,83,1,0,0,0,85,86,1,0,0,0,86,88,
  	1,0,0,0,87,85,1,0,0,0,88,89,5,13,0,0,89,13,1,0,0,0,90,92,3,16,8,0,91,
  	90,1,0,0,0,91,92,1,0,0,0,92,93,1,0,0,0,93,94,3,4,2,0,94,95,5,14,0,0,95,
  	15,1,0,0,0,96,97,7,1,0,0,97,17,1,0,0,0,98,99,5,17,0,0,99,100,3,10,5,0,
  	100,19,1,0,0,0,101,102,5,18,0,0,102,103,5,24,0,0,103,104,5,10,0,0,104,
  	21,1,0,0,0,105,106,5,19,0,0,106,107,3,0,0,0,107,23,1,0,0,0,108,110,3,
  	26,13,0,109,108,1,0,0,0,109,110,1,0,0,0,110,111,1,0,0,0,111,115,3,32,
  	16,0,112,114,3,34,17,0,113,112,1,0,0,0,114,117,1,0,0,0,115,113,1,0,0,
  	0,115,116,1,0,0,0,116,25,1,0,0,0,117,115,1,0,0,0,118,119,5,12,0,0,119,
  	123,3,28,14,0,120,122,3,28,14,0,121,120,1,0,0,0,122,125,1,0,0,0,123,121,
  	1,0,0,0,123,124,1,0,0,0,124,126,1,0,0,0,125,123,1,0,0,0,126,127,5,13,
  	0,0,127,27,1,0,0,0,128,129,5,24,0,0,129,133,3,30,15,0,130,132,3,36,18,
  	0,131,130,1,0,0,0,132,135,1,0,0,0,133,131,1,0,0,0,133,134,1,0,0,0,134,
  	29,1,0,0,0,135,133,1,0,0,0,136,138,3,4,2,0,137,136,1,0,0,0,137,138,1,
  	0,0,0,138,31,1,0,0,0,139,140,5,9,0,0,140,141,3,6,3,0,141,33,1,0,0,0,142,
  	143,5,9,0,0,143,144,3,6,3,0,144,35,1,0,0,0,145,146,3,4,2,0,146,37,1,0,
  	0,0,147,149,3,28,14,0,148,147,1,0,0,0,148,149,1,0,0,0,149,150,1,0,0,0,
  	150,154,5,20,0,0,151,153,3,0,0,0,152,151,1,0,0,0,153,156,1,0,0,0,154,
  	152,1,0,0,0,154,155,1,0,0,0,155,157,1,0,0,0,156,154,1,0,0,0,157,158,5,
  	21,0,0,158,162,3,40,20,0,159,161,3,42,21,0,160,159,1,0,0,0,161,164,1,
  	0,0,0,162,160,1,0,0,0,162,163,1,0,0,0,163,39,1,0,0,0,164,162,1,0,0,0,
  	165,168,3,0,0,0,166,168,3,44,22,0,167,165,1,0,0,0,167,166,1,0,0,0,168,
  	41,1,0,0,0,169,170,5,22,0,0,170,171,3,6,3,0,171,172,5,22,0,0,172,173,
  	3,20,10,0,173,43,1,0,0,0,174,175,5,23,0,0,175,45,1,0,0,0,176,177,3,4,
  	2,0,177,47,1,0,0,0,16,50,57,61,67,78,85,91,109,115,123,133,137,148,154,
  	162,167
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  jvmsignatureParserStaticData = staticData.release();
}

}

JvmSignatureParser::JvmSignatureParser(TokenStream *input) : JvmSignatureParser(input, antlr4::atn::ParserATNSimulatorOptions()) {}

JvmSignatureParser::JvmSignatureParser(TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options) : Parser(input) {
  JvmSignatureParser::initialize();
  _interpreter = new atn::ParserATNSimulator(this, *jvmsignatureParserStaticData->atn, jvmsignatureParserStaticData->decisionToDFA, jvmsignatureParserStaticData->sharedContextCache, options);
}

JvmSignatureParser::~JvmSignatureParser() {
  delete _interpreter;
}

const atn::ATN& JvmSignatureParser::getATN() const {
  return *jvmsignatureParserStaticData->atn;
}

std::string JvmSignatureParser::getGrammarFileName() const {
  return "JvmSignature.g4";
}

const std::vector<std::string>& JvmSignatureParser::getRuleNames() const {
  return jvmsignatureParserStaticData->ruleNames;
}

const dfa::Vocabulary& JvmSignatureParser::getVocabulary() const {
  return jvmsignatureParserStaticData->vocabulary;
}

antlr4::atn::SerializedATNView JvmSignatureParser::getSerializedATN() const {
  return jvmsignatureParserStaticData->serializedATN;
}


//----------------- JavaTypeSignatureContext ------------------------------------------------------------------

JvmSignatureParser::JavaTypeSignatureContext::JavaTypeSignatureContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

JvmSignatureParser::ReferenceTypeSignatureContext* JvmSignatureParser::JavaTypeSignatureContext::referenceTypeSignature() {
  return getRuleContext<JvmSignatureParser::ReferenceTypeSignatureContext>(0);
}

JvmSignatureParser::BaseTypeContext* JvmSignatureParser::JavaTypeSignatureContext::baseType() {
  return getRuleContext<JvmSignatureParser::BaseTypeContext>(0);
}


size_t JvmSignatureParser::JavaTypeSignatureContext::getRuleIndex() const {
  return JvmSignatureParser::RuleJavaTypeSignature;
}

void JvmSignatureParser::JavaTypeSignatureContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JvmSignatureListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterJavaTypeSignature(this);
}

void JvmSignatureParser::JavaTypeSignatureContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JvmSignatureListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitJavaTypeSignature(this);
}

JvmSignatureParser::JavaTypeSignatureContext* JvmSignatureParser::javaTypeSignature() {
  JavaTypeSignatureContext *_localctx = _tracker.createInstance<JavaTypeSignatureContext>(_ctx, getState());
  enterRule(_localctx, 0, JvmSignatureParser::RuleJavaTypeSignature);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(50);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case JvmSignatureParser::T__8:
      case JvmSignatureParser::T__17:
      case JvmSignatureParser::T__18: {
        enterOuterAlt(_localctx, 1);
        setState(48);
        referenceTypeSignature();
        break;
      }

      case JvmSignatureParser::T__0:
      case JvmSignatureParser::T__1:
      case JvmSignatureParser::T__2:
      case JvmSignatureParser::T__3:
      case JvmSignatureParser::T__4:
      case JvmSignatureParser::T__5:
      case JvmSignatureParser::T__6:
      case JvmSignatureParser::T__7: {
        enterOuterAlt(_localctx, 2);
        setState(49);
        baseType();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BaseTypeContext ------------------------------------------------------------------

JvmSignatureParser::BaseTypeContext::BaseTypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t JvmSignatureParser::BaseTypeContext::getRuleIndex() const {
  return JvmSignatureParser::RuleBaseType;
}

void JvmSignatureParser::BaseTypeContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JvmSignatureListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterBaseType(this);
}

void JvmSignatureParser::BaseTypeContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JvmSignatureListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitBaseType(this);
}

JvmSignatureParser::BaseTypeContext* JvmSignatureParser::baseType() {
  BaseTypeContext *_localctx = _tracker.createInstance<BaseTypeContext>(_ctx, getState());
  enterRule(_localctx, 2, JvmSignatureParser::RuleBaseType);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(52);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 510) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ReferenceTypeSignatureContext ------------------------------------------------------------------

JvmSignatureParser::ReferenceTypeSignatureContext::ReferenceTypeSignatureContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

JvmSignatureParser::ClassTypeSignatureContext* JvmSignatureParser::ReferenceTypeSignatureContext::classTypeSignature() {
  return getRuleContext<JvmSignatureParser::ClassTypeSignatureContext>(0);
}

JvmSignatureParser::TypeVariableSignatureContext* JvmSignatureParser::ReferenceTypeSignatureContext::typeVariableSignature() {
  return getRuleContext<JvmSignatureParser::TypeVariableSignatureContext>(0);
}

JvmSignatureParser::ArrayTypeSignatureContext* JvmSignatureParser::ReferenceTypeSignatureContext::arrayTypeSignature() {
  return getRuleContext<JvmSignatureParser::ArrayTypeSignatureContext>(0);
}


size_t JvmSignatureParser::ReferenceTypeSignatureContext::getRuleIndex() const {
  return JvmSignatureParser::RuleReferenceTypeSignature;
}

void JvmSignatureParser::ReferenceTypeSignatureContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JvmSignatureListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterReferenceTypeSignature(this);
}

void JvmSignatureParser::ReferenceTypeSignatureContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JvmSignatureListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitReferenceTypeSignature(this);
}

JvmSignatureParser::ReferenceTypeSignatureContext* JvmSignatureParser::referenceTypeSignature() {
  ReferenceTypeSignatureContext *_localctx = _tracker.createInstance<ReferenceTypeSignatureContext>(_ctx, getState());
  enterRule(_localctx, 4, JvmSignatureParser::RuleReferenceTypeSignature);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(57);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case JvmSignatureParser::T__8: {
        enterOuterAlt(_localctx, 1);
        setState(54);
        classTypeSignature();
        break;
      }

      case JvmSignatureParser::T__17: {
        enterOuterAlt(_localctx, 2);
        setState(55);
        typeVariableSignature();
        break;
      }

      case JvmSignatureParser::T__18: {
        enterOuterAlt(_localctx, 3);
        setState(56);
        arrayTypeSignature();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ClassTypeSignatureContext ------------------------------------------------------------------

JvmSignatureParser::ClassTypeSignatureContext::ClassTypeSignatureContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

JvmSignatureParser::SimpleClassTypeSignatureContext* JvmSignatureParser::ClassTypeSignatureContext::simpleClassTypeSignature() {
  return getRuleContext<JvmSignatureParser::SimpleClassTypeSignatureContext>(0);
}

JvmSignatureParser::PackageSpecifierContext* JvmSignatureParser::ClassTypeSignatureContext::packageSpecifier() {
  return getRuleContext<JvmSignatureParser::PackageSpecifierContext>(0);
}

std::vector<JvmSignatureParser::ClassTypeSignatureSuffixContext *> JvmSignatureParser::ClassTypeSignatureContext::classTypeSignatureSuffix() {
  return getRuleContexts<JvmSignatureParser::ClassTypeSignatureSuffixContext>();
}

JvmSignatureParser::ClassTypeSignatureSuffixContext* JvmSignatureParser::ClassTypeSignatureContext::classTypeSignatureSuffix(size_t i) {
  return getRuleContext<JvmSignatureParser::ClassTypeSignatureSuffixContext>(i);
}


size_t JvmSignatureParser::ClassTypeSignatureContext::getRuleIndex() const {
  return JvmSignatureParser::RuleClassTypeSignature;
}

void JvmSignatureParser::ClassTypeSignatureContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JvmSignatureListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterClassTypeSignature(this);
}

void JvmSignatureParser::ClassTypeSignatureContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JvmSignatureListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitClassTypeSignature(this);
}

JvmSignatureParser::ClassTypeSignatureContext* JvmSignatureParser::classTypeSignature() {
  ClassTypeSignatureContext *_localctx = _tracker.createInstance<ClassTypeSignatureContext>(_ctx, getState());
  enterRule(_localctx, 6, JvmSignatureParser::RuleClassTypeSignature);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(59);
    match(JvmSignatureParser::T__8);
    setState(61);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 2, _ctx)) {
    case 1: {
      setState(60);
      packageSpecifier();
      break;
    }

    default:
      break;
    }
    setState(63);
    simpleClassTypeSignature();
    setState(67);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == JvmSignatureParser::T__16) {
      setState(64);
      classTypeSignatureSuffix();
      setState(69);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(70);
    match(JvmSignatureParser::T__9);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PackageSpecifierContext ------------------------------------------------------------------

JvmSignatureParser::PackageSpecifierContext::PackageSpecifierContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* JvmSignatureParser::PackageSpecifierContext::Identifier() {
  return getToken(JvmSignatureParser::Identifier, 0);
}

JvmSignatureParser::PackageSpecifierContext* JvmSignatureParser::PackageSpecifierContext::packageSpecifier() {
  return getRuleContext<JvmSignatureParser::PackageSpecifierContext>(0);
}


size_t JvmSignatureParser::PackageSpecifierContext::getRuleIndex() const {
  return JvmSignatureParser::RulePackageSpecifier;
}

void JvmSignatureParser::PackageSpecifierContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JvmSignatureListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterPackageSpecifier(this);
}

void JvmSignatureParser::PackageSpecifierContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JvmSignatureListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitPackageSpecifier(this);
}

JvmSignatureParser::PackageSpecifierContext* JvmSignatureParser::packageSpecifier() {
  PackageSpecifierContext *_localctx = _tracker.createInstance<PackageSpecifierContext>(_ctx, getState());
  enterRule(_localctx, 8, JvmSignatureParser::RulePackageSpecifier);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(72);
    match(JvmSignatureParser::Identifier);
    setState(73);
    match(JvmSignatureParser::T__10);
    setState(74);
    packageSpecifier();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SimpleClassTypeSignatureContext ------------------------------------------------------------------

JvmSignatureParser::SimpleClassTypeSignatureContext::SimpleClassTypeSignatureContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* JvmSignatureParser::SimpleClassTypeSignatureContext::Identifier() {
  return getToken(JvmSignatureParser::Identifier, 0);
}

JvmSignatureParser::TypeArgumentsContext* JvmSignatureParser::SimpleClassTypeSignatureContext::typeArguments() {
  return getRuleContext<JvmSignatureParser::TypeArgumentsContext>(0);
}


size_t JvmSignatureParser::SimpleClassTypeSignatureContext::getRuleIndex() const {
  return JvmSignatureParser::RuleSimpleClassTypeSignature;
}

void JvmSignatureParser::SimpleClassTypeSignatureContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JvmSignatureListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterSimpleClassTypeSignature(this);
}

void JvmSignatureParser::SimpleClassTypeSignatureContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JvmSignatureListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitSimpleClassTypeSignature(this);
}

JvmSignatureParser::SimpleClassTypeSignatureContext* JvmSignatureParser::simpleClassTypeSignature() {
  SimpleClassTypeSignatureContext *_localctx = _tracker.createInstance<SimpleClassTypeSignatureContext>(_ctx, getState());
  enterRule(_localctx, 10, JvmSignatureParser::RuleSimpleClassTypeSignature);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(76);
    match(JvmSignatureParser::Identifier);
    setState(78);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == JvmSignatureParser::T__11) {
      setState(77);
      typeArguments();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TypeArgumentsContext ------------------------------------------------------------------

JvmSignatureParser::TypeArgumentsContext::TypeArgumentsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<JvmSignatureParser::TypeArgumentContext *> JvmSignatureParser::TypeArgumentsContext::typeArgument() {
  return getRuleContexts<JvmSignatureParser::TypeArgumentContext>();
}

JvmSignatureParser::TypeArgumentContext* JvmSignatureParser::TypeArgumentsContext::typeArgument(size_t i) {
  return getRuleContext<JvmSignatureParser::TypeArgumentContext>(i);
}


size_t JvmSignatureParser::TypeArgumentsContext::getRuleIndex() const {
  return JvmSignatureParser::RuleTypeArguments;
}

void JvmSignatureParser::TypeArgumentsContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JvmSignatureListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterTypeArguments(this);
}

void JvmSignatureParser::TypeArgumentsContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JvmSignatureListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitTypeArguments(this);
}

JvmSignatureParser::TypeArgumentsContext* JvmSignatureParser::typeArguments() {
  TypeArgumentsContext *_localctx = _tracker.createInstance<TypeArgumentsContext>(_ctx, getState());
  enterRule(_localctx, 12, JvmSignatureParser::RuleTypeArguments);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(80);
    match(JvmSignatureParser::T__11);
    setState(81);
    typeArgument();
    setState(85);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 885248) != 0)) {
      setState(82);
      typeArgument();
      setState(87);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(88);
    match(JvmSignatureParser::T__12);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TypeArgumentContext ------------------------------------------------------------------

JvmSignatureParser::TypeArgumentContext::TypeArgumentContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

JvmSignatureParser::ReferenceTypeSignatureContext* JvmSignatureParser::TypeArgumentContext::referenceTypeSignature() {
  return getRuleContext<JvmSignatureParser::ReferenceTypeSignatureContext>(0);
}

JvmSignatureParser::WildcardIndicatorContext* JvmSignatureParser::TypeArgumentContext::wildcardIndicator() {
  return getRuleContext<JvmSignatureParser::WildcardIndicatorContext>(0);
}


size_t JvmSignatureParser::TypeArgumentContext::getRuleIndex() const {
  return JvmSignatureParser::RuleTypeArgument;
}

void JvmSignatureParser::TypeArgumentContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JvmSignatureListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterTypeArgument(this);
}

void JvmSignatureParser::TypeArgumentContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JvmSignatureListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitTypeArgument(this);
}

JvmSignatureParser::TypeArgumentContext* JvmSignatureParser::typeArgument() {
  TypeArgumentContext *_localctx = _tracker.createInstance<TypeArgumentContext>(_ctx, getState());
  enterRule(_localctx, 14, JvmSignatureParser::RuleTypeArgument);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(91);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == JvmSignatureParser::T__14

    || _la == JvmSignatureParser::T__15) {
      setState(90);
      wildcardIndicator();
    }
    setState(93);
    referenceTypeSignature();
    setState(94);
    match(JvmSignatureParser::T__13);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- WildcardIndicatorContext ------------------------------------------------------------------

JvmSignatureParser::WildcardIndicatorContext::WildcardIndicatorContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t JvmSignatureParser::WildcardIndicatorContext::getRuleIndex() const {
  return JvmSignatureParser::RuleWildcardIndicator;
}

void JvmSignatureParser::WildcardIndicatorContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JvmSignatureListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterWildcardIndicator(this);
}

void JvmSignatureParser::WildcardIndicatorContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JvmSignatureListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitWildcardIndicator(this);
}

JvmSignatureParser::WildcardIndicatorContext* JvmSignatureParser::wildcardIndicator() {
  WildcardIndicatorContext *_localctx = _tracker.createInstance<WildcardIndicatorContext>(_ctx, getState());
  enterRule(_localctx, 16, JvmSignatureParser::RuleWildcardIndicator);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(96);
    _la = _input->LA(1);
    if (!(_la == JvmSignatureParser::T__14

    || _la == JvmSignatureParser::T__15)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ClassTypeSignatureSuffixContext ------------------------------------------------------------------

JvmSignatureParser::ClassTypeSignatureSuffixContext::ClassTypeSignatureSuffixContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

JvmSignatureParser::SimpleClassTypeSignatureContext* JvmSignatureParser::ClassTypeSignatureSuffixContext::simpleClassTypeSignature() {
  return getRuleContext<JvmSignatureParser::SimpleClassTypeSignatureContext>(0);
}


size_t JvmSignatureParser::ClassTypeSignatureSuffixContext::getRuleIndex() const {
  return JvmSignatureParser::RuleClassTypeSignatureSuffix;
}

void JvmSignatureParser::ClassTypeSignatureSuffixContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JvmSignatureListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterClassTypeSignatureSuffix(this);
}

void JvmSignatureParser::ClassTypeSignatureSuffixContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JvmSignatureListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitClassTypeSignatureSuffix(this);
}

JvmSignatureParser::ClassTypeSignatureSuffixContext* JvmSignatureParser::classTypeSignatureSuffix() {
  ClassTypeSignatureSuffixContext *_localctx = _tracker.createInstance<ClassTypeSignatureSuffixContext>(_ctx, getState());
  enterRule(_localctx, 18, JvmSignatureParser::RuleClassTypeSignatureSuffix);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(98);
    match(JvmSignatureParser::T__16);
    setState(99);
    simpleClassTypeSignature();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TypeVariableSignatureContext ------------------------------------------------------------------

JvmSignatureParser::TypeVariableSignatureContext::TypeVariableSignatureContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* JvmSignatureParser::TypeVariableSignatureContext::Identifier() {
  return getToken(JvmSignatureParser::Identifier, 0);
}


size_t JvmSignatureParser::TypeVariableSignatureContext::getRuleIndex() const {
  return JvmSignatureParser::RuleTypeVariableSignature;
}

void JvmSignatureParser::TypeVariableSignatureContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JvmSignatureListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterTypeVariableSignature(this);
}

void JvmSignatureParser::TypeVariableSignatureContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JvmSignatureListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitTypeVariableSignature(this);
}

JvmSignatureParser::TypeVariableSignatureContext* JvmSignatureParser::typeVariableSignature() {
  TypeVariableSignatureContext *_localctx = _tracker.createInstance<TypeVariableSignatureContext>(_ctx, getState());
  enterRule(_localctx, 20, JvmSignatureParser::RuleTypeVariableSignature);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(101);
    match(JvmSignatureParser::T__17);
    setState(102);
    match(JvmSignatureParser::Identifier);
    setState(103);
    match(JvmSignatureParser::T__9);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ArrayTypeSignatureContext ------------------------------------------------------------------

JvmSignatureParser::ArrayTypeSignatureContext::ArrayTypeSignatureContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

JvmSignatureParser::JavaTypeSignatureContext* JvmSignatureParser::ArrayTypeSignatureContext::javaTypeSignature() {
  return getRuleContext<JvmSignatureParser::JavaTypeSignatureContext>(0);
}


size_t JvmSignatureParser::ArrayTypeSignatureContext::getRuleIndex() const {
  return JvmSignatureParser::RuleArrayTypeSignature;
}

void JvmSignatureParser::ArrayTypeSignatureContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JvmSignatureListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterArrayTypeSignature(this);
}

void JvmSignatureParser::ArrayTypeSignatureContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JvmSignatureListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitArrayTypeSignature(this);
}

JvmSignatureParser::ArrayTypeSignatureContext* JvmSignatureParser::arrayTypeSignature() {
  ArrayTypeSignatureContext *_localctx = _tracker.createInstance<ArrayTypeSignatureContext>(_ctx, getState());
  enterRule(_localctx, 22, JvmSignatureParser::RuleArrayTypeSignature);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(105);
    match(JvmSignatureParser::T__18);
    setState(106);
    javaTypeSignature();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ClassSignatureContext ------------------------------------------------------------------

JvmSignatureParser::ClassSignatureContext::ClassSignatureContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

JvmSignatureParser::SuperclassSignatureContext* JvmSignatureParser::ClassSignatureContext::superclassSignature() {
  return getRuleContext<JvmSignatureParser::SuperclassSignatureContext>(0);
}

JvmSignatureParser::TypeParametersContext* JvmSignatureParser::ClassSignatureContext::typeParameters() {
  return getRuleContext<JvmSignatureParser::TypeParametersContext>(0);
}

std::vector<JvmSignatureParser::SuperInterfaceSignatureContext *> JvmSignatureParser::ClassSignatureContext::superInterfaceSignature() {
  return getRuleContexts<JvmSignatureParser::SuperInterfaceSignatureContext>();
}

JvmSignatureParser::SuperInterfaceSignatureContext* JvmSignatureParser::ClassSignatureContext::superInterfaceSignature(size_t i) {
  return getRuleContext<JvmSignatureParser::SuperInterfaceSignatureContext>(i);
}


size_t JvmSignatureParser::ClassSignatureContext::getRuleIndex() const {
  return JvmSignatureParser::RuleClassSignature;
}

void JvmSignatureParser::ClassSignatureContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JvmSignatureListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterClassSignature(this);
}

void JvmSignatureParser::ClassSignatureContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JvmSignatureListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitClassSignature(this);
}

JvmSignatureParser::ClassSignatureContext* JvmSignatureParser::classSignature() {
  ClassSignatureContext *_localctx = _tracker.createInstance<ClassSignatureContext>(_ctx, getState());
  enterRule(_localctx, 24, JvmSignatureParser::RuleClassSignature);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(109);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == JvmSignatureParser::T__11) {
      setState(108);
      typeParameters();
    }
    setState(111);
    superclassSignature();
    setState(115);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == JvmSignatureParser::T__8) {
      setState(112);
      superInterfaceSignature();
      setState(117);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TypeParametersContext ------------------------------------------------------------------

JvmSignatureParser::TypeParametersContext::TypeParametersContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<JvmSignatureParser::TypeParameterContext *> JvmSignatureParser::TypeParametersContext::typeParameter() {
  return getRuleContexts<JvmSignatureParser::TypeParameterContext>();
}

JvmSignatureParser::TypeParameterContext* JvmSignatureParser::TypeParametersContext::typeParameter(size_t i) {
  return getRuleContext<JvmSignatureParser::TypeParameterContext>(i);
}


size_t JvmSignatureParser::TypeParametersContext::getRuleIndex() const {
  return JvmSignatureParser::RuleTypeParameters;
}

void JvmSignatureParser::TypeParametersContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JvmSignatureListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterTypeParameters(this);
}

void JvmSignatureParser::TypeParametersContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JvmSignatureListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitTypeParameters(this);
}

JvmSignatureParser::TypeParametersContext* JvmSignatureParser::typeParameters() {
  TypeParametersContext *_localctx = _tracker.createInstance<TypeParametersContext>(_ctx, getState());
  enterRule(_localctx, 26, JvmSignatureParser::RuleTypeParameters);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(118);
    match(JvmSignatureParser::T__11);
    setState(119);
    typeParameter();
    setState(123);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == JvmSignatureParser::Identifier) {
      setState(120);
      typeParameter();
      setState(125);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(126);
    match(JvmSignatureParser::T__12);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TypeParameterContext ------------------------------------------------------------------

JvmSignatureParser::TypeParameterContext::TypeParameterContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* JvmSignatureParser::TypeParameterContext::Identifier() {
  return getToken(JvmSignatureParser::Identifier, 0);
}

JvmSignatureParser::ClassBoundContext* JvmSignatureParser::TypeParameterContext::classBound() {
  return getRuleContext<JvmSignatureParser::ClassBoundContext>(0);
}

std::vector<JvmSignatureParser::InterfaceBoundContext *> JvmSignatureParser::TypeParameterContext::interfaceBound() {
  return getRuleContexts<JvmSignatureParser::InterfaceBoundContext>();
}

JvmSignatureParser::InterfaceBoundContext* JvmSignatureParser::TypeParameterContext::interfaceBound(size_t i) {
  return getRuleContext<JvmSignatureParser::InterfaceBoundContext>(i);
}


size_t JvmSignatureParser::TypeParameterContext::getRuleIndex() const {
  return JvmSignatureParser::RuleTypeParameter;
}

void JvmSignatureParser::TypeParameterContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JvmSignatureListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterTypeParameter(this);
}

void JvmSignatureParser::TypeParameterContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JvmSignatureListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitTypeParameter(this);
}

JvmSignatureParser::TypeParameterContext* JvmSignatureParser::typeParameter() {
  TypeParameterContext *_localctx = _tracker.createInstance<TypeParameterContext>(_ctx, getState());
  enterRule(_localctx, 28, JvmSignatureParser::RuleTypeParameter);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(128);
    match(JvmSignatureParser::Identifier);
    setState(129);
    classBound();
    setState(133);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 786944) != 0)) {
      setState(130);
      interfaceBound();
      setState(135);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ClassBoundContext ------------------------------------------------------------------

JvmSignatureParser::ClassBoundContext::ClassBoundContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

JvmSignatureParser::ReferenceTypeSignatureContext* JvmSignatureParser::ClassBoundContext::referenceTypeSignature() {
  return getRuleContext<JvmSignatureParser::ReferenceTypeSignatureContext>(0);
}


size_t JvmSignatureParser::ClassBoundContext::getRuleIndex() const {
  return JvmSignatureParser::RuleClassBound;
}

void JvmSignatureParser::ClassBoundContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JvmSignatureListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterClassBound(this);
}

void JvmSignatureParser::ClassBoundContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JvmSignatureListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitClassBound(this);
}

JvmSignatureParser::ClassBoundContext* JvmSignatureParser::classBound() {
  ClassBoundContext *_localctx = _tracker.createInstance<ClassBoundContext>(_ctx, getState());
  enterRule(_localctx, 30, JvmSignatureParser::RuleClassBound);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(137);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 11, _ctx)) {
    case 1: {
      setState(136);
      referenceTypeSignature();
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SuperclassSignatureContext ------------------------------------------------------------------

JvmSignatureParser::SuperclassSignatureContext::SuperclassSignatureContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

JvmSignatureParser::ClassTypeSignatureContext* JvmSignatureParser::SuperclassSignatureContext::classTypeSignature() {
  return getRuleContext<JvmSignatureParser::ClassTypeSignatureContext>(0);
}


size_t JvmSignatureParser::SuperclassSignatureContext::getRuleIndex() const {
  return JvmSignatureParser::RuleSuperclassSignature;
}

void JvmSignatureParser::SuperclassSignatureContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JvmSignatureListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterSuperclassSignature(this);
}

void JvmSignatureParser::SuperclassSignatureContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JvmSignatureListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitSuperclassSignature(this);
}

JvmSignatureParser::SuperclassSignatureContext* JvmSignatureParser::superclassSignature() {
  SuperclassSignatureContext *_localctx = _tracker.createInstance<SuperclassSignatureContext>(_ctx, getState());
  enterRule(_localctx, 32, JvmSignatureParser::RuleSuperclassSignature);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(139);
    match(JvmSignatureParser::T__8);
    setState(140);
    classTypeSignature();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SuperInterfaceSignatureContext ------------------------------------------------------------------

JvmSignatureParser::SuperInterfaceSignatureContext::SuperInterfaceSignatureContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

JvmSignatureParser::ClassTypeSignatureContext* JvmSignatureParser::SuperInterfaceSignatureContext::classTypeSignature() {
  return getRuleContext<JvmSignatureParser::ClassTypeSignatureContext>(0);
}


size_t JvmSignatureParser::SuperInterfaceSignatureContext::getRuleIndex() const {
  return JvmSignatureParser::RuleSuperInterfaceSignature;
}

void JvmSignatureParser::SuperInterfaceSignatureContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JvmSignatureListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterSuperInterfaceSignature(this);
}

void JvmSignatureParser::SuperInterfaceSignatureContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JvmSignatureListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitSuperInterfaceSignature(this);
}

JvmSignatureParser::SuperInterfaceSignatureContext* JvmSignatureParser::superInterfaceSignature() {
  SuperInterfaceSignatureContext *_localctx = _tracker.createInstance<SuperInterfaceSignatureContext>(_ctx, getState());
  enterRule(_localctx, 34, JvmSignatureParser::RuleSuperInterfaceSignature);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(142);
    match(JvmSignatureParser::T__8);
    setState(143);
    classTypeSignature();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- InterfaceBoundContext ------------------------------------------------------------------

JvmSignatureParser::InterfaceBoundContext::InterfaceBoundContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

JvmSignatureParser::ReferenceTypeSignatureContext* JvmSignatureParser::InterfaceBoundContext::referenceTypeSignature() {
  return getRuleContext<JvmSignatureParser::ReferenceTypeSignatureContext>(0);
}


size_t JvmSignatureParser::InterfaceBoundContext::getRuleIndex() const {
  return JvmSignatureParser::RuleInterfaceBound;
}

void JvmSignatureParser::InterfaceBoundContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JvmSignatureListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterInterfaceBound(this);
}

void JvmSignatureParser::InterfaceBoundContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JvmSignatureListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitInterfaceBound(this);
}

JvmSignatureParser::InterfaceBoundContext* JvmSignatureParser::interfaceBound() {
  InterfaceBoundContext *_localctx = _tracker.createInstance<InterfaceBoundContext>(_ctx, getState());
  enterRule(_localctx, 36, JvmSignatureParser::RuleInterfaceBound);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(145);
    referenceTypeSignature();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- MethodSignatureContext ------------------------------------------------------------------

JvmSignatureParser::MethodSignatureContext::MethodSignatureContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

JvmSignatureParser::ResultContext* JvmSignatureParser::MethodSignatureContext::result() {
  return getRuleContext<JvmSignatureParser::ResultContext>(0);
}

JvmSignatureParser::TypeParameterContext* JvmSignatureParser::MethodSignatureContext::typeParameter() {
  return getRuleContext<JvmSignatureParser::TypeParameterContext>(0);
}

std::vector<JvmSignatureParser::JavaTypeSignatureContext *> JvmSignatureParser::MethodSignatureContext::javaTypeSignature() {
  return getRuleContexts<JvmSignatureParser::JavaTypeSignatureContext>();
}

JvmSignatureParser::JavaTypeSignatureContext* JvmSignatureParser::MethodSignatureContext::javaTypeSignature(size_t i) {
  return getRuleContext<JvmSignatureParser::JavaTypeSignatureContext>(i);
}

std::vector<JvmSignatureParser::ThrowsSignatureContext *> JvmSignatureParser::MethodSignatureContext::throwsSignature() {
  return getRuleContexts<JvmSignatureParser::ThrowsSignatureContext>();
}

JvmSignatureParser::ThrowsSignatureContext* JvmSignatureParser::MethodSignatureContext::throwsSignature(size_t i) {
  return getRuleContext<JvmSignatureParser::ThrowsSignatureContext>(i);
}


size_t JvmSignatureParser::MethodSignatureContext::getRuleIndex() const {
  return JvmSignatureParser::RuleMethodSignature;
}

void JvmSignatureParser::MethodSignatureContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JvmSignatureListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterMethodSignature(this);
}

void JvmSignatureParser::MethodSignatureContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JvmSignatureListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitMethodSignature(this);
}

JvmSignatureParser::MethodSignatureContext* JvmSignatureParser::methodSignature() {
  MethodSignatureContext *_localctx = _tracker.createInstance<MethodSignatureContext>(_ctx, getState());
  enterRule(_localctx, 38, JvmSignatureParser::RuleMethodSignature);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(148);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == JvmSignatureParser::Identifier) {
      setState(147);
      typeParameter();
    }
    setState(150);
    match(JvmSignatureParser::T__19);
    setState(154);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 787454) != 0)) {
      setState(151);
      javaTypeSignature();
      setState(156);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(157);
    match(JvmSignatureParser::T__20);
    setState(158);
    result();
    setState(162);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == JvmSignatureParser::T__21) {
      setState(159);
      throwsSignature();
      setState(164);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ResultContext ------------------------------------------------------------------

JvmSignatureParser::ResultContext::ResultContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

JvmSignatureParser::JavaTypeSignatureContext* JvmSignatureParser::ResultContext::javaTypeSignature() {
  return getRuleContext<JvmSignatureParser::JavaTypeSignatureContext>(0);
}

JvmSignatureParser::VoidDescriptorContext* JvmSignatureParser::ResultContext::voidDescriptor() {
  return getRuleContext<JvmSignatureParser::VoidDescriptorContext>(0);
}


size_t JvmSignatureParser::ResultContext::getRuleIndex() const {
  return JvmSignatureParser::RuleResult;
}

void JvmSignatureParser::ResultContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JvmSignatureListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterResult(this);
}

void JvmSignatureParser::ResultContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JvmSignatureListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitResult(this);
}

JvmSignatureParser::ResultContext* JvmSignatureParser::result() {
  ResultContext *_localctx = _tracker.createInstance<ResultContext>(_ctx, getState());
  enterRule(_localctx, 40, JvmSignatureParser::RuleResult);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(167);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case JvmSignatureParser::T__0:
      case JvmSignatureParser::T__1:
      case JvmSignatureParser::T__2:
      case JvmSignatureParser::T__3:
      case JvmSignatureParser::T__4:
      case JvmSignatureParser::T__5:
      case JvmSignatureParser::T__6:
      case JvmSignatureParser::T__7:
      case JvmSignatureParser::T__8:
      case JvmSignatureParser::T__17:
      case JvmSignatureParser::T__18: {
        enterOuterAlt(_localctx, 1);
        setState(165);
        javaTypeSignature();
        break;
      }

      case JvmSignatureParser::T__22: {
        enterOuterAlt(_localctx, 2);
        setState(166);
        voidDescriptor();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ThrowsSignatureContext ------------------------------------------------------------------

JvmSignatureParser::ThrowsSignatureContext::ThrowsSignatureContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

JvmSignatureParser::ClassTypeSignatureContext* JvmSignatureParser::ThrowsSignatureContext::classTypeSignature() {
  return getRuleContext<JvmSignatureParser::ClassTypeSignatureContext>(0);
}

JvmSignatureParser::TypeVariableSignatureContext* JvmSignatureParser::ThrowsSignatureContext::typeVariableSignature() {
  return getRuleContext<JvmSignatureParser::TypeVariableSignatureContext>(0);
}


size_t JvmSignatureParser::ThrowsSignatureContext::getRuleIndex() const {
  return JvmSignatureParser::RuleThrowsSignature;
}

void JvmSignatureParser::ThrowsSignatureContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JvmSignatureListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterThrowsSignature(this);
}

void JvmSignatureParser::ThrowsSignatureContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JvmSignatureListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitThrowsSignature(this);
}

JvmSignatureParser::ThrowsSignatureContext* JvmSignatureParser::throwsSignature() {
  ThrowsSignatureContext *_localctx = _tracker.createInstance<ThrowsSignatureContext>(_ctx, getState());
  enterRule(_localctx, 42, JvmSignatureParser::RuleThrowsSignature);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(169);
    match(JvmSignatureParser::T__21);
    setState(170);
    classTypeSignature();
    setState(171);
    match(JvmSignatureParser::T__21);
    setState(172);
    typeVariableSignature();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- VoidDescriptorContext ------------------------------------------------------------------

JvmSignatureParser::VoidDescriptorContext::VoidDescriptorContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t JvmSignatureParser::VoidDescriptorContext::getRuleIndex() const {
  return JvmSignatureParser::RuleVoidDescriptor;
}

void JvmSignatureParser::VoidDescriptorContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JvmSignatureListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterVoidDescriptor(this);
}

void JvmSignatureParser::VoidDescriptorContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JvmSignatureListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitVoidDescriptor(this);
}

JvmSignatureParser::VoidDescriptorContext* JvmSignatureParser::voidDescriptor() {
  VoidDescriptorContext *_localctx = _tracker.createInstance<VoidDescriptorContext>(_ctx, getState());
  enterRule(_localctx, 44, JvmSignatureParser::RuleVoidDescriptor);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(174);
    match(JvmSignatureParser::T__22);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FieldSignatureContext ------------------------------------------------------------------

JvmSignatureParser::FieldSignatureContext::FieldSignatureContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

JvmSignatureParser::ReferenceTypeSignatureContext* JvmSignatureParser::FieldSignatureContext::referenceTypeSignature() {
  return getRuleContext<JvmSignatureParser::ReferenceTypeSignatureContext>(0);
}


size_t JvmSignatureParser::FieldSignatureContext::getRuleIndex() const {
  return JvmSignatureParser::RuleFieldSignature;
}

void JvmSignatureParser::FieldSignatureContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JvmSignatureListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFieldSignature(this);
}

void JvmSignatureParser::FieldSignatureContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JvmSignatureListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFieldSignature(this);
}

JvmSignatureParser::FieldSignatureContext* JvmSignatureParser::fieldSignature() {
  FieldSignatureContext *_localctx = _tracker.createInstance<FieldSignatureContext>(_ctx, getState());
  enterRule(_localctx, 46, JvmSignatureParser::RuleFieldSignature);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(176);
    referenceTypeSignature();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

void JvmSignatureParser::initialize() {
  ::antlr4::internal::call_once(jvmsignatureParserOnceFlag, jvmsignatureParserInitialize);
}
