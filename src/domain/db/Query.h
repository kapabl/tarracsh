#ifndef TARRACSH_QUERY_H
#define TARRACSH_QUERY_H

#include "../app/Tarracsh.h"
#include "../infrastructure/db/Database.h"


namespace kapa::tarracsh::domain::db::query {


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
            _db = std::make_unique<digest::DigestDb>(_options.outputDir, app.getLog() );
        } else {
            _db = std::make_unique<callgraph::CallGraphDb>(_options.outputDir, app.getLog());
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
    Options &_options;
    stats::Results& _results;
    // DigestDb _digestDb;
    // CallGraphDb _callGraphDb;
};


}


#endif
