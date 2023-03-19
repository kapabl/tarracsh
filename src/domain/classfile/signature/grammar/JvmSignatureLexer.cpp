
// Generated from JvmSignature.g4 by ANTLR 4.12.0


#include "JvmSignatureLexer.h"


using namespace antlr4;

using namespace kapa::tarracsh::domain::classfile::signature::parser;


using namespace antlr4;

namespace {

struct JvmSignatureLexerStaticData final {
  JvmSignatureLexerStaticData(std::vector<std::string> ruleNames,
                          std::vector<std::string> channelNames,
                          std::vector<std::string> modeNames,
                          std::vector<std::string> literalNames,
                          std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), channelNames(std::move(channelNames)),
        modeNames(std::move(modeNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  JvmSignatureLexerStaticData(const JvmSignatureLexerStaticData&) = delete;
  JvmSignatureLexerStaticData(JvmSignatureLexerStaticData&&) = delete;
  JvmSignatureLexerStaticData& operator=(const JvmSignatureLexerStaticData&) = delete;
  JvmSignatureLexerStaticData& operator=(JvmSignatureLexerStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> channelNames;
  const std::vector<std::string> modeNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

::antlr4::internal::OnceFlag jvmsignaturelexerLexerOnceFlag;
JvmSignatureLexerStaticData *jvmsignaturelexerLexerStaticData = nullptr;

void jvmsignaturelexerLexerInitialize() {
  assert(jvmsignaturelexerLexerStaticData == nullptr);
  auto staticData = std::make_unique<JvmSignatureLexerStaticData>(
    std::vector<std::string>{
      "T__0", "T__1", "T__2", "T__3", "T__4", "T__5", "T__6", "T__7", "T__8", 
      "T__9", "T__10", "T__11", "T__12", "T__13", "T__14", "T__15", "T__16", 
      "T__17", "T__18", "T__19", "T__20", "T__21", "Identifier", "WS"
    },
    std::vector<std::string>{
      "DEFAULT_TOKEN_CHANNEL", "HIDDEN"
    },
    std::vector<std::string>{
      "DEFAULT_MODE"
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
  	4,0,24,114,6,-1,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,
  	6,2,7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,2,14,
  	7,14,2,15,7,15,2,16,7,16,2,17,7,17,2,18,7,18,2,19,7,19,2,20,7,20,2,21,
  	7,21,2,22,7,22,2,23,7,23,1,0,1,0,1,1,1,1,1,2,1,2,1,3,1,3,1,4,1,4,1,5,
  	1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,6,1,6,1,6,1,6,1,6,1,6,1,7,1,7,1,8,1,8,1,
  	9,1,9,1,10,1,10,1,11,1,11,1,12,1,12,1,13,1,13,1,14,1,14,1,15,1,15,1,16,
  	1,16,1,17,1,17,1,18,1,18,1,19,1,19,1,20,1,20,1,21,1,21,1,22,1,22,5,22,
  	106,8,22,10,22,12,22,109,9,22,1,23,1,23,1,23,1,23,0,0,24,1,1,3,2,5,3,
  	7,4,9,5,11,6,13,7,15,8,17,9,19,10,21,11,23,12,25,13,27,14,29,15,31,16,
  	33,17,35,18,37,19,39,20,41,21,43,22,45,23,47,24,1,0,3,4,0,36,36,65,90,
  	95,95,97,122,5,0,36,36,48,57,65,90,95,95,97,122,3,0,9,10,13,13,32,32,
  	114,0,1,1,0,0,0,0,3,1,0,0,0,0,5,1,0,0,0,0,7,1,0,0,0,0,9,1,0,0,0,0,11,
  	1,0,0,0,0,13,1,0,0,0,0,15,1,0,0,0,0,17,1,0,0,0,0,19,1,0,0,0,0,21,1,0,
  	0,0,0,23,1,0,0,0,0,25,1,0,0,0,0,27,1,0,0,0,0,29,1,0,0,0,0,31,1,0,0,0,
  	0,33,1,0,0,0,0,35,1,0,0,0,0,37,1,0,0,0,0,39,1,0,0,0,0,41,1,0,0,0,0,43,
  	1,0,0,0,0,45,1,0,0,0,0,47,1,0,0,0,1,49,1,0,0,0,3,51,1,0,0,0,5,53,1,0,
  	0,0,7,55,1,0,0,0,9,57,1,0,0,0,11,59,1,0,0,0,13,67,1,0,0,0,15,73,1,0,0,
  	0,17,75,1,0,0,0,19,77,1,0,0,0,21,79,1,0,0,0,23,81,1,0,0,0,25,83,1,0,0,
  	0,27,85,1,0,0,0,29,87,1,0,0,0,31,89,1,0,0,0,33,91,1,0,0,0,35,93,1,0,0,
  	0,37,95,1,0,0,0,39,97,1,0,0,0,41,99,1,0,0,0,43,101,1,0,0,0,45,103,1,0,
  	0,0,47,110,1,0,0,0,49,50,5,76,0,0,50,2,1,0,0,0,51,52,5,59,0,0,52,4,1,
  	0,0,0,53,54,5,60,0,0,54,6,1,0,0,0,55,56,5,62,0,0,56,8,1,0,0,0,57,58,5,
  	63,0,0,58,10,1,0,0,0,59,60,5,101,0,0,60,61,5,120,0,0,61,62,5,116,0,0,
  	62,63,5,101,0,0,63,64,5,110,0,0,64,65,5,100,0,0,65,66,5,115,0,0,66,12,
  	1,0,0,0,67,68,5,115,0,0,68,69,5,117,0,0,69,70,5,112,0,0,70,71,5,101,0,
  	0,71,72,5,114,0,0,72,14,1,0,0,0,73,74,5,46,0,0,74,16,1,0,0,0,75,76,5,
  	47,0,0,76,18,1,0,0,0,77,78,5,91,0,0,78,20,1,0,0,0,79,80,5,40,0,0,80,22,
  	1,0,0,0,81,82,5,41,0,0,82,24,1,0,0,0,83,84,5,86,0,0,84,26,1,0,0,0,85,
  	86,5,94,0,0,86,28,1,0,0,0,87,88,5,66,0,0,88,30,1,0,0,0,89,90,5,67,0,0,
  	90,32,1,0,0,0,91,92,5,68,0,0,92,34,1,0,0,0,93,94,5,70,0,0,94,36,1,0,0,
  	0,95,96,5,73,0,0,96,38,1,0,0,0,97,98,5,74,0,0,98,40,1,0,0,0,99,100,5,
  	83,0,0,100,42,1,0,0,0,101,102,5,90,0,0,102,44,1,0,0,0,103,107,7,0,0,0,
  	104,106,7,1,0,0,105,104,1,0,0,0,106,109,1,0,0,0,107,105,1,0,0,0,107,108,
  	1,0,0,0,108,46,1,0,0,0,109,107,1,0,0,0,110,111,7,2,0,0,111,112,1,0,0,
  	0,112,113,6,23,0,0,113,48,1,0,0,0,2,0,107,1,6,0,0
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  jvmsignaturelexerLexerStaticData = staticData.release();
}

}

JvmSignatureLexer::JvmSignatureLexer(CharStream *input) : Lexer(input) {
  JvmSignatureLexer::initialize();
  _interpreter = new atn::LexerATNSimulator(this, *jvmsignaturelexerLexerStaticData->atn, jvmsignaturelexerLexerStaticData->decisionToDFA, jvmsignaturelexerLexerStaticData->sharedContextCache);
}

JvmSignatureLexer::~JvmSignatureLexer() {
  delete _interpreter;
}

std::string JvmSignatureLexer::getGrammarFileName() const {
  return "JvmSignature.g4";
}

const std::vector<std::string>& JvmSignatureLexer::getRuleNames() const {
  return jvmsignaturelexerLexerStaticData->ruleNames;
}

const std::vector<std::string>& JvmSignatureLexer::getChannelNames() const {
  return jvmsignaturelexerLexerStaticData->channelNames;
}

const std::vector<std::string>& JvmSignatureLexer::getModeNames() const {
  return jvmsignaturelexerLexerStaticData->modeNames;
}

const dfa::Vocabulary& JvmSignatureLexer::getVocabulary() const {
  return jvmsignaturelexerLexerStaticData->vocabulary;
}

antlr4::atn::SerializedATNView JvmSignatureLexer::getSerializedATN() const {
  return jvmsignaturelexerLexerStaticData->serializedATN;
}

const atn::ATN& JvmSignatureLexer::getATN() const {
  return *jvmsignaturelexerLexerStaticData->atn;
}




void JvmSignatureLexer::initialize() {
  ::antlr4::internal::call_once(jvmsignaturelexerLexerOnceFlag, jvmsignaturelexerLexerInitialize);
}
