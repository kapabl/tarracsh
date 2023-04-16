#ifndef TARRACSH_CALL_GRAPH_DB_H
#define TARRACSH_CALL_GRAPH_DB_H
#include <filesystem>
#include "infrastructure/db/Database.h"
#include "table/Files.h"
#include "table/Classfiles.h"
#include "table/Methods.h"
#include "table/Fields.h"


namespace kapa::tarracsh::domain::db::callgraph {

class CallGraphDb : public infrastructure::db::Database {
public:
    explicit CallGraphDb(const Config &config, const bool hasSaveThread)
        : Database(config, hasSaveThread) {

    }

    void init() override;
    static std::shared_ptr<CallGraphDb> create(const Config &config, bool doClean,
        const bool hasSaveThread);

    auto getFiles() { return _files; }
    auto getClassfiles() { return _classfiles; }
    auto getMethods() { return _methods; }
    auto getFields() { return _fields; }

private:
    std::shared_ptr<table::Files> _files;
    std::shared_ptr<table::Classfiles> _classfiles;
    std::shared_ptr<table::Methods> _methods;
    std::shared_ptr<table::Fields> _fields;

};


}


#endif
