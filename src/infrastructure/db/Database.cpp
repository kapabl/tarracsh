#include "Database.h"
#include "table/Table.h"

#include <string>
#include <filesystem>
#include <memory>
#include <ranges>

#include "StringPool.h"

using namespace kapa::infrastructure::db;
using namespace tables;


Database::Database(const Config& config)
    : _log(*config.log), _config(config) {
    _queryEngine = std::make_unique<query::Engine>(*this);
}

void Database::stop() {
    write();
}

Table *Database::getTable(const std::string &tablename) {
    const auto result = _tables.contains(tablename ) ? _tables[tablename] : nullptr;
    return result;
}

void Database::init() {
    _stringPool = std::make_shared<db::StringPool>(generateStringPoolFilename("sp"));
}

void Database::clean() {
    _stringPool->clean();
    for (const auto& table : _tables | std::views::values) {
        table->clean();
    }
}

void Database::backup() {
    _stringPool->backup();
}

bool Database::read() {
    if (_read) return false;
    if (!_stringPool->read()) return false;

    for(const auto &table : _tables | std::views::values) {
        if (!table->read()) return false;
    }
    _read = true;
    return true;
}

bool Database::write() {
    if (!_stringPool->write()) return false;
    for (const auto& table : _tables | std::views::values) {
        if (!table->write()) return false;
    }
    return true;

}

void Database::printSchema() {
    for (const auto& table : _tables | std::views::values) {
        table->printSchema();
    }
}

columns::StringCol Database::getPoolString(const std::string &value) const {
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

bool Database::executeQuery(const std::string &query, const bool displayRaw) {
    const auto result = _queryEngine->execute2(query, displayRaw);
    return result;
}

std::string Database::generateStringPoolFilename(const std::string &name) const {
    std::string result(name + StringPoolExtension);
    result = (std::filesystem::path(_config.dataDir) / result).string();

    return result;
}
