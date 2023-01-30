#include "DigestDb.h"
#include "../infrastructure/db/table/Table.inl"
#include <memory>
#include "../infrastructure/db/StringPool.h"


using namespace std;
using namespace kapa::tarracsh::domain::db::digest;

void DigestDb::init() {
    Database::init();
    _filesTable = std::make_shared<FilesTable>(*this, "files");
    _classfilesTable = std::make_shared<ClassfilesTable>(*this, "classfiles", _filesTable);
    _filesTable->init();
    _classfilesTable->init();
    if (_hasSaveThread) {
        createSaveThread();
    }
}

std::shared_ptr<DigestDb> DigestDb::create(
    const std::string &dataDir,
    infrastructure::log::Log &log,
    const bool doClean,
    const bool hasSaveThread) {

    std::shared_ptr<DigestDb> result;

    const auto db = std::make_shared<DigestDb>(dataDir, log, hasSaveThread);

    if (Database::init(*db, doClean)) {
        result = db;
    }

    return result;

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
    _filesTable->printSchema();
    _classfilesTable->printSchema();
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
