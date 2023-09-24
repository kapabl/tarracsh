#include "ClassRefEdges.h"

#include <utility>

using namespace kapa::tarracsh::domain::db::table;
using namespace kapa::infrastructure::db::table::column;
using kapa::infrastructure::db::table::AutoIncrementedRow;


ClassRefEdges::ClassRefEdges(
        infrastructure::db::Database &db,
        const std::string &tablename,
        std::shared_ptr<ClassRefs> classRefs,
        std::shared_ptr<ClassFiles> classfiles) :
        _classRefs(std::move(classRefs)),
        ClassOwnedTable(db, tablename,sizeof(ClassRefEdgeRow), std::move(classfiles)) {
}


std::string ClassRefEdges::getKey(const AutoIncrementedRow* row) {
    std::string result("TODO class-ref-id->target-class-id");
    return result;
}

void ClassRefEdges::defineColumns() {
    DECLARE_COLUMN_PROP(ClassRefEdgeRow, id, StorageType::UInt64, DisplayAs::AsUInt64);
    DECLARE_FOREIGN_COLUMN_PROP(ClassRefEdgeRow, from, StorageType::Ref, DisplayAs::AsRef,
                                _classRefs->getName().c_str(), "classRef");
    DECLARE_FOREIGN_COLUMN_PROP(ClassRefEdgeRow, to, StorageType::Ref, DisplayAs::AsRef,
                                _classfiles->getName().c_str(), "classname");

    DECLARE_FOREIGN_COLUMN_PROP(ClassRefEdgeRow, ownerClass, StorageType::Ref, DisplayAs::AsRef,
                                _classfiles->getName().c_str(), "ownerClass");
}
