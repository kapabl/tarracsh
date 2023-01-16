#ifndef TARRACSH_FILE_TABLE_H
#define TARRACSH_FILE_TABLE_H
#include <string>
#include "../columns/Columns.h"
#include "../../../infrastructure/db/Database.h"
#include "../../../infrastructure/db/table/Table.h"
#pragma pack( push, 1 )
namespace kapa::tarracsh::domain::db::digest {


struct FileRow : infrastructure::db::tables::AutoIncrementedRow {
    infrastructure::db::tables::columns::Int32Col type{};
    infrastructure::db::tables::columns::StringCol filename{};
    infrastructure::db::tables::columns::Int64Col lastWriteTime{};
    infrastructure::db::tables::columns::UInt64Col fileSize{};
    infrastructure::db::tables::columns::UInt32Col classfileCount{};
    columns::DigestCol digest;


    FileRow()
        : type(columns::EntryType::Classfile) {
    }

};


class FilesTable : public infrastructure::db::tables::Table<FileRow> {

public:
    explicit FilesTable(infrastructure::db::Database &db, const std::string &tablename)
        : Table(db, tablename) {
    }

    [[nodiscard]] static std::string createKey(const char *filename) {
        std::string result(filename);
        return result;
    }

    [[nodiscard]] std::string createKey(const infrastructure::db::tables::columns::StringCol &filename) const {
        std::string result(createKey(_stringPool->getCString(filename)));
        return result;
    }

    void updateIndexes(const FileRow *pBeforeRow, const FileRow *pAfterRow) override {
        //TODO
    }

    std::string getKey(const FileRow &row) override {
        return createKey(row);
    }

    const char* getFilename(const FileRow& row) const {
        const auto result = _stringPool->getCString(row.filename);
        return result;
    }


    [[nodiscard]] std::string createKey(const FileRow &row) const;


protected:
    void defineColumns() override;

private:
};

}

#pragma pack(pop)
#endif
