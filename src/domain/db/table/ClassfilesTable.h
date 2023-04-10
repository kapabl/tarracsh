#ifndef TARRACSH_CLASSFILE_TABLE_H
#define TARRACSH_CLASSFILE_TABLE_H
#include <string>
#include "infrastructure/db/table/Table.h"
#include "FilesTable.h"

#pragma pack( push, 1 )
namespace kapa::tarracsh::domain::db::table {


struct ClassfileRow : infrastructure::db::table::AutoIncrementedRow {
    infrastructure::db::table::column::RefCol file{};
    infrastructure::db::table::column::StringCol classname{};
    infrastructure::db::table::column::StringCol package{};
    infrastructure::db::table::column::UInt64Col lastWriteTime{};
    infrastructure::db::table::column::UInt64Col size{};
    infrastructure::db::table::column::Int32Col crc{};
    infrastructure::db::table::column::DigestCol digest{};

    ClassfileRow() = default;
    explicit ClassfileRow(const FileRow& fileRow) {
        setFile(fileRow);
    }

    void setFile(const FileRow &fileRow) {
        file.id = fileRow.id;
    }

};

class ClassfilesTable : public  infrastructure::db::table::Table {

public:
    explicit ClassfilesTable(infrastructure::db::Database &db,
                             const std::string &tablename,
                             std::shared_ptr<FilesTable> filesTable);

    [[nodiscard]] std::string getStrongClassname(const FileRow &fileRow, const char *classname) const;
    [[nodiscard]] std::string getKey(const infrastructure::db::table::AutoIncrementedRow* row) override;
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
