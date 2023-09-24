#ifndef TARRACSH_METHOD_REF_EDGES_TABLE_H
#define TARRACSH_METHOD_REF_EDGES_TABLE_H

#include <string>
#include <memory>

#include "ClassFiles.h"
#include "Methods.h"
#include "MethodRefs.h"
#include "ClassOwnedTable.h"
#include "infrastructure/db/table/Table.h"
#include "Files.h"

#pragma pack( push, 1 )
namespace kapa::tarracsh::domain::db::table {

struct MethodRefEdgeRow : infrastructure::db::table::AutoIncrementedRow {
    infrastructure::db::table::column::RefCol ownerClass{};
    infrastructure::db::table::column::RefCol from{};
    infrastructure::db::table::column::RefCol to{};
};

class MethodRefEdges : public ClassOwnedTable {

public:
    explicit MethodRefEdges(
            infrastructure::db::Database &db,
            const std::string &tablename,
            std::shared_ptr<MethodRefs> methodRefs,
            std::shared_ptr<Methods> methods,
            std::shared_ptr<ClassFiles> classFiles );

    [[nodiscard]] std::string getKey(const infrastructure::db::table::AutoIncrementedRow *row) override;

    void defineColumns() override;

private:
    std::shared_ptr<MethodRefs> _methodRefs;
    std::shared_ptr<Methods> _methods;
};
}
#pragma pack(pop)
#endif
