#ifndef TARRACSH_METHOD_REFS_TABLE_H
#define TARRACSH_METHOD_REFS_TABLE_H

#include <string>
#include <memory>

#include "Methods.h"
#include "ClassFiles.h"
#include "ClassOwnedTable.h"
#include "infrastructure/db/table/Table.h"
#include "Files.h"

#pragma pack( push, 1 )
namespace kapa::tarracsh::domain::db::table {


    struct MethodRefRow : infrastructure::db::table::AutoIncrementedRow {
        infrastructure::db::table::column::RefCol ownerClass{};
        infrastructure::db::table::column::StringCol classname{};
        infrastructure::db::table::column::StringCol name{};
        infrastructure::db::table::column::StringCol descriptor{};

//        infrastructure::db::table::column::RefCol targetClass{};
//        infrastructure::db::table::column::RefCol targetMethod{};


        MethodRefRow() = default;

        explicit MethodRefRow(const ClassFileRow &classFileRow) {
            setClass(classFileRow);
        }

        void setClass(const ClassFileRow &classFileRow) {
            ownerClass.id = classFileRow.id;
        }

    };

    class MethodRefs : public ClassOwnedTable {

    public:
        explicit MethodRefs(infrastructure::db::Database &db,
                            const std::string &tablename,
                            std::shared_ptr<ClassFiles> classfiles,
                            std::shared_ptr<Methods> methods);

        [[nodiscard]] std::string getKey(const infrastructure::db::table::AutoIncrementedRow *row) override;

        [[nodiscard]] std::string getStrongRefName(const MethodRefRow &row) const;

        void defineColumns() override;

    private:
        const std::shared_ptr<Methods> _methods;
    };

}

#pragma pack(pop)
#endif
