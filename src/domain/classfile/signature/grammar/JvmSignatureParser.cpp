
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
      "classSignature", "simpleClassTypeSignature", "superclassSignature", 
      "interfaceSignature", "typeArguments", "typeArgument", "wildcardIndicator", 
      "wildcardBound", "typeSignature", "classTypeSignature", "fieldSignature", 
      "methodSignature", "packageSpecifier", "arrayTypeSignature", "fieldTypeSignature", 
      "methodTypeSignature", "parameterTypeSignature", "returnType", "throwsSignature", 
      "primitiveType"
    },
    std::vector<std::string>{
      "", "'L'", "';'", "'<'", "'>'", "'\\u003F'", "'extends'", "'super'", 
      "'.'", "'/'", "'['", "'('", "')'", "'V'", "'^'", "'B'", "'C'", "'D'", 
      "'F'", "'I'", "'J'", "'S'", "'Z'"
    },
    std::vector<std::string>{
      "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
      "", "", "", "", "", "", "Identifier", "WS"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,1,24,168,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,6,2,
  	7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,2,14,7,
  	14,2,15,7,15,2,16,7,16,2,17,7,17,2,18,7,18,2,19,7,19,1,0,1,0,3,0,43,8,
  	0,1,0,1,0,3,0,47,8,0,1,0,5,0,50,8,0,10,0,12,0,53,9,0,1,0,5,0,56,8,0,10,
  	0,12,0,59,9,0,1,0,5,0,62,8,0,10,0,12,0,65,9,0,1,0,1,0,1,1,1,1,3,1,71,
  	8,1,1,2,1,2,1,2,1,2,1,3,1,3,1,3,1,3,1,4,1,4,5,4,83,8,4,10,4,12,4,86,9,
  	4,1,4,1,4,1,5,3,5,91,8,5,1,5,1,5,1,6,1,6,3,6,97,8,6,1,7,1,7,1,7,1,7,3,
  	7,103,8,7,1,8,1,8,1,8,3,8,108,8,8,1,9,1,9,3,9,112,8,9,1,9,1,9,3,9,116,
  	8,9,1,9,1,9,1,9,3,9,121,8,9,5,9,123,8,9,10,9,12,9,126,9,9,1,9,1,9,1,10,
  	1,10,1,11,1,11,1,12,1,12,1,12,1,13,1,13,1,13,1,14,1,14,1,15,1,15,5,15,
  	144,8,15,10,15,12,15,147,9,15,1,15,1,15,1,15,5,15,152,8,15,10,15,12,15,
  	155,9,15,1,16,1,16,1,17,1,17,3,17,161,8,17,1,18,1,18,1,18,1,19,1,19,1,
  	19,0,0,20,0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,34,36,38,0,1,
  	1,0,15,22,166,0,40,1,0,0,0,2,68,1,0,0,0,4,72,1,0,0,0,6,76,1,0,0,0,8,80,
  	1,0,0,0,10,90,1,0,0,0,12,94,1,0,0,0,14,102,1,0,0,0,16,107,1,0,0,0,18,
  	109,1,0,0,0,20,129,1,0,0,0,22,131,1,0,0,0,24,133,1,0,0,0,26,136,1,0,0,
  	0,28,139,1,0,0,0,30,141,1,0,0,0,32,156,1,0,0,0,34,160,1,0,0,0,36,162,
  	1,0,0,0,38,165,1,0,0,0,40,42,5,1,0,0,41,43,3,24,12,0,42,41,1,0,0,0,42,
  	43,1,0,0,0,43,44,1,0,0,0,44,46,3,2,1,0,45,47,3,4,2,0,46,45,1,0,0,0,46,
  	47,1,0,0,0,47,51,1,0,0,0,48,50,3,6,3,0,49,48,1,0,0,0,50,53,1,0,0,0,51,
  	49,1,0,0,0,51,52,1,0,0,0,52,57,1,0,0,0,53,51,1,0,0,0,54,56,3,20,10,0,
  	55,54,1,0,0,0,56,59,1,0,0,0,57,55,1,0,0,0,57,58,1,0,0,0,58,63,1,0,0,0,
  	59,57,1,0,0,0,60,62,3,22,11,0,61,60,1,0,0,0,62,65,1,0,0,0,63,61,1,0,0,
  	0,63,64,1,0,0,0,64,66,1,0,0,0,65,63,1,0,0,0,66,67,5,2,0,0,67,1,1,0,0,
  	0,68,70,5,23,0,0,69,71,3,8,4,0,70,69,1,0,0,0,70,71,1,0,0,0,71,3,1,0,0,
  	0,72,73,5,1,0,0,73,74,3,18,9,0,74,75,5,2,0,0,75,5,1,0,0,0,76,77,5,1,0,
  	0,77,78,3,18,9,0,78,79,5,2,0,0,79,7,1,0,0,0,80,84,5,3,0,0,81,83,3,10,
  	5,0,82,81,1,0,0,0,83,86,1,0,0,0,84,82,1,0,0,0,84,85,1,0,0,0,85,87,1,0,
  	0,0,86,84,1,0,0,0,87,88,5,4,0,0,88,9,1,0,0,0,89,91,3,12,6,0,90,89,1,0,
  	0,0,90,91,1,0,0,0,91,92,1,0,0,0,92,93,3,16,8,0,93,11,1,0,0,0,94,96,5,
  	5,0,0,95,97,3,14,7,0,96,95,1,0,0,0,96,97,1,0,0,0,97,13,1,0,0,0,98,99,
  	5,6,0,0,99,103,3,16,8,0,100,101,5,7,0,0,101,103,3,16,8,0,102,98,1,0,0,
  	0,102,100,1,0,0,0,103,15,1,0,0,0,104,108,3,18,9,0,105,108,3,26,13,0,106,
  	108,3,38,19,0,107,104,1,0,0,0,107,105,1,0,0,0,107,106,1,0,0,0,108,17,
  	1,0,0,0,109,111,5,1,0,0,110,112,3,24,12,0,111,110,1,0,0,0,111,112,1,0,
  	0,0,112,113,1,0,0,0,113,115,5,23,0,0,114,116,3,8,4,0,115,114,1,0,0,0,
  	115,116,1,0,0,0,116,124,1,0,0,0,117,118,5,8,0,0,118,120,5,23,0,0,119,
  	121,3,8,4,0,120,119,1,0,0,0,120,121,1,0,0,0,121,123,1,0,0,0,122,117,1,
  	0,0,0,123,126,1,0,0,0,124,122,1,0,0,0,124,125,1,0,0,0,125,127,1,0,0,0,
  	126,124,1,0,0,0,127,128,5,2,0,0,128,19,1,0,0,0,129,130,3,28,14,0,130,
  	21,1,0,0,0,131,132,3,30,15,0,132,23,1,0,0,0,133,134,5,23,0,0,134,135,
  	5,9,0,0,135,25,1,0,0,0,136,137,5,10,0,0,137,138,3,16,8,0,138,27,1,0,0,
  	0,139,140,3,16,8,0,140,29,1,0,0,0,141,145,5,11,0,0,142,144,3,32,16,0,
  	143,142,1,0,0,0,144,147,1,0,0,0,145,143,1,0,0,0,145,146,1,0,0,0,146,148,
  	1,0,0,0,147,145,1,0,0,0,148,149,5,12,0,0,149,153,3,34,17,0,150,152,3,
  	36,18,0,151,150,1,0,0,0,152,155,1,0,0,0,153,151,1,0,0,0,153,154,1,0,0,
  	0,154,31,1,0,0,0,155,153,1,0,0,0,156,157,3,16,8,0,157,33,1,0,0,0,158,
  	161,3,16,8,0,159,161,5,13,0,0,160,158,1,0,0,0,160,159,1,0,0,0,161,35,
  	1,0,0,0,162,163,5,14,0,0,163,164,3,18,9,0,164,37,1,0,0,0,165,166,7,0,
  	0,0,166,39,1,0,0,0,18,42,46,51,57,63,70,84,90,96,102,107,111,115,120,
  	124,145,153,160
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


//----------------- ClassSignatureContext ------------------------------------------------------------------

JvmSignatureParser::ClassSignatureContext::ClassSignatureContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

JvmSignatureParser::SimpleClassTypeSignatureContext* JvmSignatureParser::ClassSignatureContext::simpleClassTypeSignature() {
  return getRuleContext<JvmSignatureParser::SimpleClassTypeSignatureContext>(0);
}

JvmSignatureParser::PackageSpecifierContext* JvmSignatureParser::ClassSignatureContext::packageSpecifier() {
  return getRuleContext<JvmSignatureParser::PackageSpecifierContext>(0);
}

JvmSignatureParser::SuperclassSignatureContext* JvmSignatureParser::ClassSignatureContext::superclassSignature() {
  return getRuleContext<JvmSignatureParser::SuperclassSignatureContext>(0);
}

std::vector<JvmSignatureParser::InterfaceSignatureContext *> JvmSignatureParser::ClassSignatureContext::interfaceSignature() {
  return getRuleContexts<JvmSignatureParser::InterfaceSignatureContext>();
}

JvmSignatureParser::InterfaceSignatureContext* JvmSignatureParser::ClassSignatureContext::interfaceSignature(size_t i) {
  return getRuleContext<JvmSignatureParser::InterfaceSignatureContext>(i);
}

std::vector<JvmSignatureParser::FieldSignatureContext *> JvmSignatureParser::ClassSignatureContext::fieldSignature() {
  return getRuleContexts<JvmSignatureParser::FieldSignatureContext>();
}

JvmSignatureParser::FieldSignatureContext* JvmSignatureParser::ClassSignatureContext::fieldSignature(size_t i) {
  return getRuleContext<JvmSignatureParser::FieldSignatureContext>(i);
}

std::vector<JvmSignatureParser::MethodSignatureContext *> JvmSignatureParser::ClassSignatureContext::methodSignature() {
  return getRuleContexts<JvmSignatureParser::MethodSignatureContext>();
}

JvmSignatureParser::MethodSignatureContext* JvmSignatureParser::ClassSignatureContext::methodSignature(size_t i) {
  return getRuleContext<JvmSignatureParser::MethodSignatureContext>(i);
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
  enterRule(_localctx, 0, JvmSignatureParser::RuleClassSignature);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(40);
    match(JvmSignatureParser::T__0);
    setState(42);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 0, _ctx)) {
    case 1: {
      setState(41);
      packageSpecifier();
      break;
    }

    default:
      break;
    }
    setState(44);
    simpleClassTypeSignature();
    setState(46);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 1, _ctx)) {
    case 1: {
      setState(45);
      superclassSignature();
      break;
    }

    default:
      break;
    }
    setState(51);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 2, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(48);
        interfaceSignature(); 
      }
      setState(53);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 2, _ctx);
    }
    setState(57);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 8356866) != 0)) {
      setState(54);
      fieldSignature();
      setState(59);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(63);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == JvmSignatureParser::T__10) {
      setState(60);
      methodSignature();
      setState(65);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(66);
    match(JvmSignatureParser::T__1);
   
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
  enterRule(_localctx, 2, JvmSignatureParser::RuleSimpleClassTypeSignature);
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
    setState(68);
    match(JvmSignatureParser::Identifier);
    setState(70);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == JvmSignatureParser::T__2) {
      setState(69);
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
  enterRule(_localctx, 4, JvmSignatureParser::RuleSuperclassSignature);

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
    match(JvmSignatureParser::T__0);
    setState(73);
    classTypeSignature();
    setState(74);
    match(JvmSignatureParser::T__1);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- InterfaceSignatureContext ------------------------------------------------------------------

JvmSignatureParser::InterfaceSignatureContext::InterfaceSignatureContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

JvmSignatureParser::ClassTypeSignatureContext* JvmSignatureParser::InterfaceSignatureContext::classTypeSignature() {
  return getRuleContext<JvmSignatureParser::ClassTypeSignatureContext>(0);
}


size_t JvmSignatureParser::InterfaceSignatureContext::getRuleIndex() const {
  return JvmSignatureParser::RuleInterfaceSignature;
}

void JvmSignatureParser::InterfaceSignatureContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JvmSignatureListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterInterfaceSignature(this);
}

