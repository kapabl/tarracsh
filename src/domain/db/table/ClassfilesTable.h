#ifndef TARRACSH_CLASSFILE_TABLE_H
#define TARRACSH_CLASSFILE_TABLE_H
#include <string>
#include "../../../infrastructure/db/table/Table.h"
#include "FilesTable.h"

#pragma pack( push, 1 )
namespace kapa::tarracsh::domain::db::digest {


struct ClassfileRow : infrastructure::db::tables::AutoIncrementedRow {
    infrastructure::db::tables::columns::RefCol file{};
    infrastructure::db::tables::columns::StringCol classname{};
    infrastructure::db::tables::columns::StringCol package{};
    infrastructure::db::tables::columns::UInt64Col lastWriteTime{};
    infrastructure::db::tables::columns::UInt64Col size{};
    infrastructure::db::tables::columns::Int32Col crc{};
    infrastructure::db::tables::columns::DigestCol digest{};

    ClassfileRow() = default;

    explicit ClassfileRow(const FileRow &fileRow) {
        file.id = fileRow.id;
    }

};

class ClassfilesTable : public  infrastructure::db::tables::Table<ClassfileRow> {

public:
    explicit ClassfilesTable(infrastructure::db::Database &db,
                             const std::string &tablename,
                             std::shared_ptr<FilesTable> filesTable);

    [[nodiscard]] std::string getStrongClassname(const FileRow &fileRow, const char *classname) const;
    std::string getKey(const ClassfileRow &row) override;
    [[nodiscard]] std::string getStrongClassname(const ClassfileRow &row) const;

    void defineColumns() override;

private:
    const std::shared_ptr<FilesTable> _filesTable;
    //std::unordered_map<StringCol, std::set<const ClassfileRow *>> _jarIndex;
    // std::unordered_map<StringCol, std::set<const ClassfileRow *>> _classnameIndex;
    //std::unordered_map<std::string, std::set<const ClassfileRow *>> _digestIndex;

};

}

#pragma pack(pop)
#endif
