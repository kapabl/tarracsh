#ifndef TARRACSH_CLASSFILE_TABLE_H
#define TARRACSH_CLASSFILE_TABLE_H
#include <string>
#include "Table.h"
#include "FilesTable.h"

#pragma pack( push, 1 )
namespace org::kapa::tarracsh::db::tables {


struct ClassfileRow : AutoIncrementedRow {
    columns::RefCol file{};
    columns::StringCol classname{};
    columns::StringCol package{};
    columns::UInt64Col lastWriteTime{};
    columns::UInt64Col size{};
    columns::Int32Col crc{};
    columns::DigestCol digest{};

    ClassfileRow() = default;

    explicit ClassfileRow(const FileRow &fileRow) {
        file.id = fileRow.id;
    }

};

class ClassfilesTable : public Table<ClassfileRow> {

public:
    explicit ClassfilesTable(db::Database &db,
                             const std::string &tablename,
                             std::shared_ptr<FilesTable> filesTable)
        : Table(db, tablename), _filesTable(std::move(filesTable)) {
    }

    [[nodiscard]] static std::string getStrongClassname(const char *filename, const char *jvmClassname) {
        std::string result(filename);

        if (jvmClassname != nullptr && jvmClassname[0] != 0) {
            result += "@";
            result += jvmClassname;
        }

        return result;
    }

    [[nodiscard]] std::string getStrongClassname(const FileRow &fileRow, const char *classname) const {
        std::string result(getStrongClassname(
            _stringPool->getCString(fileRow.filename), classname));
        return result;
    }

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
