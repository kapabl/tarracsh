#include "infrastructure/db/Database.h"
#include "infrastructure/db/table/Table.h"

#include <fmt/format.h>
#include <iostream>
#include <ranges>

#include "infrastructure/profiling/ScopedTimer.h"

using kapa::infrastructure::profiler::ScopedTimer;

namespace kapa::infrastructure::db {

void Database::addTable(table::Table &table) {
    _tablesByName[table.getName()] = &table;
    _tablesReadOrder.push_back(&table);
}

table::Table *Database::getTable(const std::string &tablename) {
    const auto result = _tablesByName.contains(tablename) ? _tablesByName[tablename] : nullptr;
    return result;
}

void Database::outputStats() const {
    for (const auto &table : _tablesByName | std::views::values) {
        std::cout << fmt::format("table {}, rows: {}", table->getName(), table->size()) << std::endl;
    }
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
    ScopedTimer timer(&_readTime);
    if (!_stringPool->read()) return false;
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

}
