#include "Methods.h"

#include "Classfiles.h"

using namespace kapa::tarracsh::domain::db::table;
using namespace kapa::infrastructure::db::table::column;
using kapa::infrastructure::db::table::AutoIncrementedRow;


Methods::Methods(infrastructure::db::Database &db, const std::string &tablename,
    std::shared_ptr<Classfiles> classfiles):
    ClassOwnedTable(db, tablename, sizeof(MethodRow)), _classfiles(std::move(classfiles)) {
}

std::string Methods::getKey(const AutoIncrementedRow* row) {
    return getStrongMethodName(reinterpret_cast<const MethodRow&>(*row));
}

std::string Methods::getStrongMethodName(const MethodRow& row) const {
    auto strongClassname = _classfiles->getStrongClassname(row.ownerClass.id);
    auto result = fmt::format("{}#{}", strongClassname, _stringPool->getCString(row.name));
    return result;
}

void Methods::defineColumns() {
    DECLARE_COLUMN_PROP(MethodRow, id, StorageType::UInt64, DisplayAs::AsUInt64);
    DECLARE_FOREIGN_COLUMN_PROP(MethodRow, ownerClass, StorageType::Ref, DisplayAs::AsRef, _classfiles->getName().c_str(), "classname");
    DECLARE_COLUMN_PROP(MethodRow, name, StorageType::String, DisplayAs::AsString);
    DECLARE_COLUMN_PROP(MethodRow, descriptor, StorageType::String, DisplayAs::AsString);
    DECLARE_COLUMN_PROP(MethodRow, accessFlags, StorageType::UInt64, DisplayAs::AsUInt64);
    DECLARE_COLUMN_PROP(MethodRow, refCount, StorageType::UInt64, DisplayAs::AsUInt64);
}
