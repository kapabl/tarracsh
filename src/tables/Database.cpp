#include "Database.h"
#include "Table.h"

#include <memory>
#include "StringPool.h"


using namespace std;
using namespace org::kapa::tarracsh::db;
using namespace tables;


Database::Database(const std::string& dataDir): _dataDir(dataDir) {

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

string Database::generateTableFilename(const string &name) const {
    string result(name + TableExtension);
    result = (filesystem::path(_dataDir) / result).string();

    return result;
}

string Database::generateStringPoolFilename(const string &name) const {
    string result(name + StringPoolExtension);
    result = (std::filesystem::path(_dataDir) / result).string();

    return result;
}
