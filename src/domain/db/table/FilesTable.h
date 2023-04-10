#ifndef TARRACSH_FILE_TABLE_H
#define TARRACSH_FILE_TABLE_H
#include <string>
#include "../columns/Columns.h"
#include "infrastructure/db/Database.h"
#include "infrastructure/db/table/Table.h"
#pragma pack( push, 1 )
namespace kapa::tarracsh::domain::db::digest {


struct FileRow : infrastructure::db::tables::AutoIncrementedRow {
    infrastructure::db::tables::columns::Int32Col type{};
    infrastructure::db::tables::columns::StringCol filename{};
    infrastructure::db::tables::columns::Int64Col lastWriteTime{};
    infrastructure::db::tables::columns::UInt64Col fileSize{};
    infrastructure::db::tables::columns::UInt32Col classfileCount{};
    infrastructure::db::tables::columns::DigestCol digest;


    FileRow()
        : type(columns::EntryType::Classfile) {
    }

};

class FilesTable : public infrastructure::db::tables::Table {

public:
    explicit FilesTable(infrastructure::db::Database &db, const std::string &name);
    [[nodiscard]] static std::string createKey(const char *filename);
    [[nodiscard]] std::string createKey(const infrastructure::db::tables::columns::StringCol &filename) const;
    std::string getKey(const infrastructure::db::tables::AutoIncrementedRow *row) override;
    [[nodiscard]] const char* getFilename(const FileRow *row) const;
    [[nodiscard]] std::string createKey(const infrastructure::db::tables::AutoIncrementedRow* row) const;

protected:
    void defineColumns() override;

private:
};

}

#pragma pack(pop)
#endif
