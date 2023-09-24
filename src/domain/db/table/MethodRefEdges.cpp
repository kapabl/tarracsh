#include "MethodRefEdges.h"

#include <utility>

using namespace kapa::tarracsh::domain::db::table;
using namespace kapa::infrastructure::db::table::column;
using kapa::infrastructure::db::table::AutoIncrementedRow;


MethodRefEdges::MethodRefEdges(
        infrastructure::db::Database &db,
        const std::string &tablename,
        std::shared_ptr<MethodRefs> methodRefs,
        std::shared_ptr<Methods> methods,
        std::shared_ptr<ClassFiles> classFiles) :
        _methodRefs(std::move(methodRefs)),
        _methods(std::move(methods)),
        ClassOwnedTable(db, tablename,sizeof(MethodRefEdgeRow), std::move(classFiles)) {
}


std::string MethodRefEdges::getKey(const AutoIncrementedRow* row) {
    std::string result("TODO class-ref-id->target-class-id");
    return result;
}

void MethodRefEdges::defineColumns() {
    DECLARE_COLUMN_PROP(MethodRefEdgeRow, id, StorageType::UInt64, DisplayAs::AsUInt64);

    DECLARE_FOREIGN_COLUMN_PROP(MethodRefEdgeRow, from, StorageType::Ref, DisplayAs::AsRef,
                                _methodRefs->getName().c_str(), "methodRef");

    DECLARE_FOREIGN_COLUMN_PROP(MethodRefEdgeRow, to, StorageType::Ref, DisplayAs::AsRef,
                                _methodRefs->getName().c_str(), "method");

    DECLARE_FOREIGN_COLUMN_PROP(MethodRefEdgeRow, ownerClass, StorageType::Ref, DisplayAs::AsRef,
                                _classfiles->getName().c_str(), "ownerClass");
}
