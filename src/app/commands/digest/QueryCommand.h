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
    static void run(app::App& app) {
        const QueryCommand queryCommand(app);
        queryCommand.execute();
    }

private:
    QueryCommand(app::App& app)
        : _options(app.getOptions()), _results(app.getResults()) {

        if (_options.isPublicDigest) {
            _db = std::make_unique<domain::db::digest::DigestDb>(_options.outputDir, app.getLog() );
        } else {
            _db = std::make_unique<domain::db::callgraph::CallGraphDb>(_options.outputDir, app.getLog());
        }
        _db->init();

    }


    void execute() const {
        _db->init();
        if ( _options.queryValue == TQ_SCHEMA ) {
            _db->printSchema();
        }
        else {
            _results.log->writeln("Invalid queryValue command");
        }
    }

    std::unique_ptr<infrastructure::db::Database> _db;
    domain::Options &_options;
    domain::stats::Results& _results;
};


}


#endif
