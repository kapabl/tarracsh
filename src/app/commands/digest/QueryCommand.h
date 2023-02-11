#ifndef TARRACSH_QUERY_H
#define TARRACSH_QUERY_H

#include "../../App.h"
#include "../infrastructure/db/Database.h"
#include "../domain/db/CallGraphDb.h"
#include "../domain/db/DigestDb.h"
#include "../domain/stats/Results.h"


namespace kapa::tarracsh::app::commands::digest {


#define TQ_SCHEMA "schema"
#define TQ_LIST "list"


class QueryCommand {
public:
    static bool run(Context& config);

private:
    QueryCommand(Context& config);


    void executeListQuery(const std::vector<std::string>& parts) const;

    [[nodiscard]] bool execute() const;

    std::unique_ptr<infrastructure::db::Database> _db;
    domain::Options &_options;
    domain::stats::Results& _results;
};


inline bool QueryCommand::run(Context &config) {
    const QueryCommand queryCommand(config);
    const auto result = queryCommand.execute();
    return result;
}

inline QueryCommand::QueryCommand(Context &config): _options(config.getOptions()), _results(config.getResults()) {

    if (_options.isPublicDigest) {
        _db = std::make_unique<domain::db::digest::DigestDb>(_options.outputDir, config.getLog(), false );
    } else {
        _db = std::make_unique<domain::db::callgraph::CallGraphDb>(_options.outputDir, config.getLog());
    }
    _db->init();

}

inline void QueryCommand::executeListQuery(const std::vector<std::string>& parts) const {
    if ( parts.size() <= 1) {
        _results.log->writeln("Invalid queryValue list command, expecting table name", true);
        return;
    } 
    _db->read();
    _db->list(parts[1], _options.digest.displayRaw );
}

inline bool QueryCommand::execute() const {
    auto result = true;
    _db->init();
    const std::vector<std::string> parts = infrastructure::string::stringUtils::split(_options.digest.queryValue, " ");
    const auto& command = parts[0];
    if (command == TQ_SCHEMA) {
        _db->printSchema();
    } else if (command == TQ_LIST) {
        executeListQuery(parts);
    } else {
        result = false;
        _results.log->writeln("Invalid queryValue command", true);
    }

    return result;
}
}


#endif
