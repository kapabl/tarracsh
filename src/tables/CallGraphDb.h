#ifndef TARRACSH_CALL_GRAPH_DB_H
#define TARRACSH_CALL_GRAPH_DB_H
#include <filesystem>
#include <string>
#include "Database.h"


namespace org::kapa::tarracsh::db {

class CallGraphDb : public Database {
public:
    CallGraphDb(const std::string& dataDir)
        : Database(dataDir) {

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