void JvmSignatureParser::InterfaceSignatureContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JvmSignatureListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitInterfaceSignature(this);
}

JvmSignatureParser::InterfaceSignatureContext* JvmSignatureParser::interfaceSignature() {
  InterfaceSignatureContext *_localctx = _tracker.createInstance<InterfaceSignatureContext>(_ctx, getState());
  enterRule(_localctx, 6, JvmSignatureParser::RuleInterfaceSignature);

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
    match(JvmSignatureParser::T__0);
    setState(77);
    classTypeSignature();
    setState(78);
    match(JvmSignatureParser::T__1);
   
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
  enterRule(_localctx, 8, JvmSignatureParser::RuleTypeArguments);
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
    match(JvmSignatureParser::T__2);
    setState(84);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 8356898) != 0)) {
      setState(81);
      typeArgument();
      setState(86);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(87);
    match(JvmSignatureParser::T__3);
   
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

JvmSignatureParser::TypeSignatureContext* JvmSignatureParser::TypeArgumentContext::typeSignature() {
  return getRuleContext<JvmSignatureParser::TypeSignatureContext>(0);
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
  enterRule(_localctx, 10, JvmSignatureParser::RuleTypeArgument);
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
    setState(90);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == JvmSignatureParser::T__4) {
      setState(89);
      wildcardIndicator();
    }
    setState(92);
    typeSignature();
   
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

JvmSignatureParser::WildcardBoundContext* JvmSignatureParser::WildcardIndicatorContext::wildcardBound() {
  return getRuleContext<JvmSignatureParser::WildcardBoundContext>(0);
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
  enterRule(_localctx, 12, JvmSignatureParser::RuleWildcardIndicator);
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
    setState(94);
    match(JvmSignatureParser::T__4);
    setState(96);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == JvmSignatureParser::T__5

    || _la == JvmSignatureParser::T__6) {
      setState(95);
      wildcardBound();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- WildcardBoundContext ------------------------------------------------------------------

JvmSignatureParser::WildcardBoundContext::WildcardBoundContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

JvmSignatureParser::TypeSignatureContext* JvmSignatureParser::WildcardBoundContext::typeSignature() {
  return getRuleContext<JvmSignatureParser::TypeSignatureContext>(0);
}


size_t JvmSignatureParser::WildcardBoundContext::getRuleIndex() const {
  return JvmSignatureParser::RuleWildcardBound;
}

void JvmSignatureParser::WildcardBoundContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JvmSignatureListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterWildcardBound(this);
}

void JvmSignatureParser::WildcardBoundContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JvmSignatureListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitWildcardBound(this);
}

