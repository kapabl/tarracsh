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
using namespace kapa::infrastructure::string::stringUtils;


Engine::Engine(Database &db)
    : _db(db) {
}

void Engine::log(const std::string &msg, const bool doStdout) const {
    _db.log().writeln(msg, true);
}

bool Engine::execute(const std::string &query, const bool displayRaw) const {

    antlr4::ANTLRInputStream input(query.c_str());

    KapaQueryLexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);
    KapaQueryParser parser(&tokens);

    antlr4::tree::ParseTree *queryParseTree = parser.query();

    auto result = parser.getNumberOfSyntaxErrors() == 0;

    if (result) {
        ListenerImpl listener(_db, displayRaw);
        antlr4::tree::ParseTreeWalker::DEFAULT.walk(&listener, queryParseTree);
    } else {
        _db.log().writeln("Query error", true);
    }

    return result;

}
