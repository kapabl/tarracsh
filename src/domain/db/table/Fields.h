#ifndef TARRACSH_FIELDS_TABLE_H
#define TARRACSH_FIELDS_TABLE_H
#include <string>

#include "Classfiles.h"
#include "infrastructure/db/table/Table.h"
#include "Files.h"

#pragma pack( push, 1 )
namespace kapa::tarracsh::domain::db::table {


struct FieldRow : infrastructure::db::table::AutoIncrementedRow {
    infrastructure::db::table::column::RefCol ownerClass{};
    infrastructure::db::table::column::StringCol name{};
    infrastructure::db::table::column::StringCol descriptor{};
    infrastructure::db::table::column::StringCol signature{};

    FieldRow() = default;
    explicit FieldRow(const ClassfileRow& classFileRow) {
        setClass(classFileRow);
    }

    void setClass(const ClassfileRow &classFileRow) {
        ownerClass.id = classFileRow.id;
    }

};

class Fields : public  infrastructure::db::table::Table {

public:
    explicit Fields(infrastructure::db::Database &db,
                             const std::string &tablename,
                             std::shared_ptr<Classfiles> classfiles);

    [[nodiscard]] std::string getKey(const infrastructure::db::table::AutoIncrementedRow* row) override;
    [[nodiscard]] std::string getStrongMethodName(const FieldRow &row) const;

    void defineColumns() override;

private:
    const std::shared_ptr<Classfiles> _classfiles;
    //std::unordered_map<StringCol, std::set<const FieldRow *>> _jarIndex;
    // std::unordered_map<StringCol, std::set<const FieldRow *>> _classnameIndex;
    //std::unordered_map<std::string, std::set<const FieldRow *>> _digestIndex;

};

}

#pragma pack(pop)
#endif
