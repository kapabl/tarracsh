#include "DigestDb.h"
#include <memory>
#include "infrastructure/db/StringPool.h"


using namespace std;
using namespace kapa::tarracsh::domain::db::digest;

void DigestDb::init() {
    Database::init();
    _files = std::make_shared<table::Files>(*this, "files");
    _tables[_files->getName()] = _files.get();

    _classfiles = std::make_shared<table::Classfiles>(*this, "classfiles", _files);
    _tables[_classfiles->getName()] = _classfiles.get();

    _files->init();
    _classfiles->init();
   
}

std::shared_ptr<DigestDb> DigestDb::create(
    const Config& config,
    const bool doClean,
    const bool hasSaveThread) {

    std::shared_ptr<DigestDb> result;

    const auto db = std::make_shared<DigestDb>(config, hasSaveThread);

    if (Database::init(*db, doClean)) {
        result = db;
    }

    return result;

}

