#include "Database.h"
#include "table/Table.h"

#include <string>
#include <filesystem>
#include <memory>
#include <ranges>

#include "StringPool.h"

using namespace kapa::infrastructure::db;
using namespace tables;


Database::Database(const std::string &dataDir, log::Log &log)
    : _dataDir(dataDir), _log(log) {

}

void Database::stop() {
    write();
}

void Database::list(const std::string &tablename, bool displayRaw ) {
    if ( !_tables.contains(tablename)) {
        _log.writeln(format("Invalid table name: {}", tablename));
    }
    _tables[tablename]->list(displayRaw);
}

Table& Database::getTable(const std::string &tablename) {
    auto& result = *_tables[tablename];
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
    if (!_stringPool->read()) return false;

    for(const auto &table : _tables | std::views::values) {
        if (!table->read()) return false;
    }
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
    result = (std::filesystem::path(_dataDir) / result).string();

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

std::string Database::generateStringPoolFilename(const std::string &name) const {
    std::string result(name + StringPoolExtension);
    result = (std::filesystem::path(_dataDir) / result).string();

    return result;
}
