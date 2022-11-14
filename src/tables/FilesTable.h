#ifndef TARRACSH_FILE_TABLE_H
#define TARRACSH_FILE_TABLE_H
#include <string>
#include "Table.h"
#pragma pack( push, 1 )
namespace org::kapa::tarracsh::tables {


#define MD5_DIGEST_LENGTH 16
struct FileRow: AutoIncrementedRow {

    EntryType type{Classfile};
    StringPoolItem filename{0u};
    uint64_t lastWriteTime{};
    uint64_t fileSize{};
    uint32_t classfileCount{};
    DigestColumn digest{};

    // FileRow() = default;
    // FileRow(  const File)
};

class FilesTable : public Table<FileRow, std::string> {

public:
    explicit FilesTable(const std::string& filename, std::shared_ptr<StringPool> stringPool)
        : Table(filename, stringPool) {
    }

    [[nodiscard]] static std::string createKey(const char *filename) {
        std::string result(filename);
        return result;
    }

    [[nodiscard]] std::string createKey(const StringPoolItem &filename) const {
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

private:
};

}

#pragma pack(pop)
#endif
