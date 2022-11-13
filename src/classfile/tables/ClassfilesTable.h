#ifndef TARRACSH_DIGEST_TABLE_H
#define TARRACSH_DIGEST_TABLE_H
#include <string>

#include "FilesTable.h"
#include "Table.h"
#pragma pack( push, 1 )
namespace org::kapa::tarracsh::tables {


struct ClassfileRow : AutoIncrementedRow {

    ColumnRef file{};
    StringPoolItem classname{0u};
    StringPoolItem package{0u};
    uint64_t lastWriteTime{};
    uint64_t size{};
    int crc{0};
    Md5Column md5{};

    ClassfileRow() = default;

    explicit ClassfileRow(const FileRow &fileRow) {
        file.id = fileRow.id;
    }

};

class ClassfilesTable : public Table<ClassfileRow, std::string> {

public:
    explicit ClassfilesTable(const std::string &filename,
                             const std::shared_ptr<StringPool> stringPool, 
                             std::shared_ptr<FilesTable> filesTable)
        : Table(filename, stringPool), _filesTable(std::move(filesTable)) {
    }

    [[nodiscard]] static std::string createKey(const char *filename, const char *classname) {
        std::string result(filename);

        if (classname != nullptr && classname[0] != 0) {
            result += "@";
            result += classname;
        }

        return result;
    }

    [[nodiscard]] std::string createKey(const FileRow &fileRow, const char *classname) const {
        std::string result(createKey(
            _stringPool->getCString(fileRow.filename), classname));
        return result;
    }

    void updateClassnameIndex(const ClassfileRow *pBeforeRow, const ClassfileRow *pAfterRow);

    void updateDigestIndex(const ClassfileRow *pBeforeRow, const ClassfileRow *pAfterRow);

    void updateIndexes(const ClassfileRow *pBeforeRow, const ClassfileRow *pAfterRow) override {
        //TODO 
        // if (pAfterRow->type == EntryType::Jar) {
        //     updateJarIndex(pBeforeRow, pAfterRow);
        // }
        // updateClassnameIndex(pBeforeRow, pAfterRow);
        // updateDigestIndex(pBeforeRow, pAfterRow);
    }


    std::string getKey(const ClassfileRow &row) override {
        return createKey(row);
    }


    [[nodiscard]] std::string createKey(const ClassfileRow &row) const {
        const auto pFileRow = _filesTable->getRow(row.file.id);

        auto result = createKey(*pFileRow, _stringPool->getCString(row.classname));
        return result;
    }

private:
    const std::shared_ptr<FilesTable> _filesTable;
    std::unordered_map<StringPoolItem, std::set<const ClassfileRow *>> _jarIndex;
    std::unordered_map<StringPoolItem, std::set<const ClassfileRow *>> _classnameIndex;

    //TODO 
    std::unordered_map<std::string, std::set<const ClassfileRow *>> _digestIndex;
};

}

#pragma pack(pop)
#endif
