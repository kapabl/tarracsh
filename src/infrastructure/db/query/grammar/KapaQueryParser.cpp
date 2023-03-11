
// Generated from KapaQuery.g4 by ANTLR 4.12.0


#include "KapaQueryListener.h"

#include "KapaQueryParser.h"


using namespace antlrcpp;
using namespace kapa::infrastructure::db::query;

using namespace antlr4;

namespace {

struct KapaQueryParserStaticData final {
  KapaQueryParserStaticData(std::vector<std::string> ruleNames,
                        std::vector<std::string> literalNames,
                        std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  KapaQueryParserStaticData(const KapaQueryParserStaticData&) = delete;
  KapaQueryParserStaticData(KapaQueryParserStaticData&&) = delete;
  KapaQueryParserStaticData& operator=(const KapaQueryParserStaticData&) = delete;
  KapaQueryParserStaticData& operator=(KapaQueryParserStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

::antlr4::internal::OnceFlag kapaqueryParserOnceFlag;
KapaQueryParserStaticData *kapaqueryParserStaticData = nullptr;

void kapaqueryParserInitialize() {
  assert(kapaqueryParserStaticData == nullptr);
  auto staticData = std::make_unique<KapaQueryParserStaticData>(
    std::vector<std::string>{
      "query", "statement", "schema", "list", "where", "expr", "filter", 
      "tablename", "column", "oper", "logical_oper"
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
  	4,1,16,61,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,6,2,7,
  	7,7,2,8,7,8,2,9,7,9,2,10,7,10,1,0,1,0,1,1,1,1,3,1,27,8,1,1,2,1,2,1,3,
  	1,3,1,3,1,3,3,3,35,8,3,1,4,1,4,1,4,1,5,1,5,1,5,1,5,1,5,1,5,1,5,3,5,47,
  	8,5,1,6,1,6,1,6,1,6,1,7,1,7,1,8,1,8,1,9,1,9,1,10,1,10,1,10,0,0,11,0,2,
  	4,6,8,10,12,14,16,18,20,0,3,1,0,2,3,1,0,7,11,1,0,12,13,52,0,22,1,0,0,
  	0,2,26,1,0,0,0,4,28,1,0,0,0,6,30,1,0,0,0,8,36,1,0,0,0,10,46,1,0,0,0,12,
  	48,1,0,0,0,14,52,1,0,0,0,16,54,1,0,0,0,18,56,1,0,0,0,20,58,1,0,0,0,22,
  	23,3,2,1,0,23,1,1,0,0,0,24,27,3,6,3,0,25,27,3,4,2,0,26,24,1,0,0,0,26,
  	25,1,0,0,0,27,3,1,0,0,0,28,29,5,1,0,0,29,5,1,0,0,0,30,31,7,0,0,0,31,34,
  	3,14,7,0,32,35,3,8,4,0,33,35,1,0,0,0,34,32,1,0,0,0,34,33,1,0,0,0,35,7,
  	1,0,0,0,36,37,5,4,0,0,37,38,3,10,5,0,38,9,1,0,0,0,39,47,3,12,6,0,40,41,
  	5,5,0,0,41,42,3,10,5,0,42,43,3,20,10,0,43,44,3,10,5,0,44,45,5,6,0,0,45,
  	47,1,0,0,0,46,39,1,0,0,0,46,40,1,0,0,0,47,11,1,0,0,0,48,49,3,16,8,0,49,
  	50,3,18,9,0,50,51,5,16,0,0,51,13,1,0,0,0,52,53,5,14,0,0,53,15,1,0,0,0,
  	54,55,5,14,0,0,55,17,1,0,0,0,56,57,7,1,0,0,57,19,1,0,0,0,58,59,7,2,0,
  	0,59,21,1,0,0,0,3,26,34,46
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  kapaqueryParserStaticData = staticData.release();
}

}

KapaQueryParser::KapaQueryParser(TokenStream *input) : KapaQueryParser(input, antlr4::atn::ParserATNSimulatorOptions()) {}

KapaQueryParser::KapaQueryParser(TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options) : Parser(input) {
  KapaQueryParser::initialize();
  _interpreter = new atn::ParserATNSimulator(this, *kapaqueryParserStaticData->atn, kapaqueryParserStaticData->decisionToDFA, kapaqueryParserStaticData->sharedContextCache, options);
}

KapaQueryParser::~KapaQueryParser() {
  delete _interpreter;
}

const atn::ATN& KapaQueryParser::getATN() const {
  return *kapaqueryParserStaticData->atn;
}

std::string KapaQueryParser::getGrammarFileName() const {
  return "KapaQuery.g4";
}

const std::vector<std::string>& KapaQueryParser::getRuleNames() const {
  return kapaqueryParserStaticData->ruleNames;
}

const dfa::Vocabulary& KapaQueryParser::getVocabulary() const {
  return kapaqueryParserStaticData->vocabulary;
}

antlr4::atn::SerializedATNView KapaQueryParser::getSerializedATN() const {
  return kapaqueryParserStaticData->serializedATN;
}


//----------------- QueryContext ------------------------------------------------------------------

KapaQueryParser::QueryContext::QueryContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

KapaQueryParser::StatementContext* KapaQueryParser::QueryContext::statement() {
  return getRuleContext<KapaQueryParser::StatementContext>(0);
}


size_t KapaQueryParser::QueryContext::getRuleIndex() const {
  return KapaQueryParser::RuleQuery;
}

void KapaQueryParser::QueryContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KapaQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterQuery(this);
}

void KapaQueryParser::QueryContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KapaQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitQuery(this);
}

KapaQueryParser::QueryContext* KapaQueryParser::query() {
  QueryContext *_localctx = _tracker.createInstance<QueryContext>(_ctx, getState());
  enterRule(_localctx, 0, KapaQueryParser::RuleQuery);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(22);
    statement();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StatementContext ------------------------------------------------------------------

KapaQueryParser::StatementContext::StatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

KapaQueryParser::ListContext* KapaQueryParser::StatementContext::list() {
  return getRuleContext<KapaQueryParser::ListContext>(0);
}

KapaQueryParser::SchemaContext* KapaQueryParser::StatementContext::schema() {
  return getRuleContext<KapaQueryParser::SchemaContext>(0);
}


size_t KapaQueryParser::StatementContext::getRuleIndex() const {
  return KapaQueryParser::RuleStatement;
}

void KapaQueryParser::StatementContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KapaQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterStatement(this);
}

void KapaQueryParser::StatementContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KapaQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitStatement(this);
}

KapaQueryParser::StatementContext* KapaQueryParser::statement() {
  StatementContext *_localctx = _tracker.createInstance<StatementContext>(_ctx, getState());
  enterRule(_localctx, 2, KapaQueryParser::RuleStatement);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(26);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case KapaQueryParser::T__1:
      case KapaQueryParser::T__2: {
        enterOuterAlt(_localctx, 1);
        setState(24);
        list();
        break;
      }

      case KapaQueryParser::T__0: {
        enterOuterAlt(_localctx, 2);
        setState(25);
        schema();
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

//----------------- SchemaContext ------------------------------------------------------------------

KapaQueryParser::SchemaContext::SchemaContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t KapaQueryParser::SchemaContext::getRuleIndex() const {
  return KapaQueryParser::RuleSchema;
}

void KapaQueryParser::SchemaContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KapaQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterSchema(this);
}

void KapaQueryParser::SchemaContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KapaQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitSchema(this);
}

KapaQueryParser::SchemaContext* KapaQueryParser::schema() {
  SchemaContext *_localctx = _tracker.createInstance<SchemaContext>(_ctx, getState());
  enterRule(_localctx, 4, KapaQueryParser::RuleSchema);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(28);
    match(KapaQueryParser::T__0);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ListContext ------------------------------------------------------------------

KapaQueryParser::ListContext::ListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

KapaQueryParser::TablenameContext* KapaQueryParser::ListContext::tablename() {
  return getRuleContext<KapaQueryParser::TablenameContext>(0);
}

KapaQueryParser::WhereContext* KapaQueryParser::ListContext::where() {
  return getRuleContext<KapaQueryParser::WhereContext>(0);
}


size_t KapaQueryParser::ListContext::getRuleIndex() const {
  return KapaQueryParser::RuleList;
}

void KapaQueryParser::ListContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KapaQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterList(this);
}

void KapaQueryParser::ListContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KapaQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitList(this);
}

KapaQueryParser::ListContext* KapaQueryParser::list() {
  ListContext *_localctx = _tracker.createInstance<ListContext>(_ctx, getState());
  enterRule(_localctx, 6, KapaQueryParser::RuleList);
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
    setState(30);
    _la = _input->LA(1);
    if (!(_la == KapaQueryParser::T__1

    || _la == KapaQueryParser::T__2)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
    setState(31);
    tablename();
    setState(34);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case KapaQueryParser::T__3: {
        setState(32);
        where();
        break;
      }

      case KapaQueryParser::EOF: {
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

//----------------- WhereContext ------------------------------------------------------------------

KapaQueryParser::WhereContext::WhereContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

KapaQueryParser::ExprContext* KapaQueryParser::WhereContext::expr() {
  return getRuleContext<KapaQueryParser::ExprContext>(0);
}


size_t KapaQueryParser::WhereContext::getRuleIndex() const {
  return KapaQueryParser::RuleWhere;
}

void KapaQueryParser::WhereContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KapaQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterWhere(this);
}

void KapaQueryParser::WhereContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KapaQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitWhere(this);
}

KapaQueryParser::WhereContext* KapaQueryParser::where() {
  WhereContext *_localctx = _tracker.createInstance<WhereContext>(_ctx, getState());
  enterRule(_localctx, 8, KapaQueryParser::RuleWhere);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(36);
    match(KapaQueryParser::T__3);
    setState(37);
    expr();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExprContext ------------------------------------------------------------------

KapaQueryParser::ExprContext::ExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

KapaQueryParser::FilterContext* KapaQueryParser::ExprContext::filter() {
  return getRuleContext<KapaQueryParser::FilterContext>(0);
}

std::vector<KapaQueryParser::ExprContext *> KapaQueryParser::ExprContext::expr() {
  return getRuleContexts<KapaQueryParser::ExprContext>();
}

KapaQueryParser::ExprContext* KapaQueryParser::ExprContext::expr(size_t i) {
  return getRuleContext<KapaQueryParser::ExprContext>(i);
}

KapaQueryParser::Logical_operContext* KapaQueryParser::ExprContext::logical_oper() {
  return getRuleContext<KapaQueryParser::Logical_operContext>(0);
}


size_t KapaQueryParser::ExprContext::getRuleIndex() const {
  return KapaQueryParser::RuleExpr;
}

void KapaQueryParser::ExprContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KapaQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterExpr(this);
}

void KapaQueryParser::ExprContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KapaQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitExpr(this);
}

KapaQueryParser::ExprContext* KapaQueryParser::expr() {
  ExprContext *_localctx = _tracker.createInstance<ExprContext>(_ctx, getState());
  enterRule(_localctx, 10, KapaQueryParser::RuleExpr);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(46);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case KapaQueryParser::Identifier: {
        enterOuterAlt(_localctx, 1);
        setState(39);
        filter();
        break;
      }

      case KapaQueryParser::T__4: {
        enterOuterAlt(_localctx, 2);
        setState(40);
        match(KapaQueryParser::T__4);
        setState(41);
        expr();
        setState(42);
        logical_oper();
        setState(43);
        expr();
        setState(44);
        match(KapaQueryParser::T__5);
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

//----------------- FilterContext ------------------------------------------------------------------

KapaQueryParser::FilterContext::FilterContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

KapaQueryParser::ColumnContext* KapaQueryParser::FilterContext::column() {
  return getRuleContext<KapaQueryParser::ColumnContext>(0);
}

KapaQueryParser::OperContext* KapaQueryParser::FilterContext::oper() {
  return getRuleContext<KapaQueryParser::OperContext>(0);
}

tree::TerminalNode* KapaQueryParser::FilterContext::EscapedString() {
  return getToken(KapaQueryParser::EscapedString, 0);
}


size_t KapaQueryParser::FilterContext::getRuleIndex() const {
  return KapaQueryParser::RuleFilter;
}

void KapaQueryParser::FilterContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KapaQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFilter(this);
}

void KapaQueryParser::FilterContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KapaQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFilter(this);
}

KapaQueryParser::FilterContext* KapaQueryParser::filter() {
  FilterContext *_localctx = _tracker.createInstance<FilterContext>(_ctx, getState());
  enterRule(_localctx, 12, KapaQueryParser::RuleFilter);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(48);
    column();
    setState(49);
    oper();
    setState(50);
    match(KapaQueryParser::EscapedString);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TablenameContext ------------------------------------------------------------------

KapaQueryParser::TablenameContext::TablenameContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* KapaQueryParser::TablenameContext::Identifier() {
  return getToken(KapaQueryParser::Identifier, 0);
}


size_t KapaQueryParser::TablenameContext::getRuleIndex() const {
  return KapaQueryParser::RuleTablename;
}

void KapaQueryParser::TablenameContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KapaQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterTablename(this);
}

void KapaQueryParser::TablenameContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KapaQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitTablename(this);
}

