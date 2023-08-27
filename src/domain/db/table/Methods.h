#ifndef TARRACSH_METHODS_TABLE_H
#define TARRACSH_METHODS_TABLE_H
#include <string>

#include "Classfiles.h"
#include "infrastructure/db/table/Table.h"
#include "Files.h"

#pragma pack( push, 1 )
namespace kapa::tarracsh::domain::db::table {


struct MethodRow : infrastructure::db::table::AutoIncrementedRow {
    infrastructure::db::table::column::RefCol ownerClass{};
    infrastructure::db::table::column::StringCol name{};
    infrastructure::db::table::column::StringCol descriptor{};
//    infrastructure::db::table::column::StringCol signature{};
    infrastructure::db::table::column::UInt64Col callCount{};

    MethodRow() = default;
    explicit MethodRow(const ClassfileRow& classFileRow) {
        setClass(classFileRow);
    }

    void setClass(const ClassfileRow &classFileRow) {
        ownerClass.id = classFileRow.id;
    }

};

class Methods : public  infrastructure::db::table::Table {

public:
    explicit Methods(infrastructure::db::Database &db,
                             const std::string &tablename,
                             std::shared_ptr<Classfiles> classfiles);

    [[nodiscard]] std::string getKey(const infrastructure::db::table::AutoIncrementedRow* row) override;
    [[nodiscard]] std::string getStrongMethodName(const MethodRow &row) const;

    void defineColumns() override;

private:
    const std::shared_ptr<Classfiles> _classfiles;
    //std::unordered_map<StringCol, std::set<const MethodRow *>> _jarIndex;
    // std::unordered_map<StringCol, std::set<const MethodRow *>> _classnameIndex;
    //std::unordered_map<std::string, std::set<const MethodRow *>> _digestIndex;

};

}

#pragma pack(pop)
#endif
