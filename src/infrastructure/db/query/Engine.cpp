#include "Engine.h"
#include <vector>
#include "../../string/StringUtils.h"
#include "../Database.h"
#include "../table/Table.h"
#include <antlr4-runtime/antlr4-runtime.h>
#include "grammar/KapaQueryLexer.h"
#include "grammar/KapaQueryParser.h"
#include "grammar/ListenerImpl.h"


using namespace kapa::infrastructure::db::query;
using kapa::infrastructure::db::tables::Table;
using namespace kapa::infrastructure::string::stringUtils;


Engine::Engine(Database &db)
    : _db(db) {
}

void Engine::log(const std::string &msg, const bool doStdout) const {
    _db.log().writeln(msg, true);
}

void Engine::executeListQuery(const std::vector<std::string> &parts, const bool displayRaw) const {
    if (parts.size() <= 1) {
        log("Invalid queryValue list command, expecting table name", true);
        return;
    }
    _db.read();
    list(parts[1], displayRaw);
}

void Engine::list(const std::string &tablename, const bool displayRaw) const {
    Table *table = _db.getTable(tablename);
    if (table != nullptr) {
        table->list(displayRaw);
    } else {
        log(format("Invalid table name: {}", tablename));
    }

}

bool Engine::execute(const std::string &query, const bool displayRaw) {
    auto result = true;
    const std::vector<std::string> parts = split(query, " ");
    const auto &command = parts[0];
    if (command == TQ_SCHEMA) {
        _db.printSchema();
    } else if (command == TQ_LIST) {
        executeListQuery(parts, displayRaw);
    } else {
        result = false;
        _db.log().writeln("Invalid queryValue command", true);
    }

    return result;

}

bool Engine::execute2(const std::string &query, const bool displayRaw) {

    antlr4::ANTLRInputStream input(query.c_str());

    KapaQueryLexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);
    KapaQueryParser parser(&tokens);

    antlr4::tree::ParseTree *queryParseTree = parser.query();

    auto result = parser.getNumberOfSyntaxErrors() == 0;

    if (result) {
        ListenerImpl listener;
        antlr4::tree::ParseTreeWalker::DEFAULT.walk(&listener, queryParseTree);
        //TODO execute
    } else {
        _db.log().writeln("Query error", true);
    }

    return result;

}
