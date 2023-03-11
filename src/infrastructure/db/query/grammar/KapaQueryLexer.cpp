
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
      "T__0", "T__1", "T__2", "T__3", "T__4", "T__5", "NOT_EQUAL", "EQUAL", 
      "REGEX", "STARS_WITH", "END_WITH", "AND", "OR", "Identifier", "WS", 
      "EscapedString", "Escape"
    },
    std::vector<std::string>{
      "DEFAULT_TOKEN_CHANNEL", "HIDDEN"
    },
    std::vector<std::string>{
      "DEFAULT_MODE"
    },
    std::vector<std::string>{
      "", "'schema'", "'list'", "'select'", "'where'", "'('", "')'", "'!='", 
      "'='", "'*^*'", "'^*'", "'*^'", "'and'", "'or'"
    },
    std::vector<std::string>{
      "", "", "", "", "", "", "", "NOT_EQUAL", "EQUAL", "REGEX", "STARS_WITH", 
      "END_WITH", "AND", "OR", "Identifier", "WS", "EscapedString"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,0,16,139,6,-1,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,
  	6,2,7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,2,14,
  	7,14,2,15,7,15,2,16,7,16,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,1,1,1,1,1,1,1,
  	1,1,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,3,1,3,1,3,1,3,1,3,1,3,1,4,1,4,1,5,1,
  	5,1,6,1,6,1,6,1,7,1,7,1,8,1,8,1,8,1,8,1,9,1,9,1,9,1,10,1,10,1,10,1,11,
  	1,11,1,11,1,11,1,12,1,12,1,12,1,13,1,13,5,13,89,8,13,10,13,12,13,92,9,
  	13,1,14,4,14,95,8,14,11,14,12,14,96,1,14,1,14,1,15,1,15,1,15,1,15,1,15,
  	5,15,106,8,15,10,15,12,15,109,9,15,1,15,1,15,1,15,1,15,1,15,1,15,5,15,
  	117,8,15,10,15,12,15,120,9,15,1,15,1,15,1,15,1,15,5,15,126,8,15,10,15,
  	12,15,129,9,15,1,15,3,15,132,8,15,1,16,1,16,1,16,1,16,3,16,138,8,16,1,
  	127,0,17,1,1,3,2,5,3,7,4,9,5,11,6,13,7,15,8,17,9,19,10,21,11,23,12,25,
  	13,27,14,29,15,31,16,33,0,1,0,6,3,0,65,90,95,95,97,122,4,0,48,57,65,90,
  	95,95,97,122,3,0,9,10,13,13,32,32,1,0,34,34,1,0,39,39,2,0,8221,8221,8243,
  	8243,150,0,1,1,0,0,0,0,3,1,0,0,0,0,5,1,0,0,0,0,7,1,0,0,0,0,9,1,0,0,0,
  	0,11,1,0,0,0,0,13,1,0,0,0,0,15,1,0,0,0,0,17,1,0,0,0,0,19,1,0,0,0,0,21,
  	1,0,0,0,0,23,1,0,0,0,0,25,1,0,0,0,0,27,1,0,0,0,0,29,1,0,0,0,0,31,1,0,
  	0,0,1,35,1,0,0,0,3,42,1,0,0,0,5,47,1,0,0,0,7,54,1,0,0,0,9,60,1,0,0,0,
  	11,62,1,0,0,0,13,64,1,0,0,0,15,67,1,0,0,0,17,69,1,0,0,0,19,73,1,0,0,0,
  	21,76,1,0,0,0,23,79,1,0,0,0,25,83,1,0,0,0,27,86,1,0,0,0,29,94,1,0,0,0,
  	31,131,1,0,0,0,33,137,1,0,0,0,35,36,5,115,0,0,36,37,5,99,0,0,37,38,5,
  	104,0,0,38,39,5,101,0,0,39,40,5,109,0,0,40,41,5,97,0,0,41,2,1,0,0,0,42,
  	43,5,108,0,0,43,44,5,105,0,0,44,45,5,115,0,0,45,46,5,116,0,0,46,4,1,0,
  	0,0,47,48,5,115,0,0,48,49,5,101,0,0,49,50,5,108,0,0,50,51,5,101,0,0,51,
  	52,5,99,0,0,52,53,5,116,0,0,53,6,1,0,0,0,54,55,5,119,0,0,55,56,5,104,
  	0,0,56,57,5,101,0,0,57,58,5,114,0,0,58,59,5,101,0,0,59,8,1,0,0,0,60,61,
  	5,40,0,0,61,10,1,0,0,0,62,63,5,41,0,0,63,12,1,0,0,0,64,65,5,33,0,0,65,
  	66,5,61,0,0,66,14,1,0,0,0,67,68,5,61,0,0,68,16,1,0,0,0,69,70,5,42,0,0,
  	70,71,5,94,0,0,71,72,5,42,0,0,72,18,1,0,0,0,73,74,5,94,0,0,74,75,5,42,
  	0,0,75,20,1,0,0,0,76,77,5,42,0,0,77,78,5,94,0,0,78,22,1,0,0,0,79,80,5,
  	97,0,0,80,81,5,110,0,0,81,82,5,100,0,0,82,24,1,0,0,0,83,84,5,111,0,0,
  	84,85,5,114,0,0,85,26,1,0,0,0,86,90,7,0,0,0,87,89,7,1,0,0,88,87,1,0,0,
  	0,89,92,1,0,0,0,90,88,1,0,0,0,90,91,1,0,0,0,91,28,1,0,0,0,92,90,1,0,0,
  	0,93,95,7,2,0,0,94,93,1,0,0,0,95,96,1,0,0,0,96,94,1,0,0,0,96,97,1,0,0,
  	0,97,98,1,0,0,0,98,99,6,14,0,0,99,30,1,0,0,0,100,107,5,34,0,0,101,106,
  	3,33,16,0,102,103,5,34,0,0,103,106,5,34,0,0,104,106,8,3,0,0,105,101,1,
  	0,0,0,105,102,1,0,0,0,105,104,1,0,0,0,106,109,1,0,0,0,107,105,1,0,0,0,
  	107,108,1,0,0,0,108,110,1,0,0,0,109,107,1,0,0,0,110,132,5,34,0,0,111,
  	118,5,39,0,0,112,117,3,33,16,0,113,114,5,39,0,0,114,117,5,39,0,0,115,
  	117,8,4,0,0,116,112,1,0,0,0,116,113,1,0,0,0,116,115,1,0,0,0,117,120,1,
  	0,0,0,118,116,1,0,0,0,118,119,1,0,0,0,119,121,1,0,0,0,120,118,1,0,0,0,
  	121,132,5,39,0,0,122,127,5,8220,0,0,123,126,3,33,16,0,124,126,9,0,0,0,
  	125,123,1,0,0,0,125,124,1,0,0,0,126,129,1,0,0,0,127,128,1,0,0,0,127,125,
  	1,0,0,0,128,130,1,0,0,0,129,127,1,0,0,0,130,132,7,5,0,0,131,100,1,0,0,
  	0,131,111,1,0,0,0,131,122,1,0,0,0,132,32,1,0,0,0,133,134,5,96,0,0,134,
  	138,5,39,0,0,135,136,5,96,0,0,136,138,5,34,0,0,137,133,1,0,0,0,137,135,
  	1,0,0,0,138,34,1,0,0,0,11,0,90,96,105,107,116,118,125,127,131,137,1,6,
  	0,0
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