JvmSignatureParser::WildcardBoundContext* JvmSignatureParser::wildcardBound() {
  WildcardBoundContext *_localctx = _tracker.createInstance<WildcardBoundContext>(_ctx, getState());
  enterRule(_localctx, 14, JvmSignatureParser::RuleWildcardBound);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(102);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case JvmSignatureParser::T__5: {
        enterOuterAlt(_localctx, 1);
        setState(98);
        match(JvmSignatureParser::T__5);
        setState(99);
        typeSignature();
        break;
      }

      case JvmSignatureParser::T__6: {
        enterOuterAlt(_localctx, 2);
        setState(100);
        match(JvmSignatureParser::T__6);
        setState(101);
        typeSignature();
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

//----------------- TypeSignatureContext ------------------------------------------------------------------

JvmSignatureParser::TypeSignatureContext::TypeSignatureContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

JvmSignatureParser::ClassTypeSignatureContext* JvmSignatureParser::TypeSignatureContext::classTypeSignature() {
  return getRuleContext<JvmSignatureParser::ClassTypeSignatureContext>(0);
}

JvmSignatureParser::ArrayTypeSignatureContext* JvmSignatureParser::TypeSignatureContext::arrayTypeSignature() {
  return getRuleContext<JvmSignatureParser::ArrayTypeSignatureContext>(0);
}

JvmSignatureParser::PrimitiveTypeContext* JvmSignatureParser::TypeSignatureContext::primitiveType() {
  return getRuleContext<JvmSignatureParser::PrimitiveTypeContext>(0);
}


size_t JvmSignatureParser::TypeSignatureContext::getRuleIndex() const {
  return JvmSignatureParser::RuleTypeSignature;
}

void JvmSignatureParser::TypeSignatureContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JvmSignatureListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterTypeSignature(this);
}

void JvmSignatureParser::TypeSignatureContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JvmSignatureListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitTypeSignature(this);
}

