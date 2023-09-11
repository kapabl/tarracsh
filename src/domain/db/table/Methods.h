#ifndef TARRACSH_METHODS_TABLE_H
#define TARRACSH_METHODS_TABLE_H
#include <string>
#include <map>
#include <set>

#include "ClassFiles.h"
#include "ClassOwnedTable.h"
#include "infrastructure/db/table/Table.h"
#include "Files.h"

#pragma pack( push, 1 )
namespace kapa::tarracsh::domain::db::table {


struct MethodRow : infrastructure::db::table::AutoIncrementedRow {
    infrastructure::db::table::column::RefCol ownerClass{};
    infrastructure::db::table::column::UInt64Col accessFlags{};
    infrastructure::db::table::column::StringCol name{};
    infrastructure::db::table::column::StringCol descriptor{};
    infrastructure::db::table::column::UInt64Col refCount{};

    MethodRow() = default;
    explicit MethodRow(const ClassFileRow& classFileRow) {
        setClass(classFileRow);
    }

    void setClass(const ClassFileRow &classFileRow) {
        ownerClass.id = classFileRow.id;
    }

};

class Methods : public ClassOwnedTable {

public:
    explicit Methods(infrastructure::db::Database &db,
                             const std::string &tablename,
                             std::shared_ptr<ClassFiles> classfiles);

    [[nodiscard]] std::string getKey(const infrastructure::db::table::AutoIncrementedRow* row) override;
    [[nodiscard]] std::string getStrongMemberName(const MethodRow &row) const;

    void defineColumns() override;

};

}

#pragma pack(pop)
#endif
