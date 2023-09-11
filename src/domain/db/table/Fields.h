#ifndef TARRACSH_FIELDS_TABLE_H
#define TARRACSH_FIELDS_TABLE_H
#include <string>

#include "ClassFiles.h"
#include "ClassOwnedTable.h"
#include "infrastructure/db/table/Table.h"
#include "Files.h"

#pragma pack( push, 1 )
namespace kapa::tarracsh::domain::db::table {


struct FieldRow : infrastructure::db::table::AutoIncrementedRow {
    infrastructure::db::table::column::RefCol ownerClass{};
    infrastructure::db::table::column::StringCol name{};
    infrastructure::db::table::column::StringCol descriptor{};
    infrastructure::db::table::column::StringCol signature{};
    infrastructure::db::table::column::UInt64Col refCount{};

    FieldRow() = default;
    explicit FieldRow(const ClassFileRow& classFileRow) {
        setClass(classFileRow);
    }

    void setClass(const ClassFileRow &classFileRow) {
        ownerClass.id = classFileRow.id;
    }

};

class Fields : public  ClassOwnedTable {

public:
    explicit Fields(infrastructure::db::Database &db,
                             const std::string &tablename,
                             std::shared_ptr<ClassFiles> classfiles);

    [[nodiscard]] std::string getKey(const infrastructure::db::table::AutoIncrementedRow* row) override;
    [[nodiscard]] std::string getStrongMemberName(const FieldRow &row) const;

    void defineColumns() override;

};

}

#pragma pack(pop)
#endif