KapaQueryParser::TablenameContext* KapaQueryParser::tablename() {
  TablenameContext *_localctx = _tracker.createInstance<TablenameContext>(_ctx, getState());
  enterRule(_localctx, 14, KapaQueryParser::RuleTablename);

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
    match(KapaQueryParser::Identifier);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ColumnContext ------------------------------------------------------------------

KapaQueryParser::ColumnContext::ColumnContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* KapaQueryParser::ColumnContext::Identifier() {
  return getToken(KapaQueryParser::Identifier, 0);
}


size_t KapaQueryParser::ColumnContext::getRuleIndex() const {
  return KapaQueryParser::RuleColumn;
}

void KapaQueryParser::ColumnContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KapaQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterColumn(this);
}

void KapaQueryParser::ColumnContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KapaQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitColumn(this);
}

KapaQueryParser::ColumnContext* KapaQueryParser::column() {
  ColumnContext *_localctx = _tracker.createInstance<ColumnContext>(_ctx, getState());
  enterRule(_localctx, 16, KapaQueryParser::RuleColumn);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(54);
    match(KapaQueryParser::Identifier);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- OperContext ------------------------------------------------------------------

KapaQueryParser::OperContext::OperContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* KapaQueryParser::OperContext::REGEX() {
  return getToken(KapaQueryParser::REGEX, 0);
}

tree::TerminalNode* KapaQueryParser::OperContext::EQUAL() {
  return getToken(KapaQueryParser::EQUAL, 0);
}

tree::TerminalNode* KapaQueryParser::OperContext::NOT_EQUAL() {
  return getToken(KapaQueryParser::NOT_EQUAL, 0);
}

tree::TerminalNode* KapaQueryParser::OperContext::STARS_WITH() {
  return getToken(KapaQueryParser::STARS_WITH, 0);
}

tree::TerminalNode* KapaQueryParser::OperContext::END_WITH() {
  return getToken(KapaQueryParser::END_WITH, 0);
}


size_t KapaQueryParser::OperContext::getRuleIndex() const {
  return KapaQueryParser::RuleOper;
}

void KapaQueryParser::OperContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KapaQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterOper(this);
}

