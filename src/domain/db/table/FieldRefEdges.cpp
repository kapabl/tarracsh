#include "FieldRefEdges.h"

#include <utility>

using namespace kapa::tarracsh::domain::db::table;
using namespace kapa::infrastructure::db::table::column;
using kapa::infrastructure::db::table::AutoIncrementedRow;


FieldRefEdges::FieldRefEdges(
        infrastructure::db::Database &db,
        const std::string &tablename,
        std::shared_ptr<FieldRefs> fieldRefs,
        std::shared_ptr<Fields> fields,
        std::shared_ptr<ClassFiles> classFiles) :
        _fieldRefs(std::move(fieldRefs)),
        _fields(std::move(fields)),
        ClassOwnedTable(db, tablename,sizeof(FieldRefEdgeRow), std::move(classFiles)) {
}


std::string FieldRefEdges::getKey(const AutoIncrementedRow* row) {
    auto& edgeRow = *static_cast<const FieldRefEdgeRow*>(row);
    std::string result(fmt::format("{}-{}", edgeRow.from.id, edgeRow.to.id));
    return result;
}

void FieldRefEdges::defineColumns() {
    DECLARE_COLUMN_PROP(FieldRefEdgeRow, id, StorageType::UInt64, DisplayAs::AsUInt64);

    DECLARE_FOREIGN_COLUMN_PROP(FieldRefEdgeRow, from, StorageType::Ref, DisplayAs::AsRef,
                                _fieldRefs->getName().c_str(), "fieldRef");

    DECLARE_FOREIGN_COLUMN_PROP(FieldRefEdgeRow, to, StorageType::Ref, DisplayAs::AsRef,
                                _fieldRefs->getName().c_str(), "field");

    DECLARE_FOREIGN_COLUMN_PROP(FieldRefEdgeRow, ownerClass, StorageType::Ref, DisplayAs::AsRef,
                                _classfiles->getName().c_str(), "ownerClass");
}
