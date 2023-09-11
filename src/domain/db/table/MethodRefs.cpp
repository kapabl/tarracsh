#include "MethodRefs.h"


using namespace kapa::tarracsh::domain::db::table;
using namespace kapa::infrastructure::db::table::column;
using kapa::infrastructure::db::table::AutoIncrementedRow;


MethodRefs::MethodRefs(
        infrastructure::db::Database &db,
        const std::string &tablename,
        std::shared_ptr<ClassFiles> classfiles,
        std::shared_ptr<Methods> methods) :
        ClassOwnedTable(db, tablename, sizeof(MethodRefRow), std::move(classfiles)),
        _methods(std::move(methods)) {
}

std::string MethodRefs::getKey(const AutoIncrementedRow *row) {
    return getStrongRefName(reinterpret_cast<const MethodRefRow &>(*row));
}

std::string MethodRefs::getStrongRefName(const MethodRefRow &row) const {
    auto strongClassname = _classfiles->getStrongClassname(row.ownerClass.id);
    auto result = fmt::format("{}#{}", strongClassname, _stringPool->getCString(row.name));
    return result;
}

void MethodRefs::defineColumns() {
    DECLARE_COLUMN_PROP(MethodRefRow, id, StorageType::UInt64, DisplayAs::AsUInt64);
    DECLARE_FOREIGN_COLUMN_PROP(MethodRefRow, ownerClass, StorageType::Ref, DisplayAs::AsRef,
                                _classfiles->getName().c_str(), "classname");
    DECLARE_COLUMN_PROP(MethodRefRow, name, StorageType::String, DisplayAs::AsString);
    DECLARE_COLUMN_PROP(MethodRefRow, descriptor, StorageType::String, DisplayAs::AsString);

    DECLARE_FOREIGN_COLUMN_PROP(MethodRefRow, targetClass, StorageType::Ref, DisplayAs::AsRef,
                                _classfiles->getName().c_str(), "classname");
    DECLARE_FOREIGN_COLUMN_PROP(MethodRefRow, targetMethod, StorageType::Ref, DisplayAs::AsRef,
                                _methods->getName().c_str(), "name");
}
