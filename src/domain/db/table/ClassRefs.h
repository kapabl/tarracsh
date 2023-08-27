#ifndef TARRACSH_CLASS_REFS_TABLE_H
#define TARRACSH_CLASS_REFS_TABLE_H
#include <string>

#include "Classfiles.h"
#include "infrastructure/db/table/Table.h"
#include "Files.h"

#pragma pack( push, 1 )
namespace kapa::tarracsh::domain::db::table {


struct ClassRefRow : infrastructure::db::table::AutoIncrementedRow {
    infrastructure::db::table::column::RefCol ownerClass{};
    infrastructure::db::table::column::StringCol name{};

    //TODO what about multiple target classes
    infrastructure::db::table::column::RefCol targetClass{};

    ClassRefRow() = default;
    explicit ClassRefRow(const ClassfileRow& classFileRow) {
        setClass(classFileRow);
    }

    void setClass(const ClassfileRow &classFileRow) {
        ownerClass.id = classFileRow.id;
    }

};

class ClassRefs : public  infrastructure::db::table::Table {

public:
    explicit ClassRefs(infrastructure::db::Database &db,
                             const std::string &tablename,
                             std::shared_ptr<Classfiles> classfiles);

    [[nodiscard]] std::string getKey(const infrastructure::db::table::AutoIncrementedRow* row) override;
    [[nodiscard]] std::string getStrongMethodName(const ClassRefRow &row) const;

    void defineColumns() override;

private:
    const std::shared_ptr<Classfiles> _classfiles;
    //std::unordered_map<StringCol, std::set<const ClassRefRow *>> _jarIndex;
    // std::unordered_map<StringCol, std::set<const ClassRefRow *>> _classnameIndex;
    //std::unordered_map<std::string, std::set<const ClassRefRow *>> _digestIndex;

};

}

#pragma pack(pop)
#endif
