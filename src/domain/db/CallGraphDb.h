#ifndef TARRACSH_CALL_GRAPH_DB_H
#define TARRACSH_CALL_GRAPH_DB_H
#include <filesystem>
#include "infrastructure/db/Database.h"
#include "table/Files.h"
#include "table/ClassFiles.h"
#include "table/ClassRefs.h"
#include "table/Methods.h"
#include "table/MethodRefs.h"
#include "table/Fields.h"
#include "table/FieldRefs.h"


namespace kapa::tarracsh::domain::db::callgraph {

class CallGraphDb : public infrastructure::db::Database {
public:
    explicit CallGraphDb(const Config &config, const bool hasSaveThread)
        : Database(config, hasSaveThread) {

    }

    void init() override;
    static std::shared_ptr<CallGraphDb> create(const Config &config, bool doClean,
        bool hasSaveThread);

    auto getFiles() { return _files; }
    auto getClassfiles() { return _classfiles; }
    auto getClassRefs() { return _classRefs; }
    auto getMethods() { return _methods; }
    auto getMethodRefs() { return _methodRefs; }
    auto getFields() { return _fields; }
    auto getFieldRefs() { return _fieldRefs; }

    auto deleteClass(table::ClassFileRow* row) -> uint64_t;

private:
    std::shared_ptr<table::Files> _files;
    std::shared_ptr<table::ClassFiles> _classfiles;
    std::shared_ptr<table::ClassRefs> _classRefs;
    std::shared_ptr<table::Methods> _methods;
    std::shared_ptr<table::MethodRefs> _methodRefs;
    std::shared_ptr<table::Fields> _fields;
    std::shared_ptr<table::FieldRefs> _fieldRefs;

};


}


#endif
