#ifndef TARRACSH_CLASSFILE_TABLE_H
#define TARRACSH_CLASSFILE_TABLE_H
#include <string>
#include "infrastructure/db/table/Table.h"
#include "Files.h"

#pragma pack( push, 1 )
namespace kapa::tarracsh::domain::db::table {


struct ClassFileRow : infrastructure::db::table::AutoIncrementedRow {
    infrastructure::db::table::column::RefCol file{};
    infrastructure::db::table::column::StringCol classname{};
//    infrastructure::db::table::column::StringCol package{};
    infrastructure::db::table::column::UInt64Col lastWriteTime{};
    infrastructure::db::table::column::UInt64Col size{};
    infrastructure::db::table::column::Int32Col crc{};
    infrastructure::db::table::column::DigestCol digest{};

    ClassFileRow() = default;
    explicit ClassFileRow(const FileRow& fileRow) {
        setFile(fileRow);
    }

    void setFile(const FileRow &fileRow) {
        file.id = fileRow.id;
    }

    [[nodiscard]] auto hasValidFile() const {
        const bool result = !file.isInvalid();
        return result;
    }

};

class ClassFiles : public  infrastructure::db::table::Table {

public:
    explicit ClassFiles(infrastructure::db::Database &db,
                        const std::string &tablename,
                        std::shared_ptr<Files> filesTable);

    [[nodiscard]] std::string getStrongClassname(const FileRow &fileRow, const char *classname) const;
    [[nodiscard]] std::string getKey(const infrastructure::db::table::AutoIncrementedRow* row) override;
    [[nodiscard]] std::string getStrongClassname(uint64_t id);
    [[nodiscard]] std::string getStrongClassname(const ClassFileRow &row) const;

    void defineColumns() override;
    auto getClassFileRow( int64_t rowId );
    auto getClassname(const ClassFileRow &row) const -> std::string;
private:
    const std::shared_ptr<Files> _filesTable;
    //std::unordered_map<StringCol, std::set<const ClassFileRow *>> _jarIndex;
    // std::unordered_map<StringCol, std::set<const ClassFileRow *>> _classnameIndex;
    //std::unordered_map<std::string, std::set<const ClassFileRow *>> _digestIndex;

};

}

#pragma pack(pop)
#endif
