#ifndef TARRACSH_QUERY_H
#define TARRACSH_QUERY_H

#include <functional>

#include "app/AppRuntime.h"
#include "infrastructure/db/Database.h"
#include "infrastructure/db/query/QueryApi.h"
#include "domain/db/CallGraphDb.h"
#include "domain/db/DigestDb.h"
#include "domain/stats/Results.h"


namespace kapa::tarracsh::app::commands {

using infrastructure::db::Database;


class Query {
public:
    using DatabaseFactory = std::function<std::unique_ptr<Database>(const Database::Config &, bool useDigest)>;
    using QueryExecutor = std::function<bool(Database &, const std::string &, bool displayRaw)>;

    /// Executes a query against the digest database when `--public-digest` is set, otherwise the call-graph database.
    static bool run(Context &config);
    static DatabaseFactory setDatabaseFactoryForTests(DatabaseFactory replacement);
    static QueryExecutor setQueryExecutorForTests(QueryExecutor replacement);

private:
    explicit Query(Context &context);

    [[nodiscard]] bool execute() const;

    std::unique_ptr<Database> _db;
    domain::Options &_options;
    domain::stats::Results &_results;

    static DatabaseFactory &databaseFactory();
    static QueryExecutor &queryExecutor();
};


inline bool Query::run(Context &config) {
    const Query queryCommand(config);
    const auto result = queryCommand.execute();
    return result;
}

inline Query::Query(Context &context)
    : _options(context.getOptions()), _results(context.getResults()) {

    Database::Config dbConfig{_options.outputDir, &context.getLog()};
    _db = databaseFactory()(dbConfig, _options.isPublicDigest);
    _db->init();

}

inline bool Query::execute() const {
    return queryExecutor()(
        *_db, _options.getSubCommandOptions().queryValue, _options.getSubCommandOptions().displayRaw);
}

inline Query::DatabaseFactory &Query::databaseFactory() {
    static DatabaseFactory factory = [](const Database::Config &config, const bool useDigest) {
        if (useDigest) {
            return std::unique_ptr<Database>(new domain::db::digest::DigestDb(config, false));
        }
        return std::unique_ptr<Database>(new domain::db::callgraph::CallGraphDb(config, false));
    };
    return factory;
}

inline Query::QueryExecutor &Query::queryExecutor() {
    static QueryExecutor executor = [](Database &db, const std::string &query, const bool displayRaw) {
        return infrastructure::db::query::ExecuteQuery(db, query, displayRaw);
    };
    return executor;
}

inline Query::DatabaseFactory Query::setDatabaseFactoryForTests(DatabaseFactory replacement) {
    auto &factory = databaseFactory();
    auto previous = std::move(factory);
    factory = std::move(replacement);
    return previous;
}

inline Query::QueryExecutor Query::setQueryExecutorForTests(QueryExecutor replacement) {
    auto &executor = queryExecutor();
    auto previous = std::move(executor);
    executor = std::move(replacement);
    return previous;
}
}


#endif