JvmSignatureParser::TypeSignatureContext* JvmSignatureParser::typeSignature() {
  TypeSignatureContext *_localctx = _tracker.createInstance<TypeSignatureContext>(_ctx, getState());
  enterRule(_localctx, 16, JvmSignatureParser::RuleTypeSignature);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(107);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case JvmSignatureParser::T__0: {
        enterOuterAlt(_localctx, 1);
        setState(104);
        classTypeSignature();
        break;
      }

      case JvmSignatureParser::T__9: {
        enterOuterAlt(_localctx, 2);
        setState(105);
        arrayTypeSignature();
        break;
      }

      case JvmSignatureParser::T__14:
      case JvmSignatureParser::T__15:
      case JvmSignatureParser::T__16:
      case JvmSignatureParser::T__17:
      case JvmSignatureParser::T__18:
      case JvmSignatureParser::T__19:
      case JvmSignatureParser::T__20:
      case JvmSignatureParser::T__21: {
        enterOuterAlt(_localctx, 3);
        setState(106);
        primitiveType();
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

std::vector<tree::TerminalNode *> JvmSignatureParser::ClassTypeSignatureContext::Identifier() {
  return getTokens(JvmSignatureParser::Identifier);
}

tree::TerminalNode* JvmSignatureParser::ClassTypeSignatureContext::Identifier(size_t i) {
  return getToken(JvmSignatureParser::Identifier, i);
}

JvmSignatureParser::PackageSpecifierContext* JvmSignatureParser::ClassTypeSignatureContext::packageSpecifier() {
  return getRuleContext<JvmSignatureParser::PackageSpecifierContext>(0);
}

std::vector<JvmSignatureParser::TypeArgumentsContext *> JvmSignatureParser::ClassTypeSignatureContext::typeArguments() {
  return getRuleContexts<JvmSignatureParser::TypeArgumentsContext>();
}

JvmSignatureParser::TypeArgumentsContext* JvmSignatureParser::ClassTypeSignatureContext::typeArguments(size_t i) {
  return getRuleContext<JvmSignatureParser::TypeArgumentsContext>(i);
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
  enterRule(_localctx, 18, JvmSignatureParser::RuleClassTypeSignature);
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
    match(JvmSignatureParser::T__0);
    setState(111);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 11, _ctx)) {
    case 1: {
      setState(110);
      packageSpecifier();
      break;
    }

    default:
      break;
    }
    setState(113);
    match(JvmSignatureParser::Identifier);
    setState(115);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == JvmSignatureParser::T__2) {
      setState(114);
      typeArguments();
    }
    setState(124);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == JvmSignatureParser::T__7) {
      setState(117);
      match(JvmSignatureParser::T__7);
      setState(118);
      match(JvmSignatureParser::Identifier);
      setState(120);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == JvmSignatureParser::T__2) {
        setState(119);
        typeArguments();
      }
      setState(126);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(127);
    match(JvmSignatureParser::T__1);
   
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

