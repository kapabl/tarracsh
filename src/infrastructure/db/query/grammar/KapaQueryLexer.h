
// Generated from KapaQuery.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"


namespace kapa::infrastructure::db::query {


class  KapaQueryLexer : public antlr4::Lexer {
public:
  enum {
    T__0 = 1, T__1 = 2, T__2 = 3, T__3 = 4, T__4 = 5, T__5 = 6, NOT = 7, 
    NOT_EQUAL = 8, EQUAL = 9, REGEX = 10, STARTS_WITH = 11, ENDS_WITH = 12, 
    AND = 13, OR = 14, Identifier = 15, WS = 16, EscapedString = 17
  };

  explicit KapaQueryLexer(antlr4::CharStream *input);

  ~KapaQueryLexer() override;


  std::string getGrammarFileName() const override;

  const std::vector<std::string>& getRuleNames() const override;

  const std::vector<std::string>& getChannelNames() const override;

  const std::vector<std::string>& getModeNames() const override;

  const antlr4::dfa::Vocabulary& getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;

  const antlr4::atn::ATN& getATN() const override;

  // By default the static state used to implement the lexer is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:

  // Individual action functions triggered by action() above.

  // Individual semantic predicate functions triggered by sempred() above.

};

}  // namespace kapa::infrastructure::db::query
