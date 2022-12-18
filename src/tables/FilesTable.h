#ifndef TARRACSH_FILE_TABLE_H
#define TARRACSH_FILE_TABLE_H
#include <string>
#include "Table.h"
#pragma pack( push, 1 )
namespace org::kapa::tarracsh::db::tables {


START_AUTOINCREMENT_ROW(File)
    DECLARE_ROW_VALUE(Int32, type)
    DECLARE_ROW_VALUE(String, filename)
    DECLARE_ROW_VALUE(UInt64, lastWriteTime)
    DECLARE_ROW_VALUE(UInt64, fileSize)
    DECLARE_ROW_VALUE(UInt32, classfileCount)
    DECLARE_ROW_VALUE(Digest, digest)

    FileRow()
        : type(columns::EntryType::Classfile) {}

END_ROW

class FilesTable : public Table<FileRow> {

public:
    explicit FilesTable(db::Database& db, const std::string& tablename)
        : Table(db, tablename) {
    }

    [[nodiscard]] static std::string createKey(const char *filename) {
        std::string result(filename);
        return result;
    }

    [[nodiscard]] std::string createKey(const columns::StringCol &filename) const {
        std::string result(createKey(_stringPool->getCString(filename)));
        return result;
    }

    void updateIndexes(const FileRow *pBeforeRow, const FileRow *pAfterRow) override {
        //TODO
    }

    std::string getKey(const FileRow &row) override {
        return createKey(row);
    }


    [[nodiscard]] std::string createKey(const FileRow &row) const {
        auto result = createKey(row.filename);
        return result;
    }


protected:
    void defineColumns() override;

private:
};

}

#pragma pack(pop)
#endif
