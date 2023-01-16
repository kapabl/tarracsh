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
    columns::DigestCol digest{};

    ClassfileRow() = default;

    explicit ClassfileRow(const FileRow &fileRow) {
        file.id = fileRow.id;
    }

};

class ClassfilesTable : public  infrastructure::db::tables::Table<ClassfileRow> {

public:
    explicit ClassfilesTable(infrastructure::db::Database &db,
                             const std::string &tablename,
                             std::shared_ptr<FilesTable> filesTable)
        : Table(db, tablename), _filesTable(std::move(filesTable)) {
    }

    [[nodiscard]] std::string getStrongClassname(const FileRow &fileRow, const char *classname) const;

    // void updateClassnameIndex(const ClassfileRow *pBeforeRow, const ClassfileRow *pAfterRow);
    //
    // void updateDigestIndex(const ClassfileRow *pBeforeRow, const ClassfileRow *pAfterRow);

    void updateIndexes(const ClassfileRow *pBeforeRow, const ClassfileRow *pAfterRow) override {
        //TODO 
        // if (pAfterRow->type == EntryType::Jar) {
        //     updateJarIndex(pBeforeRow, pAfterRow);
        // }
        // updateClassnameIndex(pBeforeRow, pAfterRow);
        // updateDigestIndex(pBeforeRow, pAfterRow);
    }


    std::string getKey(const ClassfileRow &row) override {
        return getStrongClassname(row);
    }


    [[nodiscard]] std::string getStrongClassname(const ClassfileRow &row) const {
        const auto pFileRow = _filesTable->getRow(row.file.id);

        auto result = getStrongClassname(*pFileRow, _stringPool->getCString(row.classname));
        return result;
    }

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
