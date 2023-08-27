#ifndef TARRACSH_METHOD_REFS_TABLE_H
#define TARRACSH_METHOD_REFS_TABLE_H
#include <string>
#include <memory>

#include "Methods.h"
#include "Classfiles.h"
#include "infrastructure/db/table/Table.h"
#include "Files.h"

#pragma pack( push, 1 )
namespace kapa::tarracsh::domain::db::table {


struct MethodRefRow : infrastructure::db::table::AutoIncrementedRow {
    infrastructure::db::table::column::RefCol ownerClass{};
    infrastructure::db::table::column::StringCol classname{};
    infrastructure::db::table::column::StringCol name{};
    infrastructure::db::table::column::StringCol descriptor{};

    infrastructure::db::table::column::RefCol targetClass{};
    infrastructure::db::table::column::RefCol targetMethod{};


    MethodRefRow() = default;
    explicit MethodRefRow(const ClassfileRow& classFileRow) {
        setClass(classFileRow);
    }

    void setClass(const ClassfileRow &classFileRow) {
        ownerClass.id = classFileRow.id;
    }

    void setTargetClass(const ClassfileRow &classFileRow) {
        targetClass.id = classFileRow.id;
    }

    void setTargetMethod(const MethodRow &methodRow) {
        targetField.id = methodRow.id;
    }

};

class MethodRefs : public  infrastructure::db::table::Table {

public:
    explicit MethodRefs(infrastructure::db::Database &db,
                             const std::string &tablename,
                             std::shared_ptr<Methods> methods,
                             std::shared_ptr<Classfiles> classfiles);

    [[nodiscard]] std::string getKey(const infrastructure::db::table::AutoIncrementedRow* row) override;
    [[nodiscard]] std::string getStrongMethodName(const MethodRefRow &row) const;

    void defineColumns() override;

private:
    const std::shared_ptr<Classfiles> _classfiles;
    const std::shared_ptr<Methods> _methods;
    //std::unordered_map<StringCol, std::set<const MethodRefRow *>> _jarIndex;
    // std::unordered_map<StringCol, std::set<const MethodRefRow *>> _classnameIndex;
    //std::unordered_map<std::string, std::set<const MethodRefRow *>> _digestIndex;

};

}

#pragma pack(pop)
#endif
