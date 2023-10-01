#ifndef TARRACSH_CLASS_REF_EDGES_TABLE_H
#define TARRACSH_CLASS_REF_EDGES_TABLE_H

#include <string>
#include <memory>

#include "ClassFiles.h"
#include "ClassRefs.h"
#include "ClassOwnedTable.h"
#include "infrastructure/db/table/Table.h"
#include "Files.h"

#pragma pack( push, 1 )
namespace kapa::tarracsh::domain::db::table {

struct ClassRefEdgeRow : infrastructure::db::table::AutoIncrementedRow {
    infrastructure::db::table::column::RefCol ownerClass{};
    infrastructure::db::table::column::RefCol from{};
    infrastructure::db::table::column::RefCol to{};
};

class ClassRefEdges : public ClassOwnedTable {

public:
    typedef ClassRefs TRefTable;
    explicit ClassRefEdges(
            infrastructure::db::Database &db,
            const std::string &tablename,
            std::shared_ptr<ClassRefs> classRefs,
            std::shared_ptr<ClassFiles> classfiles);

    [[nodiscard]] std::string getKey(const infrastructure::db::table::AutoIncrementedRow *row) override;

    void defineColumns() override;

private:
    std::shared_ptr<ClassRefs> _classRefs;
};
}
#pragma pack(pop)
#endif
