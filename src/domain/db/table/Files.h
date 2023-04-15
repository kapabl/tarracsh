#ifndef TARRACSH_FILE_TABLE_H
#define TARRACSH_FILE_TABLE_H
#include <string>
#include "../columns/Columns.h"
#include "infrastructure/db/table/Table.h"
#pragma pack( push, 1 )
namespace kapa::tarracsh::domain::db::table {


struct FileRow : infrastructure::db::table::AutoIncrementedRow {
    infrastructure::db::table::column::Int32Col type{};
    infrastructure::db::table::column::StringCol filename{};
    infrastructure::db::table::column::Int64Col lastWriteTime{};
    infrastructure::db::table::column::UInt64Col fileSize{};
    infrastructure::db::table::column::UInt32Col classfileCount{};
    infrastructure::db::table::column::DigestCol digest;


    FileRow()
        : type(digest::column::EntryType::Classfile) {
    }

};

class Files : public infrastructure::db::table::Table {

public:
    explicit Files(infrastructure::db::Database &db, const std::string &name);
    [[nodiscard]] static std::string createKey(const char *filename);
    [[nodiscard]] std::string createKey(const infrastructure::db::table::column::StringCol &filename) const;
    std::string getKey(const infrastructure::db::table::AutoIncrementedRow *row) override;
    [[nodiscard]] const char* getFilename(const FileRow *row) const;
    [[nodiscard]] std::string createKey(const infrastructure::db::table::AutoIncrementedRow* row) const;

protected:
    void defineColumns() override;

private:
};

}

#pragma pack(pop)
#endif
