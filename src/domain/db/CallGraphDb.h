#ifndef TARRACSH_CALL_GRAPH_DB_H
#define TARRACSH_CALL_GRAPH_DB_H
#include <filesystem>
#include <string>
#include "../infrastructure/db/Database.h"


namespace kapa::tarracsh::domain::db::callgraph {

class CallGraphDb : public infrastructure::db::Database {
public:
    CallGraphDb(const std::string& dataDir, infrastructure::log::Log& log)
        : Database(dataDir, log) {

    }

    void init() override;
    void clean() override;
    bool read() override;
    bool write() override;

    void printSchema() override;
private:
};


}


#endif