JvmSignatureParser::FieldTypeSignatureContext* JvmSignatureParser::FieldSignatureContext::fieldTypeSignature() {
  return getRuleContext<JvmSignatureParser::FieldTypeSignatureContext>(0);
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
  enterRule(_localctx, 20, JvmSignatureParser::RuleFieldSignature);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(129);
    fieldTypeSignature();
   
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

JvmSignatureParser::MethodTypeSignatureContext* JvmSignatureParser::MethodSignatureContext::methodTypeSignature() {
  return getRuleContext<JvmSignatureParser::MethodTypeSignatureContext>(0);
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
  enterRule(_localctx, 22, JvmSignatureParser::RuleMethodSignature);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(131);
    methodTypeSignature();
   
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
  enterRule(_localctx, 24, JvmSignatureParser::RulePackageSpecifier);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(133);
    match(JvmSignatureParser::Identifier);
    setState(134);
    match(JvmSignatureParser::T__8);
   
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

JvmSignatureParser::TypeSignatureContext* JvmSignatureParser::ArrayTypeSignatureContext::typeSignature() {
  return getRuleContext<JvmSignatureParser::TypeSignatureContext>(0);
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
  enterRule(_localctx, 26, JvmSignatureParser::RuleArrayTypeSignature);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(136);
    match(JvmSignatureParser::T__9);
    setState(137);
    typeSignature();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FieldTypeSignatureContext ------------------------------------------------------------------

JvmSignatureParser::FieldTypeSignatureContext::FieldTypeSignatureContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

JvmSignatureParser::TypeSignatureContext* JvmSignatureParser::FieldTypeSignatureContext::typeSignature() {
  return getRuleContext<JvmSignatureParser::TypeSignatureContext>(0);
}


size_t JvmSignatureParser::FieldTypeSignatureContext::getRuleIndex() const {
  return JvmSignatureParser::RuleFieldTypeSignature;
}

void JvmSignatureParser::FieldTypeSignatureContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JvmSignatureListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFieldTypeSignature(this);
}

void JvmSignatureParser::FieldTypeSignatureContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JvmSignatureListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFieldTypeSignature(this);
}

JvmSignatureParser::FieldTypeSignatureContext* JvmSignatureParser::fieldTypeSignature() {
  FieldTypeSignatureContext *_localctx = _tracker.createInstance<FieldTypeSignatureContext>(_ctx, getState());
  enterRule(_localctx, 28, JvmSignatureParser::RuleFieldTypeSignature);

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
    typeSignature();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- MethodTypeSignatureContext ------------------------------------------------------------------

JvmSignatureParser::MethodTypeSignatureContext::MethodTypeSignatureContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

JvmSignatureParser::ReturnTypeContext* JvmSignatureParser::MethodTypeSignatureContext::returnType() {
  return getRuleContext<JvmSignatureParser::ReturnTypeContext>(0);
}

std::vector<JvmSignatureParser::ParameterTypeSignatureContext *> JvmSignatureParser::MethodTypeSignatureContext::parameterTypeSignature() {
  return getRuleContexts<JvmSignatureParser::ParameterTypeSignatureContext>();
}

JvmSignatureParser::ParameterTypeSignatureContext* JvmSignatureParser::MethodTypeSignatureContext::parameterTypeSignature(size_t i) {
  return getRuleContext<JvmSignatureParser::ParameterTypeSignatureContext>(i);
}

std::vector<JvmSignatureParser::ThrowsSignatureContext *> JvmSignatureParser::MethodTypeSignatureContext::throwsSignature() {
  return getRuleContexts<JvmSignatureParser::ThrowsSignatureContext>();
}

JvmSignatureParser::ThrowsSignatureContext* JvmSignatureParser::MethodTypeSignatureContext::throwsSignature(size_t i) {
  return getRuleContext<JvmSignatureParser::ThrowsSignatureContext>(i);
}


size_t JvmSignatureParser::MethodTypeSignatureContext::getRuleIndex() const {
  return JvmSignatureParser::RuleMethodTypeSignature;
}

void JvmSignatureParser::MethodTypeSignatureContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JvmSignatureListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterMethodTypeSignature(this);
}

void JvmSignatureParser::MethodTypeSignatureContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JvmSignatureListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitMethodTypeSignature(this);
}

