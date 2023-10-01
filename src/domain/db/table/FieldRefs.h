#ifndef TARRACSH_FIELD_REFS_TABLE_H
#define TARRACSH_FIELD_REFS_TABLE_H
#include <string>

#include "ClassFiles.h"
#include "ClassOwnedTable.h"
#include "Fields.h"
#include "infrastructure/db/table/Table.h"

#pragma pack( push, 1 )
namespace kapa::tarracsh::domain::db::table {


struct FieldRefRow : infrastructure::db::table::AutoIncrementedRow {
    infrastructure::db::table::column::RefCol ownerClass{};
    infrastructure::db::table::column::StringCol classname{};
    infrastructure::db::table::column::StringCol name{};
    infrastructure::db::table::column::StringCol descriptor{};
    infrastructure::db::table::column::UInt64Col edgeCount{};

    FieldRefRow() = default;
    explicit FieldRefRow(const ClassFileRow& classFileRow) {
        setClass(classFileRow);
    }

    void setClass(const ClassFileRow &classFileRow) {
        ownerClass.id = classFileRow.id;
    }


};

class FieldRefs : public ClassOwnedTable {

public:

    explicit FieldRefs(infrastructure::db::Database &db,
                             const std::string &tablename,
                             std::shared_ptr<ClassFiles> classfile,
                             std::shared_ptr<Fields> fields);

    [[nodiscard]] std::string getKey(const infrastructure::db::table::AutoIncrementedRow* row) override;
    [[nodiscard]] std::string getStrongRefName(const FieldRefRow &row) const;

    void defineColumns() override;

private:
    const std::shared_ptr<Fields> _fields;
};

}

#pragma pack(pop)
#endif
