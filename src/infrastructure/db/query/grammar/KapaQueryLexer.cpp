
// Generated from KapaQuery.g4 by ANTLR 4.12.0


#include "KapaQueryLexer.h"


using namespace antlr4;

using namespace kapa::infrastructure::db::query;


using namespace antlr4;

namespace {

struct KapaQueryLexerStaticData final {
  KapaQueryLexerStaticData(std::vector<std::string> ruleNames,
                          std::vector<std::string> channelNames,
                          std::vector<std::string> modeNames,
                          std::vector<std::string> literalNames,
                          std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), channelNames(std::move(channelNames)),
        modeNames(std::move(modeNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  KapaQueryLexerStaticData(const KapaQueryLexerStaticData&) = delete;
  KapaQueryLexerStaticData(KapaQueryLexerStaticData&&) = delete;
  KapaQueryLexerStaticData& operator=(const KapaQueryLexerStaticData&) = delete;
  KapaQueryLexerStaticData& operator=(KapaQueryLexerStaticData&&) = delete;

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

::antlr4::internal::OnceFlag kapaquerylexerLexerOnceFlag;
KapaQueryLexerStaticData *kapaquerylexerLexerStaticData = nullptr;

void kapaquerylexerLexerInitialize() {
  assert(kapaquerylexerLexerStaticData == nullptr);
  auto staticData = std::make_unique<KapaQueryLexerStaticData>(
    std::vector<std::string>{
      "T__0", "T__1", "T__2", "T__3", "T__4", "T__5", "T__6", "OP", "Identifier", 
      "WS", "EscapedString", "Escape"
    },
    std::vector<std::string>{
      "DEFAULT_TOKEN_CHANNEL", "HIDDEN"
    },
    std::vector<std::string>{
      "DEFAULT_MODE"
    },
    std::vector<std::string>{
      "", "'schema'", "'list'", "'where'", "'('", "'and'", "')'", "'or'"
    },
    std::vector<std::string>{
      "", "", "", "", "", "", "", "", "OP", "Identifier", "WS", "EscapedString"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,0,11,114,6,-1,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,
  	6,2,7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,1,0,1,0,1,0,1,0,1,0,1,0,
  	1,0,1,1,1,1,1,1,1,1,1,1,1,2,1,2,1,2,1,2,1,2,1,2,1,3,1,3,1,4,1,4,1,4,1,
  	4,1,5,1,5,1,6,1,6,1,6,1,7,1,7,1,7,1,7,1,7,3,7,60,8,7,1,8,1,8,5,8,64,8,
  	8,10,8,12,8,67,9,8,1,9,4,9,70,8,9,11,9,12,9,71,1,9,1,9,1,10,1,10,1,10,
  	1,10,1,10,5,10,81,8,10,10,10,12,10,84,9,10,1,10,1,10,1,10,1,10,1,10,1,
  	10,5,10,92,8,10,10,10,12,10,95,9,10,1,10,1,10,1,10,1,10,5,10,101,8,10,
  	10,10,12,10,104,9,10,1,10,3,10,107,8,10,1,11,1,11,1,11,1,11,3,11,113,
  	8,11,1,102,0,12,1,1,3,2,5,3,7,4,9,5,11,6,13,7,15,8,17,9,19,10,21,11,23,
  	0,1,0,6,3,0,65,90,95,95,97,122,4,0,48,57,65,90,95,95,97,122,3,0,9,10,
  	13,13,32,32,1,0,34,34,1,0,39,39,2,0,8221,8221,8243,8243,126,0,1,1,0,0,
  	0,0,3,1,0,0,0,0,5,1,0,0,0,0,7,1,0,0,0,0,9,1,0,0,0,0,11,1,0,0,0,0,13,1,
  	0,0,0,0,15,1,0,0,0,0,17,1,0,0,0,0,19,1,0,0,0,0,21,1,0,0,0,1,25,1,0,0,
  	0,3,32,1,0,0,0,5,37,1,0,0,0,7,43,1,0,0,0,9,45,1,0,0,0,11,49,1,0,0,0,13,
  	51,1,0,0,0,15,59,1,0,0,0,17,61,1,0,0,0,19,69,1,0,0,0,21,106,1,0,0,0,23,
  	112,1,0,0,0,25,26,5,115,0,0,26,27,5,99,0,0,27,28,5,104,0,0,28,29,5,101,
  	0,0,29,30,5,109,0,0,30,31,5,97,0,0,31,2,1,0,0,0,32,33,5,108,0,0,33,34,
  	5,105,0,0,34,35,5,115,0,0,35,36,5,116,0,0,36,4,1,0,0,0,37,38,5,119,0,
  	0,38,39,5,104,0,0,39,40,5,101,0,0,40,41,5,114,0,0,41,42,5,101,0,0,42,
  	6,1,0,0,0,43,44,5,40,0,0,44,8,1,0,0,0,45,46,5,97,0,0,46,47,5,110,0,0,
  	47,48,5,100,0,0,48,10,1,0,0,0,49,50,5,41,0,0,50,12,1,0,0,0,51,52,5,111,
  	0,0,52,53,5,114,0,0,53,14,1,0,0,0,54,60,5,61,0,0,55,56,5,108,0,0,56,57,
  	5,105,0,0,57,58,5,107,0,0,58,60,5,101,0,0,59,54,1,0,0,0,59,55,1,0,0,0,
  	60,16,1,0,0,0,61,65,7,0,0,0,62,64,7,1,0,0,63,62,1,0,0,0,64,67,1,0,0,0,
  	65,63,1,0,0,0,65,66,1,0,0,0,66,18,1,0,0,0,67,65,1,0,0,0,68,70,7,2,0,0,
  	69,68,1,0,0,0,70,71,1,0,0,0,71,69,1,0,0,0,71,72,1,0,0,0,72,73,1,0,0,0,
  	73,74,6,9,0,0,74,20,1,0,0,0,75,82,5,34,0,0,76,81,3,23,11,0,77,78,5,34,
  	0,0,78,81,5,34,0,0,79,81,8,3,0,0,80,76,1,0,0,0,80,77,1,0,0,0,80,79,1,
  	0,0,0,81,84,1,0,0,0,82,80,1,0,0,0,82,83,1,0,0,0,83,85,1,0,0,0,84,82,1,
  	0,0,0,85,107,5,34,0,0,86,93,5,39,0,0,87,92,3,23,11,0,88,89,5,39,0,0,89,
  	92,5,39,0,0,90,92,8,4,0,0,91,87,1,0,0,0,91,88,1,0,0,0,91,90,1,0,0,0,92,
  	95,1,0,0,0,93,91,1,0,0,0,93,94,1,0,0,0,94,96,1,0,0,0,95,93,1,0,0,0,96,
  	107,5,39,0,0,97,102,5,8220,0,0,98,101,3,23,11,0,99,101,9,0,0,0,100,98,
  	1,0,0,0,100,99,1,0,0,0,101,104,1,0,0,0,102,103,1,0,0,0,102,100,1,0,0,
  	0,103,105,1,0,0,0,104,102,1,0,0,0,105,107,7,5,0,0,106,75,1,0,0,0,106,
  	86,1,0,0,0,106,97,1,0,0,0,107,22,1,0,0,0,108,109,5,96,0,0,109,113,5,39,
  	0,0,110,111,5,96,0,0,111,113,5,34,0,0,112,108,1,0,0,0,112,110,1,0,0,0,
  	113,24,1,0,0,0,12,0,59,65,71,80,82,91,93,100,102,106,112,1,6,0,0
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  kapaquerylexerLexerStaticData = staticData.release();
}

}

KapaQueryLexer::KapaQueryLexer(CharStream *input) : Lexer(input) {
  KapaQueryLexer::initialize();
  _interpreter = new atn::LexerATNSimulator(this, *kapaquerylexerLexerStaticData->atn, kapaquerylexerLexerStaticData->decisionToDFA, kapaquerylexerLexerStaticData->sharedContextCache);
}

KapaQueryLexer::~KapaQueryLexer() {
  delete _interpreter;
}

std::string KapaQueryLexer::getGrammarFileName() const {
  return "KapaQuery.g4";
}

const std::vector<std::string>& KapaQueryLexer::getRuleNames() const {
  return kapaquerylexerLexerStaticData->ruleNames;
}

const std::vector<std::string>& KapaQueryLexer::getChannelNames() const {
  return kapaquerylexerLexerStaticData->channelNames;
}

const std::vector<std::string>& KapaQueryLexer::getModeNames() const {
  return kapaquerylexerLexerStaticData->modeNames;
}

const dfa::Vocabulary& KapaQueryLexer::getVocabulary() const {
  return kapaquerylexerLexerStaticData->vocabulary;
}

antlr4::atn::SerializedATNView KapaQueryLexer::getSerializedATN() const {
  return kapaquerylexerLexerStaticData->serializedATN;
}

const atn::ATN& KapaQueryLexer::getATN() const {
  return *kapaquerylexerLexerStaticData->atn;
}




void KapaQueryLexer::initialize() {
  ::antlr4::internal::call_once(kapaquerylexerLexerOnceFlag, kapaquerylexerLexerInitialize);
}
