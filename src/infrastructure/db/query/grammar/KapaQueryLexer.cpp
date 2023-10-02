
// Generated from KapaQuery.g4 by ANTLR 4.13.1


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
#if ANTLR4_USE_THREAD_LOCAL_CACHE
static thread_local
#endif
KapaQueryLexerStaticData *kapaquerylexerLexerStaticData = nullptr;

void kapaquerylexerLexerInitialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  if (kapaquerylexerLexerStaticData != nullptr) {
    return;
  }
#else
  assert(kapaquerylexerLexerStaticData == nullptr);
#endif
  auto staticData = std::make_unique<KapaQueryLexerStaticData>(
    std::vector<std::string>{
      "T__0", "T__1", "T__2", "T__3", "T__4", "T__5", "NOT", "NOT_EQUAL", 
      "EQUAL", "REGEX", "STARTS_WITH", "ENDS_WITH", "AND", "OR", "Identifier", 
      "WS", "EscapedString", "Escape"
    },
    std::vector<std::string>{
      "DEFAULT_TOKEN_CHANNEL", "HIDDEN"
    },
    std::vector<std::string>{
      "DEFAULT_MODE"
    },
    std::vector<std::string>{
      "", "'schema'", "'list'", "'select'", "'where'", "'('", "')'", "'not'", 
      "'!='", "'='", "'*^*'", "'^*'", "'*^'", "'and'", "'or'"
    },
    std::vector<std::string>{
      "", "", "", "", "", "", "", "NOT", "NOT_EQUAL", "EQUAL", "REGEX", 
      "STARTS_WITH", "ENDS_WITH", "AND", "OR", "Identifier", "WS", "EscapedString"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,0,17,145,6,-1,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,
  	6,2,7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,2,14,
  	7,14,2,15,7,15,2,16,7,16,2,17,7,17,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,1,1,
  	1,1,1,1,1,1,1,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,3,1,3,1,3,1,3,1,3,1,3,1,4,
  	1,4,1,5,1,5,1,6,1,6,1,6,1,6,1,7,1,7,1,7,1,8,1,8,1,9,1,9,1,9,1,9,1,10,
  	1,10,1,10,1,11,1,11,1,11,1,12,1,12,1,12,1,12,1,13,1,13,1,13,1,14,1,14,
  	5,14,95,8,14,10,14,12,14,98,9,14,1,15,4,15,101,8,15,11,15,12,15,102,1,
  	15,1,15,1,16,1,16,1,16,1,16,1,16,5,16,112,8,16,10,16,12,16,115,9,16,1,
  	16,1,16,1,16,1,16,1,16,1,16,5,16,123,8,16,10,16,12,16,126,9,16,1,16,1,
  	16,1,16,1,16,5,16,132,8,16,10,16,12,16,135,9,16,1,16,3,16,138,8,16,1,
  	17,1,17,1,17,1,17,3,17,144,8,17,1,133,0,18,1,1,3,2,5,3,7,4,9,5,11,6,13,
  	7,15,8,17,9,19,10,21,11,23,12,25,13,27,14,29,15,31,16,33,17,35,0,1,0,
  	6,3,0,65,90,95,95,97,122,4,0,48,57,65,90,95,95,97,122,3,0,9,10,13,13,
  	32,32,1,0,34,34,1,0,39,39,2,0,8221,8221,8243,8243,156,0,1,1,0,0,0,0,3,
  	1,0,0,0,0,5,1,0,0,0,0,7,1,0,0,0,0,9,1,0,0,0,0,11,1,0,0,0,0,13,1,0,0,0,
  	0,15,1,0,0,0,0,17,1,0,0,0,0,19,1,0,0,0,0,21,1,0,0,0,0,23,1,0,0,0,0,25,
  	1,0,0,0,0,27,1,0,0,0,0,29,1,0,0,0,0,31,1,0,0,0,0,33,1,0,0,0,1,37,1,0,
  	0,0,3,44,1,0,0,0,5,49,1,0,0,0,7,56,1,0,0,0,9,62,1,0,0,0,11,64,1,0,0,0,
  	13,66,1,0,0,0,15,70,1,0,0,0,17,73,1,0,0,0,19,75,1,0,0,0,21,79,1,0,0,0,
  	23,82,1,0,0,0,25,85,1,0,0,0,27,89,1,0,0,0,29,92,1,0,0,0,31,100,1,0,0,
  	0,33,137,1,0,0,0,35,143,1,0,0,0,37,38,5,115,0,0,38,39,5,99,0,0,39,40,
  	5,104,0,0,40,41,5,101,0,0,41,42,5,109,0,0,42,43,5,97,0,0,43,2,1,0,0,0,
  	44,45,5,108,0,0,45,46,5,105,0,0,46,47,5,115,0,0,47,48,5,116,0,0,48,4,
  	1,0,0,0,49,50,5,115,0,0,50,51,5,101,0,0,51,52,5,108,0,0,52,53,5,101,0,
  	0,53,54,5,99,0,0,54,55,5,116,0,0,55,6,1,0,0,0,56,57,5,119,0,0,57,58,5,
  	104,0,0,58,59,5,101,0,0,59,60,5,114,0,0,60,61,5,101,0,0,61,8,1,0,0,0,
  	62,63,5,40,0,0,63,10,1,0,0,0,64,65,5,41,0,0,65,12,1,0,0,0,66,67,5,110,
  	0,0,67,68,5,111,0,0,68,69,5,116,0,0,69,14,1,0,0,0,70,71,5,33,0,0,71,72,
  	5,61,0,0,72,16,1,0,0,0,73,74,5,61,0,0,74,18,1,0,0,0,75,76,5,42,0,0,76,
  	77,5,94,0,0,77,78,5,42,0,0,78,20,1,0,0,0,79,80,5,94,0,0,80,81,5,42,0,
  	0,81,22,1,0,0,0,82,83,5,42,0,0,83,84,5,94,0,0,84,24,1,0,0,0,85,86,5,97,
  	0,0,86,87,5,110,0,0,87,88,5,100,0,0,88,26,1,0,0,0,89,90,5,111,0,0,90,
  	91,5,114,0,0,91,28,1,0,0,0,92,96,7,0,0,0,93,95,7,1,0,0,94,93,1,0,0,0,
  	95,98,1,0,0,0,96,94,1,0,0,0,96,97,1,0,0,0,97,30,1,0,0,0,98,96,1,0,0,0,
  	99,101,7,2,0,0,100,99,1,0,0,0,101,102,1,0,0,0,102,100,1,0,0,0,102,103,
  	1,0,0,0,103,104,1,0,0,0,104,105,6,15,0,0,105,32,1,0,0,0,106,113,5,34,
  	0,0,107,112,3,35,17,0,108,109,5,34,0,0,109,112,5,34,0,0,110,112,8,3,0,
  	0,111,107,1,0,0,0,111,108,1,0,0,0,111,110,1,0,0,0,112,115,1,0,0,0,113,
  	111,1,0,0,0,113,114,1,0,0,0,114,116,1,0,0,0,115,113,1,0,0,0,116,138,5,
  	34,0,0,117,124,5,39,0,0,118,123,3,35,17,0,119,120,5,39,0,0,120,123,5,
  	39,0,0,121,123,8,4,0,0,122,118,1,0,0,0,122,119,1,0,0,0,122,121,1,0,0,
  	0,123,126,1,0,0,0,124,122,1,0,0,0,124,125,1,0,0,0,125,127,1,0,0,0,126,
  	124,1,0,0,0,127,138,5,39,0,0,128,133,5,8220,0,0,129,132,3,35,17,0,130,
  	132,9,0,0,0,131,129,1,0,0,0,131,130,1,0,0,0,132,135,1,0,0,0,133,134,1,
  	0,0,0,133,131,1,0,0,0,134,136,1,0,0,0,135,133,1,0,0,0,136,138,7,5,0,0,
  	137,106,1,0,0,0,137,117,1,0,0,0,137,128,1,0,0,0,138,34,1,0,0,0,139,140,
  	5,96,0,0,140,144,5,39,0,0,141,142,5,96,0,0,142,144,5,34,0,0,143,139,1,
  	0,0,0,143,141,1,0,0,0,144,36,1,0,0,0,11,0,96,102,111,113,122,124,131,
  	133,137,143,1,6,0,0
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
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  kapaquerylexerLexerInitialize();
#else
  ::antlr4::internal::call_once(kapaquerylexerLexerOnceFlag, kapaquerylexerLexerInitialize);
#endif
}
