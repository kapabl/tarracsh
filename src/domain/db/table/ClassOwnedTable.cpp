#include "ClassOwnedTable.h"

#include "ClassFiles.h"

using namespace kapa::tarracsh::domain::db::table;
using namespace kapa::infrastructure::db::table::column;
using kapa::infrastructure::db::table::AutoIncrementedRow;


ClassOwnedTable::ClassOwnedTable(
        infrastructure::db::Database &db,
        const std::string &tablename,
        size_t rowSize,
        std::shared_ptr<ClassFiles> classfiles) :
        Table(db, tablename, rowSize), _classfiles(std::move(classfiles)) {
}

auto ClassOwnedTable::deleteClass(ClassFileRow *classfileRow) -> uint64_t {
    uint64_t result = 0;
    auto ids = _ownerClassIndex[classfileRow->id];
    for (auto id: ids) {
        if (deleteRow(id)) {
            result++;
        }
    }
    _ownerClassIndex.erase(classfileRow->id);
    return result;
}