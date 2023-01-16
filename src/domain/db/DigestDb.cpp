#include "DigestDb.h"

#include <memory>

#include "table/ClassfilesTable.h"
#include "table/FilesTable.h"
#include "../infrastructure/db/StringPool.h"


using namespace std;
using namespace kapa::tarracsh::domain::db::digest;

void DigestDb::init() {
    Database::init();
    _filesTable = std::make_shared<FilesTable>(*this, "files");
    _classfilesTable = std::make_shared<ClassfilesTable>(*this, "classfiles", _filesTable);
    _filesTable->init();
    _classfilesTable->init();
}

void DigestDb::clean() {
    Database::clean();
    _filesTable->clean();
    _classfilesTable->clean();
}

bool DigestDb::read() {
    const auto result = Database::read() &&
                        _filesTable->read() &&
                        _classfilesTable->read();
    return result;
}

bool DigestDb::write() {
    const auto result = Database::write() &&
                        _filesTable->write() &&
                        _classfilesTable->write();
    return result;
}

void DigestDb::printSchema() {
    //TODO
    _filesTable->printSchema();
    _classfilesTable->printSchema();
}

void DigestDb::outputStats() const {
    cout << std::format("Number of files - jars and classfiles: {}", _filesTable->size()) << endl;
    cout << std::format("Number of classes: {}", _classfilesTable->size()) << endl;
}
