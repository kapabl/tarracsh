#include "Database.h"
#include "table/Table.h"

#include <string>
#include <filesystem>
#include <memory>
#include "StringPool.h"

using namespace kapa::infrastructure::db;
using namespace tables;


Database::Database(const std::string &dataDir, log::Log &log)
    : _dataDir(dataDir), _log(log) {

}

void Database::init() {
    _stringPool = std::make_shared<db::StringPool>(generateStringPoolFilename("sp"));
}

void Database::clean() {
    _stringPool->clean();
}

bool Database::read() {
    const auto result = _stringPool->read();
    return result;
}

bool Database::write() {
    const auto result = _stringPool->write();
    return result;
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

std::string Database::generateStringPoolFilename(const std::string &name) const {
    std::string result(name + StringPoolExtension);
    result = (std::filesystem::path(_dataDir) / result).string();

    return result;
}
