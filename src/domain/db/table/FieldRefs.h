#ifndef TARRACSH_FIELD_REFS_TABLE_H
#define TARRACSH_FIELD_REFS_TABLE_H
#include <string>

#include "Classfiles.h"
#include "ClassOwnedTable.h"
#include "Fields.h"
#include "infrastructure/db/table/Table.h"

#pragma pack( push, 1 )
namespace kapa::tarracsh::domain::db::table {


struct FieldRefRow : infrastructure::db::table::AutoIncrementedRow {
    infrastructure::db::table::column::RefCol ownerClass{};
    infrastructure::db::table::column::StringCol name{};
    infrastructure::db::table::column::StringCol descriptor{};

    infrastructure::db::table::column::RefCol targetClass{};
    infrastructure::db::table::column::RefCol targetField{};

    FieldRefRow() = default;
    explicit FieldRefRow(const ClassfileRow& classFileRow) {
        setClass(classFileRow);
    }

    void setClass(const ClassfileRow &classFileRow) {
        ownerClass.id = classFileRow.id;
    }

    void setTargetClass(const ClassfileRow &classFileRow) {
        targetClass.id = classFileRow.id;
    }

    void setTargetField(const FieldRow &fieldRow) {
        targetField.id = fieldRow.id;
    }

};

class FieldRefs : public ClassOwnedTable {

public:
    explicit FieldRefs(infrastructure::db::Database &db,
                             const std::string &tablename,
                             std::shared_ptr<Classfiles> classfile,
                             std::shared_ptr<Fields> fields);

    [[nodiscard]] std::string getKey(const infrastructure::db::table::AutoIncrementedRow* row) override;
    [[nodiscard]] std::string getStrongMethodName(const FieldRefRow &row) const;

    void defineColumns() override;

private:
    const std::shared_ptr<Fields> _fields;
};

}

#pragma pack(pop)
#endif
