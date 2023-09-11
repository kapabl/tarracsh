#include "Database.h"
#include "table/Table.h"

#include <string>
#include <filesystem>
#include <chrono>
#include <memory>
#include <ranges>
#include <memory>
#include <condition_variable>
#include "StringPool.h"

using namespace kapa::infrastructure::db;
using namespace table;
using namespace std::chrono_literals;


kapa::infrastructure::profiler::MillisecondDuration Database::getReadTime() const {
    return _readTime;
}


Database::Database(const Config &config, const bool hasSaveThread)
    : _log(*config.log), _config(config), _hasSaveThread(hasSaveThread) {
    _queryEngine = std::make_unique<query::Engine>(*this);

    if (_hasSaveThread) {
        createSaveThread();
    }
}

void Database::createSaveThread() {

    _saveThread = std::jthread([this](const std::stop_token &stopToken) -> void {
        std::mutex mutex;
        std::unique_lock lock(mutex);
        while (true) {
            std::condition_variable_any().wait_for(lock, stopToken, 10s, [&stopToken] {
                return stopToken.stop_requested();
            });

            if (stopToken.stop_requested()) {
                break;
            }
            write();
        }

    });
}


void Database::outputStats() const {
    for (const auto& table : _tablesByName | std::views::values) {
        std::cout << fmt::format("table {}, rows: {}", table->getName(), table->size()) << std::endl;
    }
}


void Database::stop() {
    if (_saveThread.get_id() != std::jthread::id()) {
        _saveThread.request_stop();
    }
    write();
}

Table *Database::getTable(const std::string &tablename) {
    const auto result = _tablesByName.contains(tablename) ? _tablesByName[tablename] : nullptr;
    return result;
}

void Database::init() {
    _stringPool = std::make_shared<db::StringPool>(generateStringPoolFilename("sp"));
}

void Database::clean() {
    _stringPool->clean();
    for (const auto &table : _tablesByName | std::views::values) {
        table->clean();
    }
}

void Database::backup() {
    _stringPool->backup();
    for (const auto &table : _tablesByName | std::views::values) {
        table->backup();
    }
}

bool Database::read() {
    if (_read) return false;
    profiler::ScopedTimer timer(&_readTime);
    if (!_stringPool->read()) return false;

//    for (const auto &table : _tablesByName | std::views::values) {
//        if (!table->read()) return false;
//    }
    for (const auto &table : _tablesReadOrder) {
        if (!table->read()) return false;
    }
    _read = true;
    return true;
}

bool Database::write() {
    if (!_stringPool->write()) return false;
    for (const auto &table : _tablesByName | std::views::values) {
        if (!table->write()) return false;
    }
    return true;

}

void Database::printSchema() {
    for (const auto &table : _tablesByName | std::views::values) {
        table->printSchema();
    }
}

column::StringCol Database::getPoolString(const std::string &value) const {
    const auto result = _stringPool->add(value);
    return result;
}

std::string Database::generateTableFilename(const std::string &name) const {
    std::string result(name + TableExtension);
    result = (std::filesystem::path(_config.dataDir) / result).string();

    return result;
}

bool Database::init(Database &db, const bool doClean) {
    auto result = true;
    db.init();

    if (doClean) {
        db.backup();
        db.clean();
    } else {
        result = db.read();
    }

    return result;
}

bool Database::executeQuery(const std::string &query, const bool displayRaw) const {
    const auto result = _queryEngine->execute(query, displayRaw);
    return result;
}

std::string Database::generateStringPoolFilename(const std::string &name) const {
    std::string result(name + StringPoolExtension);
    result = (std::filesystem::path(_config.dataDir) / result).string();

    return result;
}
