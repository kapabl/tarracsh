#ifndef TARRACSH_FIELD_REF_EDGES_TABLE_H
#define TARRACSH_FIELD_REF_EDGES_TABLE_H

#include <string>
#include <memory>

#include "ClassFiles.h"
#include "Fields.h"
#include "FieldRefs.h"
#include "ClassOwnedTable.h"
#include "infrastructure/db/table/Table.h"
#include "Files.h"

#pragma pack( push, 1 )
namespace kapa::tarracsh::domain::db::table {

struct FieldRefEdgeRow : infrastructure::db::table::AutoIncrementedRow {
    infrastructure::db::table::column::RefCol ownerClass{};
    infrastructure::db::table::column::RefCol from{};
    infrastructure::db::table::column::RefCol to{};
};

class FieldRefEdges : public ClassOwnedTable {

public:
    struct Types {
        typedef Fields TargetTable;
        typedef FieldRow TargetRow;

        typedef FieldRefs RefTable;
        typedef FieldRefRow RefRow;

        typedef FieldRefEdges EdgeTable;
        typedef FieldRefEdgeRow EdgeRow;
    };

    explicit FieldRefEdges(
            infrastructure::db::Database &db,
            const std::string &tablename,
            std::shared_ptr<FieldRefs> methodRefs,
            std::shared_ptr<Fields> fields,
            std::shared_ptr<ClassFiles> classFiles );

    [[nodiscard]] std::string getKey(const infrastructure::db::table::AutoIncrementedRow *row) override;

    void defineColumns() override;

private:
    std::shared_ptr<FieldRefs> _fieldRefs;
    std::shared_ptr<Fields> _fields;
};
}
#pragma pack(pop)
#endif
