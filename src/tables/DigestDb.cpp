#include "DigestDb.h"

#include <memory>

#include "ClassfilesTable.h"
#include "FilesTable.h"
#include "StringPool.h"


using namespace std;
using namespace org::kapa::tarracsh::db;
using namespace tables;


void DigestDb::init() {
    Database::init();
    _filesTable = std::make_shared<FilesTable>(*this, "files");
    _digestTable = std::make_shared<ClassfilesTable>(*this, "classfiles", _filesTable);
    _filesTable->init();
    _digestTable->init();
}

void DigestDb::clean() {
    Database::clean();
    _filesTable->clean();
    _digestTable->clean();
}

bool DigestDb::read() {
    const auto result = Database::read() &&
                        _filesTable->read() &&
                        _digestTable->read();
    return result;
}

bool DigestDb::write() {
    const auto result = Database::write() &&
                        _filesTable->write() &&
                        _digestTable->write();
    return result;
}

void DigestDb::printSchema() {
    //TODO
    _filesTable->printSchema();
    _digestTable->printSchema();
}
