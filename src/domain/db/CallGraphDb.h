#ifndef TARRACSH_CALL_GRAPH_DB_H
#define TARRACSH_CALL_GRAPH_DB_H
#include <filesystem>
#include <unordered_map>
#include <unordered_set>
#include "infrastructure/db/Database.h"
#include "table/Files.h"

#include "table/ClassFiles.h"
#include "table/ClassRefs.h"
#include "table/ClassRefEdges.h"

#include "table/Methods.h"
#include "table/MethodRefs.h"
#include "table/MethodRefEdges.h"

#include "table/Fields.h"
#include "table/FieldRefs.h"
#include "table/FieldRefEdges.h"


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

    auto getClassFiles() { return _classfiles; }

    auto getClassRefs() { return _classRefs; }

    auto getMethods() { return _methods; }

    auto getMethodRefs() { return _methodRefs; }

    auto getFields() { return _fields; }

    auto getFieldRefs() { return _fieldRefs; }

    auto getClassRefEdges() { return _classRefEdges; }
    auto getMethodRefEdges() { return _methodRefEdges; }
    auto getFieldRefEdges() { return _fieldRefEdges; }

    auto deleteClass(table::ClassFileRow *row) -> uint64_t;

private:
    std::shared_ptr<table::Files> _files;
    std::shared_ptr<table::ClassFiles> _classfiles;
    std::shared_ptr<table::ClassRefs> _classRefs;
    std::shared_ptr<table::Methods> _methods;
    std::shared_ptr<table::MethodRefs> _methodRefs;
    std::shared_ptr<table::Fields> _fields;
    std::shared_ptr<table::FieldRefs> _fieldRefs;

    std::shared_ptr<table::ClassRefEdges> _classRefEdges;
    std::shared_ptr<table::MethodRefEdges> _methodRefEdges;
    std::shared_ptr<table::FieldRefEdges> _fieldRefEdges;

};
}


#endif
