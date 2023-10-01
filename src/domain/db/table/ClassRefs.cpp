#include "ClassRefs.h"

#include <utility>

#include "ClassFiles.h"

using namespace kapa::tarracsh::domain::db::table;
using namespace kapa::infrastructure::db::table::column;
using kapa::infrastructure::db::table::AutoIncrementedRow;


ClassRefs::ClassRefs(infrastructure::db::Database &db, const std::string &tablename,
    std::shared_ptr<ClassFiles> classfiles): ClassOwnedTable(db, tablename, sizeof(ClassRefRow), std::move(classfiles)) {
}

std::string ClassRefs::getKey(const AutoIncrementedRow* row) {
    return getStrongRefName(reinterpret_cast<const ClassRefRow &>(*row));
}

std::string ClassRefs::getStrongRefName(const ClassRefRow& row) const {
    auto strongClassname = _classfiles->getStrongClassname(row.ownerClass.id);
    auto result = fmt::format("{}#{}", strongClassname, _stringPool->getCString(row.name));
    return result;
}

void ClassRefs::defineColumns() {
    DECLARE_COLUMN_PROP(ClassRefRow, id, StorageType::UInt64, DisplayAs::AsUInt64);
    DECLARE_COLUMN_PROP(ClassRefRow, edgeCount, StorageType::UInt64, DisplayAs::AsUInt64);
    DECLARE_FOREIGN_COLUMN_PROP(ClassRefRow, ownerClass, StorageType::Ref, DisplayAs::AsRef, _classfiles->getName().c_str(), "classname");
   // DECLARE_FOREIGN_COLUMN_PROP(ClassRefRow, targetClass, StorageType::Ref, DisplayAs::AsRef, _classfiles->getName().c_str(), "classname");
    DECLARE_COLUMN_PROP(ClassRefRow, name, StorageType::String, DisplayAs::AsString);
}
