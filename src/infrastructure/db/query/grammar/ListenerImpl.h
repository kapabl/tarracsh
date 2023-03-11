#ifndef KAPA_DB_QUERY_LISTENER
#define KAPA_DB_QUERY_LISTENER

#include <functional>
#include <regex>

#include "../../Database.h"
#include "../../table/Table.h"
#include "antlr4-runtime.h"
#include "KapaQueryBaseListener.h"


namespace kapa::infrastructure::db::query {

    class ListenerImpl final : public KapaQueryBaseListener {
    public:
        explicit ListenerImpl(Database& db, const bool displayRaw);

        void enterQuery(KapaQueryParser::QueryContext* /*ctx*/) override;
        void exitQuery(KapaQueryParser::QueryContext* /*ctx*/) override;

        void enterStatement(KapaQueryParser::StatementContext* /*ctx*/) override;
        void exitStatement(KapaQueryParser::StatementContext* /*ctx*/) override;

        void enterSchema(KapaQueryParser::SchemaContext* /*ctx*/) override;
        void exitSchema(KapaQueryParser::SchemaContext* /*ctx*/) override;

        void enterList(KapaQueryParser::ListContext* /*ctx*/) override;
        void exitList(KapaQueryParser::ListContext* /*ctx*/) override;

        void enterWhere(KapaQueryParser::WhereContext* /*ctx*/) override;
        void exitWhere(KapaQueryParser::WhereContext* /*ctx*/) override;

        void enterExpr(KapaQueryParser::ExprContext* /*ctx*/) override;
        void exitExpr(KapaQueryParser::ExprContext* /*ctx*/) override;

       
        void enterFilter(KapaQueryParser::FilterContext* /*ctx*/) override;
        void exitFilter(KapaQueryParser::FilterContext* /*ctx*/) override;

        void enterTablename(KapaQueryParser::TablenameContext* /*ctx*/) override;
        void exitTablename(KapaQueryParser::TablenameContext* /*ctx*/) override;

        void enterColumn(KapaQueryParser::ColumnContext* /*ctx*/) override;
        void exitColumn(KapaQueryParser::ColumnContext* /*ctx*/) override;


        void enterEveryRule(antlr4::ParserRuleContext* /*ctx*/) override;
        void exitEveryRule(antlr4::ParserRuleContext* /*ctx*/) override;
        void visitTerminal(antlr4::tree::TerminalNode* /*node*/) override;
        void visitErrorNode(antlr4::tree::ErrorNode* /*node*/) override;

    private:
        Database& _db;
        bool _displayRaw;
        std::vector<std::string> _semanticErrors;

        mutable std::unordered_map<std::string, std::unique_ptr<std::regex>> _regexCache;
        mutable std::unordered_map<std::string, std::string> _sanitizedStringCache;

        typedef std::function<bool(tables::AutoIncrementedRow&)> RowPredicate;
        RowPredicate _where = [](auto& row) -> bool { return true; };
        //RowPredicate _whereExpr = [](auto& row) -> bool { return true; };
        tables::Table * _mainTable{nullptr};
        [[nodiscard]] std::string getColumnValue(const tables::AutoIncrementedRow& row, const std::string& columnName) const;

        std::unordered_map<antlr4::ParserRuleContext*, RowPredicate> _rulePredicates;

        [[nodiscard]] bool hasSemanticErrors() const { return _semanticErrors.size() > 0; }

        [[nodiscard]] std::regex *getRegex(const std::string &right, const bool isCaseInsensitive) const;
        std::string getSanitizedString(const std::string &value) const;

    };

} // namespace kapa::infrastructure::db::query
#endif
