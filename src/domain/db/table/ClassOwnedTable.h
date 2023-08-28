#ifndef TARRACSH_CLASS_OWNED_TABLE_H
#define TARRACSH_CLASS_OWNED_TABLE_H
#include <string>
#include <map>
#include <set>

#include "Classfiles.h"
#include "infrastructure/db/table/Table.h"
#include "Files.h"

#pragma pack( push, 1 )
namespace kapa::tarracsh::domain::db::table {

class ClassOwnedTable : public  infrastructure::db::table::Table {

public:
    explicit ClassOwnedTable(infrastructure::db::Database &db,
                             const std::string &tablename,
                             std::shared_ptr<Classfiles> classfiles);
    auto deleteClass(ClassfileRow *classfileRow) -> uint64_t;

protected:
    const std::shared_ptr<Classfiles> _classfiles;
    std::map<uint64_t, std::set<uint64_t>> _ownerClassIndex;
};

}

#pragma pack(pop)
#endif
