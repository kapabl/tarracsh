#ifndef TARRACSH_CALL_GRAPH_DB_H
#define TARRACSH_CALL_GRAPH_DB_H
#include <filesystem>
#include <string>
#include "../infrastructure/db/Database.h"


namespace kapa::tarracsh::domain::db::callgraph {

class CallGraphDb : public infrastructure::db::Database {
public:
    explicit CallGraphDb(const Config& config)
        : Database(config) {

    }

    void init() override;
    static std::shared_ptr<CallGraphDb> create(const Config& config, bool doClean);
private:
};


}


#endif
