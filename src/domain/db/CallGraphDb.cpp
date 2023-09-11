#include "CallGraphDb.h"


using namespace std;
using namespace kapa::tarracsh::domain::db::callgraph;
using kapa::tarracsh::domain::db::table::ClassFileRow;


void CallGraphDb::init() {
    Database::init();
    _files = std::make_shared<table::Files>(*this, "files");
    _tablesByName[_files->getName()] = _files.get();
    _tablesReadOrder.push_back(_files.get());

    _classfiles = std::make_shared<table::ClassFiles>(*this, "classfiles", _files);
    _tablesByName[_classfiles->getName()] = _classfiles.get();
    _tablesReadOrder.push_back(_classfiles.get());

    _classRefs = std::make_shared<table::ClassRefs>(*this, "classRefs", _classfiles);
    _tablesByName[_classRefs->getName()] = _classRefs.get();
    _tablesReadOrder.push_back(_classRefs.get());

    _methods = std::make_shared<table::Methods>(*this, "methods", _classfiles);
    _tablesByName[_methods->getName()] = _methods.get();
    _tablesReadOrder.push_back(_methods.get());

    _methodRefs = std::make_shared<table::MethodRefs>(*this, "methodRefs", _classfiles, _methods);
    _tablesByName[_methodRefs->getName()] = _methodRefs.get();
    _tablesReadOrder.push_back(_methodRefs.get());

    _fields = std::make_shared<table::Fields>(*this, "fields", _classfiles);
    _tablesByName[_fields->getName()] = _fields.get();
    _tablesReadOrder.push_back(_fields.get());

    _fieldRefs = std::make_shared<table::FieldRefs>(*this, "fieldRefs", _classfiles, _fields);
    _tablesByName[_methods->getName()] = _methods.get();
    _tablesReadOrder.push_back(_fieldRefs.get());

    _files->init();

    _classfiles->init();
    _classRefs->init();

    _methods->init();
    _methodRefs->init();

    _fields->init();
    _fieldRefs->init();
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
