#include "FieldRefs.h"

#include "ClassFiles.h"

using namespace kapa::tarracsh::domain::db::table;
using namespace kapa::infrastructure::db::table::column;
using kapa::infrastructure::db::table::AutoIncrementedRow;


FieldRefs::FieldRefs(
        infrastructure::db::Database &db,
        const std::string &tablename,
        std::shared_ptr<ClassFiles> classfiles,
        std::shared_ptr<Fields> fields) :
        ClassOwnedTable(db, tablename, sizeof(FieldRefRow), std::move(classfiles)),
        _fields(std::move(fields)) {
}

std::string FieldRefs::getKey(const AutoIncrementedRow *row) {
    return getStrongRefName(reinterpret_cast<const FieldRefRow &>(*row));
}

std::string FieldRefs::getStrongRefName(const FieldRefRow &row) const {
    auto strongClassname = _classfiles->getStrongClassname(row.ownerClass.id);
    auto result = fmt::format("{}#{}", strongClassname, _stringPool->getCString(row.name));
    return result;
}

void FieldRefs::defineColumns() {
    DECLARE_COLUMN_PROP(FieldRefRow, id, StorageType::UInt64, DisplayAs::AsUInt64);
    DECLARE_FOREIGN_COLUMN_PROP(FieldRefRow, ownerClass, StorageType::Ref, DisplayAs::AsRef,
                                _classfiles->getName().c_str(), "classname");
    DECLARE_COLUMN_PROP(FieldRefRow, name, StorageType::String, DisplayAs::AsString);
    DECLARE_COLUMN_PROP(FieldRefRow, descriptor, StorageType::String, DisplayAs::AsString);

    DECLARE_FOREIGN_COLUMN_PROP(FieldRefRow, targetClass, StorageType::Ref, DisplayAs::AsRef,
                                _classfiles->getName().c_str(), "classname");
    DECLARE_FOREIGN_COLUMN_PROP(FieldRefRow, targetField, StorageType::Ref, DisplayAs::AsRef,
                                _fields->getName().c_str(), "name");

}
