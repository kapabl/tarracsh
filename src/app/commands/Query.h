#ifndef TARRACSH_QUERY_H
#define TARRACSH_QUERY_H

#include "app/App.h"
#include "infrastructure/db/Database.h"
#include "domain/db/CallGraphDb.h"
#include "domain/db/DigestDb.h"
#include "domain/stats/Results.h"


namespace kapa::tarracsh::app::commands {

using infrastructure::db::Database;


class Query {
public:
    static bool run(Context &config);

private:
    explicit Query(Context &context);

    [[nodiscard]] bool execute() const;

    std::unique_ptr<Database> _db;
    domain::Options &_options;
    domain::stats::Results &_results;
};


inline bool Query::run(Context &config) {
    const Query queryCommand(config);
    const auto result = queryCommand.execute();
    return result;
}

inline Query::Query(Context &context)
    : _options(context.getOptions()), _results(context.getResults()) {

    Database::Config dbConfig{_options.outputDir, &context.getLog()};
    if (_options.isPublicDigest) {
        _db = std::make_unique<domain::db::digest::DigestDb>(dbConfig, false);
    } else {
        _db = std::make_unique<domain::db::callgraph::CallGraphDb>(dbConfig, false);
    }
    _db->init();

}

inline bool Query::execute() const {
    //_db->init();
    const auto result = _db->executeQuery(_options.getSubCommandOptions().queryValue, _options.getSubCommandOptions().displayRaw);
    return result;
}
}


#endif
