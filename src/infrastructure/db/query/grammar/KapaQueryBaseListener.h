
// Generated from KapaQuery.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"
#include "KapaQueryListener.h"


namespace kapa::infrastructure::db::query {

/**
 * This class provides an empty implementation of KapaQueryListener,
 * which can be extended to create a listener which only needs to handle a subset
 * of the available methods.
 */
class  KapaQueryBaseListener : public KapaQueryListener {
public:

  virtual void enterQuery(KapaQueryParser::QueryContext * /*ctx*/) override { }
  virtual void exitQuery(KapaQueryParser::QueryContext * /*ctx*/) override { }

  virtual void enterStatement(KapaQueryParser::StatementContext * /*ctx*/) override { }
  virtual void exitStatement(KapaQueryParser::StatementContext * /*ctx*/) override { }

  virtual void enterSchema(KapaQueryParser::SchemaContext * /*ctx*/) override { }
  virtual void exitSchema(KapaQueryParser::SchemaContext * /*ctx*/) override { }

  virtual void enterList(KapaQueryParser::ListContext * /*ctx*/) override { }
  virtual void exitList(KapaQueryParser::ListContext * /*ctx*/) override { }

  virtual void enterWhere(KapaQueryParser::WhereContext * /*ctx*/) override { }
  virtual void exitWhere(KapaQueryParser::WhereContext * /*ctx*/) override { }

  virtual void enterExpr(KapaQueryParser::ExprContext * /*ctx*/) override { }
  virtual void exitExpr(KapaQueryParser::ExprContext * /*ctx*/) override { }

  virtual void enterFilter(KapaQueryParser::FilterContext * /*ctx*/) override { }
  virtual void exitFilter(KapaQueryParser::FilterContext * /*ctx*/) override { }

  virtual void enterTablename(KapaQueryParser::TablenameContext * /*ctx*/) override { }
  virtual void exitTablename(KapaQueryParser::TablenameContext * /*ctx*/) override { }

  virtual void enterColumn(KapaQueryParser::ColumnContext * /*ctx*/) override { }
  virtual void exitColumn(KapaQueryParser::ColumnContext * /*ctx*/) override { }

  virtual void enterOper(KapaQueryParser::OperContext * /*ctx*/) override { }
  virtual void exitOper(KapaQueryParser::OperContext * /*ctx*/) override { }

  virtual void enterLogical_oper(KapaQueryParser::Logical_operContext * /*ctx*/) override { }
  virtual void exitLogical_oper(KapaQueryParser::Logical_operContext * /*ctx*/) override { }


  virtual void enterEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void exitEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void visitTerminal(antlr4::tree::TerminalNode * /*node*/) override { }
  virtual void visitErrorNode(antlr4::tree::ErrorNode * /*node*/) override { }

};

}  // namespace kapa::infrastructure::db::query