JvmSignatureParser::MethodTypeSignatureContext* JvmSignatureParser::methodTypeSignature() {
  MethodTypeSignatureContext *_localctx = _tracker.createInstance<MethodTypeSignatureContext>(_ctx, getState());
  enterRule(_localctx, 30, JvmSignatureParser::RuleMethodTypeSignature);
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
    setState(141);
    match(JvmSignatureParser::T__10);
    setState(145);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 8356866) != 0)) {
      setState(142);
      parameterTypeSignature();
      setState(147);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(148);
    match(JvmSignatureParser::T__11);
    setState(149);
    returnType();
    setState(153);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == JvmSignatureParser::T__13) {
      setState(150);
      throwsSignature();
      setState(155);
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

//----------------- ParameterTypeSignatureContext ------------------------------------------------------------------

JvmSignatureParser::ParameterTypeSignatureContext::ParameterTypeSignatureContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

JvmSignatureParser::TypeSignatureContext* JvmSignatureParser::ParameterTypeSignatureContext::typeSignature() {
  return getRuleContext<JvmSignatureParser::TypeSignatureContext>(0);
}


size_t JvmSignatureParser::ParameterTypeSignatureContext::getRuleIndex() const {
  return JvmSignatureParser::RuleParameterTypeSignature;
}

void JvmSignatureParser::ParameterTypeSignatureContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JvmSignatureListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterParameterTypeSignature(this);
}

