#ifndef KAPA_DB_QUERY_LISTENER
#define KAPA_DB_QUERY_LISTENER


#include "antlr4-runtime.h"
#include "KapaQueryBaseListener.h"


namespace kapa::infrastructure::db::query {

class ListenerImpl final : public KapaQueryBaseListener {
public:

  virtual void enterQuery(KapaQueryParser::QueryContext * /*ctx*/) override;
  virtual void exitQuery(KapaQueryParser::QueryContext * /*ctx*/) override;

  virtual void enterStatement(KapaQueryParser::StatementContext * /*ctx*/) override;
  virtual void exitStatement(KapaQueryParser::StatementContext * /*ctx*/) override;

  virtual void enterSchema(KapaQueryParser::SchemaContext * /*ctx*/) override;
  virtual void exitSchema(KapaQueryParser::SchemaContext * /*ctx*/) override;

  virtual void enterList(KapaQueryParser::ListContext * /*ctx*/) override;
  virtual void exitList(KapaQueryParser::ListContext * /*ctx*/) override;

  virtual void enterWhere(KapaQueryParser::WhereContext * /*ctx*/) override;
  virtual void exitWhere(KapaQueryParser::WhereContext * /*ctx*/) override;

  virtual void enterExpr(KapaQueryParser::ExprContext * /*ctx*/) override;
  virtual void exitExpr(KapaQueryParser::ExprContext * /*ctx*/) override;

  virtual void enterFilter(KapaQueryParser::FilterContext * /*ctx*/) override;
  virtual void exitFilter(KapaQueryParser::FilterContext * /*ctx*/) override;

  virtual void enterTablename(KapaQueryParser::TablenameContext * /*ctx*/) override;
  virtual void exitTablename(KapaQueryParser::TablenameContext * /*ctx*/) override;

  virtual void enterColumn(KapaQueryParser::ColumnContext * /*ctx*/) override;
  virtual void exitColumn(KapaQueryParser::ColumnContext * /*ctx*/) override;


  virtual void enterEveryRule(antlr4::ParserRuleContext * /*ctx*/) override;
  virtual void exitEveryRule(antlr4::ParserRuleContext * /*ctx*/) override;
  virtual void visitTerminal(antlr4::tree::TerminalNode * /*node*/) override;
  virtual void visitErrorNode(antlr4::tree::ErrorNode * /*node*/) override;

};

}  // namespace kapa::infrastructure::db::query
#endif