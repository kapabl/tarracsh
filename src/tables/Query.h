#ifndef TARRACSH_QUERY_H
#define TARRACSH_QUERY_H

#include "../app/Tarracsh.h"
#include "../tables/Database.h"


namespace org::kapa::tarracsh::db::query {


#define TQ_SCHEMA "schema"


class QueryCommand {
public:
    static void run(app::Config& config) {
        const QueryCommand queryCommand(config);
        queryCommand.execute();
    }

private:
    QueryCommand(app::Config& config)
        : _options(config.getOptions()), _results(config.getResults()) {

        if (_options.isPublicDigest) {
            _db = std::make_unique<DigestDb>(_options.outputDir);
        } else {
            _db = std::make_unique<CallGraphDb>(_options.outputDir);
        }
        _db->init();

    }


    void execute() const {
        _db->init();
        if ( _options.queryValue == TQ_SCHEMA ) {
            _db->printSchema();
        }
        else {
            _results.log.writeln("Invalid queryValue command");
        }
    }

    std::unique_ptr<Database> _db;
    Options &_options;
    stats::Results& _results;
    // DigestDb _digestDb;
    // CallGraphDb _callGraphDb;
};


}


#endif