void JvmSignatureParser::ParameterTypeSignatureContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JvmSignatureListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitParameterTypeSignature(this);
}

JvmSignatureParser::ParameterTypeSignatureContext* JvmSignatureParser::parameterTypeSignature() {
  ParameterTypeSignatureContext *_localctx = _tracker.createInstance<ParameterTypeSignatureContext>(_ctx, getState());
  enterRule(_localctx, 32, JvmSignatureParser::RuleParameterTypeSignature);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(156);
    typeSignature();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ReturnTypeContext ------------------------------------------------------------------

JvmSignatureParser::ReturnTypeContext::ReturnTypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

JvmSignatureParser::TypeSignatureContext* JvmSignatureParser::ReturnTypeContext::typeSignature() {
  return getRuleContext<JvmSignatureParser::TypeSignatureContext>(0);
}


size_t JvmSignatureParser::ReturnTypeContext::getRuleIndex() const {
  return JvmSignatureParser::RuleReturnType;
}

void JvmSignatureParser::ReturnTypeContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JvmSignatureListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterReturnType(this);
}

void JvmSignatureParser::ReturnTypeContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JvmSignatureListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitReturnType(this);
}

JvmSignatureParser::ReturnTypeContext* JvmSignatureParser::returnType() {
  ReturnTypeContext *_localctx = _tracker.createInstance<ReturnTypeContext>(_ctx, getState());
  enterRule(_localctx, 34, JvmSignatureParser::RuleReturnType);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(160);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case JvmSignatureParser::T__0:
      case JvmSignatureParser::T__9:
      case JvmSignatureParser::T__14:
      case JvmSignatureParser::T__15:
      case JvmSignatureParser::T__16:
      case JvmSignatureParser::T__17:
      case JvmSignatureParser::T__18:
      case JvmSignatureParser::T__19:
      case JvmSignatureParser::T__20:
      case JvmSignatureParser::T__21: {
        enterOuterAlt(_localctx, 1);
        setState(158);
        typeSignature();
        break;
      }

      case JvmSignatureParser::T__12: {
        enterOuterAlt(_localctx, 2);
        setState(159);
        match(JvmSignatureParser::T__12);
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
  enterRule(_localctx, 36, JvmSignatureParser::RuleThrowsSignature);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(162);
    match(JvmSignatureParser::T__13);
    setState(163);
    classTypeSignature();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PrimitiveTypeContext ------------------------------------------------------------------

JvmSignatureParser::PrimitiveTypeContext::PrimitiveTypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t JvmSignatureParser::PrimitiveTypeContext::getRuleIndex() const {
  return JvmSignatureParser::RulePrimitiveType;
}

void JvmSignatureParser::PrimitiveTypeContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JvmSignatureListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterPrimitiveType(this);
}

void JvmSignatureParser::PrimitiveTypeContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<JvmSignatureListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitPrimitiveType(this);
}

JvmSignatureParser::PrimitiveTypeContext* JvmSignatureParser::primitiveType() {
  PrimitiveTypeContext *_localctx = _tracker.createInstance<PrimitiveTypeContext>(_ctx, getState());
  enterRule(_localctx, 38, JvmSignatureParser::RulePrimitiveType);
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
    setState(165);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 8355840) != 0))) {
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

void JvmSignatureParser::initialize() {
  ::antlr4::internal::call_once(jvmsignatureParserOnceFlag, jvmsignatureParserInitialize);
}
