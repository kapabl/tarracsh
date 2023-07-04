#include "Fields.h"

#include "Classfiles.h"

using namespace kapa::tarracsh::domain::db::table;
using namespace kapa::infrastructure::db::table::column;
using kapa::infrastructure::db::table::AutoIncrementedRow;


Fields::Fields(infrastructure::db::Database &db, const std::string &tablename,
    std::shared_ptr<Classfiles> classfiles): Table(db, tablename, sizeof(FieldRow)), _classfiles(std::move(classfiles)) {
}

std::string Fields::getKey(const AutoIncrementedRow* row) {
    return getStrongMethodName(reinterpret_cast<const FieldRow&>(*row));
}

std::string Fields::getStrongMethodName(const FieldRow& row) const {
    auto strongClassname = _classfiles->getStrongClassname(row.ownerClass.id);
    auto result = fmt::format("{}#{}", strongClassname, _stringPool->getCString(row.name));
    return result;
}

void Fields::defineColumns() {
    DECLARE_COLUMN_PROP(FieldRow, id, StorageType::UInt64, DisplayAs::AsUInt64);
    DECLARE_FOREIGN_COLUMN_PROP(FieldRow, ownerClass, StorageType::Ref, DisplayAs::AsRef, _classfiles->getName().c_str(), "classname");
    DECLARE_COLUMN_PROP(FieldRow, name, StorageType::String, DisplayAs::AsString);
    DECLARE_COLUMN_PROP(FieldRow, descriptor, StorageType::String, DisplayAs::AsString);
    DECLARE_COLUMN_PROP(FieldRow, signature, StorageType::String, DisplayAs::AsString);
}
