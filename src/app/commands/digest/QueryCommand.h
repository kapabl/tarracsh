#ifndef TARRACSH_QUERY_H
#define TARRACSH_QUERY_H

#include "../../App.h"
#include "../infrastructure/db/Database.h"
#include "../domain/db/CallGraphDb.h"
#include "../domain/db/DigestDb.h"
#include "../domain/stats/Results.h"


namespace kapa::tarracsh::app::commands::digest {


#define TQ_SCHEMA "schema"


class QueryCommand {
public:
    static bool run(Context& config) {
        const QueryCommand queryCommand(config);
        const auto result = queryCommand.execute();
        return result;
    }

private:
    QueryCommand(Context& config)
        : _options(config.getOptions()), _results(config.getResults()) {

        if (_options.isPublicDigest) {
            _db = std::make_unique<domain::db::digest::DigestDb>(_options.outputDir, config.getLog(), false );
        } else {
            _db = std::make_unique<domain::db::callgraph::CallGraphDb>(_options.outputDir, config.getLog());
        }
        _db->init();

    }


    [[nodiscard]] bool execute() const {
        _db->init();
        const auto result = _options.digest.queryValue == TQ_SCHEMA;
        if ( result ) {
            _db->printSchema();
        }
        else {
            _results.log->writeln("Invalid queryValue command");
        }

        return result;
    }

    std::unique_ptr<infrastructure::db::Database> _db;
    domain::Options &_options;
    domain::stats::Results& _results;
};


}


#endif
