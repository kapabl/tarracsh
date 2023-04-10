#ifndef TARRACSH_CALL_GRAPH_DB_H
#define TARRACSH_CALL_GRAPH_DB_H
#include <filesystem>
#include "infrastructure/db/Database.h"
#include "table/FilesTable.h"
#include "table/ClassfilesTable.h"


namespace kapa::tarracsh::domain::db::callgraph {

class CallGraphDb : public infrastructure::db::Database {
public:
    explicit CallGraphDb(const Config& config)
        : Database(config) {

    }

    void init() override;
    static std::shared_ptr<CallGraphDb> create(const Config& config, bool doClean);
private:
    std::shared_ptr<table::FilesTable> _filesTable;
    std::shared_ptr<table::ClassfilesTable> _classfilesTable;
    std::atomic_bool _stopSaveThread;
    bool _hasSaveThread;
    std::jthread _saveThread;

    void createSaveThread();
};


}


#endif