void KapaQueryParser::OperContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KapaQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitOper(this);
}

KapaQueryParser::OperContext* KapaQueryParser::oper() {
  OperContext *_localctx = _tracker.createInstance<OperContext>(_ctx, getState());
  enterRule(_localctx, 18, KapaQueryParser::RuleOper);
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
    setState(56);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 3968) != 0))) {
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

//----------------- Logical_operContext ------------------------------------------------------------------

KapaQueryParser::Logical_operContext::Logical_operContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* KapaQueryParser::Logical_operContext::AND() {
  return getToken(KapaQueryParser::AND, 0);
}

tree::TerminalNode* KapaQueryParser::Logical_operContext::OR() {
  return getToken(KapaQueryParser::OR, 0);
}


size_t KapaQueryParser::Logical_operContext::getRuleIndex() const {
  return KapaQueryParser::RuleLogical_oper;
}

void KapaQueryParser::Logical_operContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KapaQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterLogical_oper(this);
}

void KapaQueryParser::Logical_operContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KapaQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitLogical_oper(this);
}

KapaQueryParser::Logical_operContext* KapaQueryParser::logical_oper() {
  Logical_operContext *_localctx = _tracker.createInstance<Logical_operContext>(_ctx, getState());
  enterRule(_localctx, 20, KapaQueryParser::RuleLogical_oper);
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
    setState(58);
    _la = _input->LA(1);
    if (!(_la == KapaQueryParser::AND

    || _la == KapaQueryParser::OR)) {
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

void KapaQueryParser::initialize() {
  ::antlr4::internal::call_once(kapaqueryParserOnceFlag, kapaqueryParserInitialize);
}
