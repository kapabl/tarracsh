#include "CallGraphDb.h"


using namespace std;
using namespace kapa::tarracsh::domain::db::callgraph;


void CallGraphDb::init() {
    Database::init();
    _files = std::make_shared<table::Files>(*this, "files");
    _tables[_files->getName()] = _files.get();

    _classfiles = std::make_shared<table::Classfiles>(*this, "classfiles", _files);
    _tables[_classfiles->getName()] = _classfiles.get();

    _methods = std::make_shared<table::Methods>(*this, "methods", _classfiles);
    _tables[_methods->getName()] = _methods.get();

    _files->init();
    _classfiles->init();
    _methods->init();
}

std::shared_ptr<CallGraphDb> CallGraphDb::create(
    const Config& config,
    const bool doClean,
    const bool hasSaveThread) {

    std::shared_ptr<CallGraphDb> result;

    const auto db = std::make_shared<CallGraphDb>(config, hasSaveThread);

    if (Database::init(*db, doClean)) {
        result = db;
    }

    return result;

}
