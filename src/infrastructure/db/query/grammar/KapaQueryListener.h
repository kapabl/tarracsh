
// Generated from KapaQuery.g4 by ANTLR 4.12.0

#pragma once


#include "antlr4-runtime.h"
#include "KapaQueryParser.h"


namespace kapa::infrastructure::db::query {

/**
 * This interface defines an abstract listener for a parse tree produced by KapaQueryParser.
 */
class  KapaQueryListener : public antlr4::tree::ParseTreeListener {
public:

  virtual void enterQuery(KapaQueryParser::QueryContext *ctx) = 0;
  virtual void exitQuery(KapaQueryParser::QueryContext *ctx) = 0;

  virtual void enterStatement(KapaQueryParser::StatementContext *ctx) = 0;
  virtual void exitStatement(KapaQueryParser::StatementContext *ctx) = 0;

  virtual void enterSchema(KapaQueryParser::SchemaContext *ctx) = 0;
  virtual void exitSchema(KapaQueryParser::SchemaContext *ctx) = 0;

  virtual void enterList(KapaQueryParser::ListContext *ctx) = 0;
  virtual void exitList(KapaQueryParser::ListContext *ctx) = 0;

  virtual void enterWhere(KapaQueryParser::WhereContext *ctx) = 0;
  virtual void exitWhere(KapaQueryParser::WhereContext *ctx) = 0;

  virtual void enterExpr(KapaQueryParser::ExprContext *ctx) = 0;
  virtual void exitExpr(KapaQueryParser::ExprContext *ctx) = 0;

  virtual void enterFilter(KapaQueryParser::FilterContext *ctx) = 0;
  virtual void exitFilter(KapaQueryParser::FilterContext *ctx) = 0;

  virtual void enterTablename(KapaQueryParser::TablenameContext *ctx) = 0;
  virtual void exitTablename(KapaQueryParser::TablenameContext *ctx) = 0;

  virtual void enterColumn(KapaQueryParser::ColumnContext *ctx) = 0;
  virtual void exitColumn(KapaQueryParser::ColumnContext *ctx) = 0;

  virtual void enterOper(KapaQueryParser::OperContext *ctx) = 0;
  virtual void exitOper(KapaQueryParser::OperContext *ctx) = 0;

  virtual void enterLogical_oper(KapaQueryParser::Logical_operContext *ctx) = 0;
  virtual void exitLogical_oper(KapaQueryParser::Logical_operContext *ctx) = 0;


};

}  // namespace kapa::infrastructure::db::query
