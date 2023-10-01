#include "CallGraphDb.h"


using namespace std;
using namespace kapa::tarracsh::domain::db::callgraph;
using kapa::tarracsh::domain::db::table::ClassFileRow;


void CallGraphDb::init() {
    Database::init();
    _files = std::make_shared<table::Files>(*this, "files");
    addTable(*_files);

    _classfiles = std::make_shared<table::ClassFiles>(*this, "classfiles", _files);
    addTable(*_classfiles);

    _classRefs = std::make_shared<table::ClassRefs>(*this, "classRefs", _classfiles);
    addTable(*_classRefs);

    _methods = std::make_shared<table::Methods>(*this, "methods", _classfiles);
    addTable(*_methods);

    _methodRefs = std::make_shared<table::MethodRefs>(*this, "methodRefs", _classfiles, _methods);
    addTable(*_methodRefs);

    _fields = std::make_shared<table::Fields>(*this, "fields", _classfiles);
    addTable(*_fields);

    _fieldRefs = std::make_shared<table::FieldRefs>(*this, "fieldRefs", _classfiles, _fields);
    addTable(*_fieldRefs);

    _classRefEdges = std::make_shared<table::ClassRefEdges>(*this, "classRefEdges", _classRefs, _classfiles);
    addTable(*_classRefEdges);

    _methodRefEdges = std::make_shared<table::MethodRefEdges>(*this, "methodRefEdges", _methodRefs, _methods, _classfiles);
    addTable(*_classRefEdges);

    _fieldRefEdges = std::make_shared<table::FieldRefEdges>(*this, "fieldRefEdges", _fieldRefs, _fields, _classfiles);
    addTable(*_classRefEdges);

    _files->init();

    _classfiles->init();
    _classRefs->init();
    _classRefEdges->init();

    _methods->init();
    _methodRefs->init();
    _methodRefEdges->init();

    _fields->init();
    _fieldRefs->init();
    _fieldRefEdges->init();
}



auto CallGraphDb::deleteClass(ClassFileRow *row) -> uint64_t {
    auto result = getClassRefs()->deleteClass(row) +
                  getMethods()->deleteClass(row) +
                  getMethodRefs()->deleteClass(row) +
                  getFields()->deleteClass(row) +
                  getFieldRefs()->deleteClass(row);
    return result;
}

std::shared_ptr<CallGraphDb> CallGraphDb::create(
        const Config &config,
        const bool doClean,
        const bool hasSaveThread) {

    std::shared_ptr<CallGraphDb> result;

    const auto db = std::make_shared<CallGraphDb>(config, hasSaveThread);

    if (Database::init(*db, doClean)) {
        result = db;
    }

    return result;

}
