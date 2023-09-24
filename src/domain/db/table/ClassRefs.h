#ifndef TARRACSH_CLASS_REFS_TABLE_H
#define TARRACSH_CLASS_REFS_TABLE_H
#include <string>

#include "ClassFiles.h"
#include "ClassOwnedTable.h"
#include "infrastructure/db/table/Table.h"
#include "Files.h"

#pragma pack( push, 1 )
namespace kapa::tarracsh::domain::db::table {


struct ClassRefRow : infrastructure::db::table::AutoIncrementedRow {
    infrastructure::db::table::column::RefCol ownerClass{};
    infrastructure::db::table::column::StringCol name{};
    infrastructure::db::table::column::UInt64Col refCount{};

    ClassRefRow() = default;
    explicit ClassRefRow(const ClassFileRow& classFileRow) {
        setClass(classFileRow);
    }

    void setClass(const ClassFileRow &classFileRow) {
        ownerClass.id = classFileRow.id;
    }

};

class ClassRefs : public ClassOwnedTable {

public:
    explicit ClassRefs(infrastructure::db::Database &db,
                             const std::string &tablename,
                             std::shared_ptr<ClassFiles> classfiles);

    [[nodiscard]] std::string getKey(const infrastructure::db::table::AutoIncrementedRow* row) override;
    [[nodiscard]] std::string getStrongRefName(const ClassRefRow &row) const;

    void defineColumns() override;

};

}

#pragma pack(pop)
#endif
