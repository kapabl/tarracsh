#include "ListenerImpl.h"
#include <format>
#include <regex>

#include "../../Database.h"
#include "../../table/Table.h"
#include "../../../string/StringUtils.h"


using namespace kapa::infrastructure::db::query;

ListenerImpl::ListenerImpl(Database &db, const bool displayRaw)
    : _db(db), _displayRaw(displayRaw) {
}

void ListenerImpl::enterQuery(KapaQueryParser::QueryContext *queryContext) {
    // TODO
}

void ListenerImpl::exitQuery(KapaQueryParser::QueryContext *queryContext) {
    // TODO
}

void ListenerImpl::enterStatement(KapaQueryParser::StatementContext *statementContext) {
    // TODO
}

void ListenerImpl::exitStatement(KapaQueryParser::StatementContext *statementContext) {
    // TODO
}

void ListenerImpl::enterSchema(KapaQueryParser::SchemaContext *schemaContext) {
    // TODO
}

void ListenerImpl::exitSchema(KapaQueryParser::SchemaContext *schemaContext) {
    _db.printSchema();
}

void ListenerImpl::enterList(KapaQueryParser::ListContext *listContext) {

    _db.read();
    const auto tablename = listContext->tablename()->getText();
    _mainTable = _db.getTable(tablename);
    if (_mainTable == nullptr) {
        _semanticErrors.push_back(std::format("Invalid table name: {}", tablename));
    }
}

void ListenerImpl::exitList(KapaQueryParser::ListContext *listContext) {
    if (!hasSemanticErrors()) {
        _mainTable->list([this](tables::AutoIncrementedRow &row) -> bool {
            const auto result = _where(row);
            return result;
        }, _displayRaw);
    } else {
        for (auto error : _semanticErrors) {
            _db.log().writeln(error, true);
        }
    }
}

void ListenerImpl::enterWhere(KapaQueryParser::WhereContext *whereContext) {
    _where = [this, whereContext](tables::AutoIncrementedRow &row)-> bool {
        const bool result = _rulePredicates[whereContext->expr()](row);
        return result;
    };

}

void ListenerImpl::exitWhere(KapaQueryParser::WhereContext *whereContext) {
    // TODO
}

void ListenerImpl::enterExpr(KapaQueryParser::ExprContext *exprContext) {
    auto *filterContext = exprContext->filter();
    if (filterContext != nullptr) {
        _rulePredicates[exprContext] = [this, filterContext](tables::AutoIncrementedRow &row)-> bool {
            const bool result = _rulePredicates[filterContext](row);
            return result;
        };
    } else {
        _rulePredicates[exprContext] = [this, exprContext](tables::AutoIncrementedRow &row)-> bool {
            const bool left = _rulePredicates[exprContext->expr()[0]](row);
            if (exprContext->logical_oper()->AND() != nullptr) {
                if (!left) return false;
                return _rulePredicates[exprContext->expr()[1]](row);
            }

            if (exprContext->logical_oper()->OR() != nullptr) {
                if (left) return false;
                return _rulePredicates[exprContext->expr()[1]](row);
            }

            return false;
        };
    }

}

void ListenerImpl::exitExpr(KapaQueryParser::ExprContext *exprContext) {
    // TODO
}

std::regex *ListenerImpl::getRegex(const std::string &right, const bool isCaseInsensitive) const {
    if (!_regexCache.contains(right)) {
        std::regex_constants::syntax_option_type flags = std::regex_constants::ECMAScript;
        if (isCaseInsensitive) {
            flags |= std::regex_constants::icase;
        }
        _regexCache[right] = std::make_unique<std::regex>(right, flags);
    }
    std::regex* result = &*_regexCache[right];
    return result;

}

std::string ListenerImpl::getSanitizedString(const std::string &value) const {
    if (!_sanitizedStringCache.contains(value)) {
        _sanitizedStringCache[value] =
            string::stringUtils::replaceAll(string::stringUtils::trim(value, "'"), "\\\\", "\\");
    }
    std::string result = _sanitizedStringCache[value];
    return result;

}

void ListenerImpl::enterFilter(KapaQueryParser::FilterContext *filterContext) {

}

std::string ListenerImpl::getColumnValue(const tables::AutoIncrementedRow &row, const std::string &columnName) const {
    auto result = _mainTable->getColumnValue(row.id, columnName.c_str());
    return result;
}

void ListenerImpl::exitFilter(KapaQueryParser::FilterContext *filterContext) {
    auto columnName = filterContext->column()->getText();
    const auto right = filterContext->EscapedString()->getText();
    auto sanitizedRight = getSanitizedString(right);
    auto oper = filterContext->oper();

    const std::regex* filterRegex{ nullptr };

    if( oper->REGEX() != nullptr) {
        const auto isCaseInsensitive = sanitizedRight.ends_with("/i");
        if (isCaseInsensitive) {
            sanitizedRight = sanitizedRight.substr(0, sanitizedRight.length() - 2);
        }
        filterRegex = getRegex(sanitizedRight, isCaseInsensitive);
    }

    _rulePredicates[filterContext] = [this,
            columnName,
            oper,
            filterRegex,
            sanitizedRight](auto &row) -> bool {

            auto left = getColumnValue(row, columnName);
            bool result;
            if (filterRegex != nullptr) {
                result = regex_search(left, *filterRegex);
            } else if (oper->NOT_EQUAL() != nullptr) {
                result = left != sanitizedRight;
            } else if (oper->STARS_WITH() != nullptr) {
                result = left.starts_with(sanitizedRight) == 0;
            } else if (oper->END_WITH() != nullptr) {
                result = left.ends_with(sanitizedRight);
            } else {
                result = left == sanitizedRight;
            }
            return result;
        };
}

void ListenerImpl::enterTablename(KapaQueryParser::TablenameContext *tablenameContext) {
    // TODO
}

void ListenerImpl::exitTablename(KapaQueryParser::TablenameContext *tablenameContext) {
    // TODO
}

void ListenerImpl::enterColumn(KapaQueryParser::ColumnContext *columnContext) {
    const auto columnName = columnContext->getText();
    if (_mainTable != nullptr && !_mainTable->isValidColumn(columnName)) {
        _semanticErrors.push_back(std::format("Invalid column name: {}", columnName));
    }
}

void ListenerImpl::exitColumn(KapaQueryParser::ColumnContext *columnContext) {
    // TODO
}

void ListenerImpl::enterEveryRule(antlr4::ParserRuleContext *parserRuleContext) {
    // TODO
}

void ListenerImpl::exitEveryRule(antlr4::ParserRuleContext *parserRuleContext) {
    // TODO

}

void ListenerImpl::visitTerminal(antlr4::tree::TerminalNode *terminalNode) {
    // TODO
}

void ListenerImpl::visitErrorNode(antlr4::tree::ErrorNode *errorNode) {
    // TODO
}
