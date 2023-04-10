#include "DigestDb.h"
#include <memory>
#include "infrastructure/db/StringPool.h"


using namespace std;
using namespace kapa::tarracsh::domain::db::digest;

void DigestDb::init() {
    Database::init();
    _filesTable = std::make_shared<table::FilesTable>(*this, "files");
    _tables[_filesTable->getName()] = _filesTable.get();

    _classfilesTable = std::make_shared<table::ClassfilesTable>(*this, "classfiles", _filesTable);
    _tables[_classfilesTable->getName()] = _classfilesTable.get();

    _filesTable->init();
    _classfilesTable->init();
   
    if (_hasSaveThread) {
        createSaveThread();
    }
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

void DigestDb::outputStats() const {
    cout << std::format("Number of files - jars and classfiles: {}", _filesTable->size()) << endl;
    cout << std::format("Number of classes: {}", _classfilesTable->size()) << endl;
}

void DigestDb::createSaveThread() {

    _saveThread = std::jthread([this](std::stop_token stopToken) -> void {
        std::mutex mutex;
        std::unique_lock lock(mutex);
        while (true) {
            condition_variable_any().wait_for(lock, stopToken, 10s, [&stopToken] {
                return stopToken.stop_requested();
            });

            if (stopToken.stop_requested()) {
                break;
            }
            write();
        }

    });
}

void DigestDb::stop() {
    if (_saveThread.get_id() != std::jthread::id()) {
        _saveThread.request_stop();
    }
    write();
}

void DigestDb::backup() {
    Database::backup();
    _filesTable->backup();
    _classfilesTable->backup();
}
