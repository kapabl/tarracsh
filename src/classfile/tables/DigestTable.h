#ifndef PUBLIC_SHA_TABLE_H
#define PUBLIC_SHA_TABLE_H
#include <string>
#include "Table.h"

namespace org::kapa::tarracsh::tables {

enum EntryType { Classfile, Jar, Directory };

#define MD5_DIGEST_LENGTH 16

struct MD5 {
    unsigned char buf[MD5_DIGEST_LENGTH]{};
    bool operator==(const MD5 &right) const = default;
};

struct DigestRow {

    EntryType type{Classfile};
    StringPoolItem filename{0u};
    StringPoolItem classname{0u};
    StringPoolItem package{0u};
    uint64_t lastWriteTime{};
    uint64_t fileSize{};
    uint64_t count{1};
    int crc{0};
    MD5 md5{};

    DigestRow() = default;
};

class DigestTable : public Table<DigestRow, std::string> {

public:
    DigestTable(const std::string &filename)
        : Table(filename) {

    }

    [[nodiscard]] static std::string createKey(const char *filename, const char *classname) {
        std::string result(filename);

        if (classname != nullptr && classname[0] != 0) {
            result += "@";
            result += classname;
        }

        return result;
    }

    [[nodiscard]] std::string createKey(const StringPoolItem &filename, const StringPoolItem &classname) const {
        std::string result(createKey(_stringPool->getCString(filename), _stringPool->getCString(classname)));
        return result;
    }

    void updateJarIndex(const DigestRow *pBeforeRow, const DigestRow *pAfterRow);

    void updateClassnameIndex(const DigestRow *pBeforeRow, const DigestRow *pAfterRow);

    void updateDigestIndex(const DigestRow *pBeforeRow, const DigestRow *pAfterRow);

    void updateIndexes(const DigestRow *pBeforeRow, const DigestRow *pAfterRow) override {
        if (pAfterRow->type == EntryType::Jar) {
            updateJarIndex(pBeforeRow, pAfterRow);
        }
        updateClassnameIndex(pBeforeRow, pAfterRow);
        updateDigestIndex(pBeforeRow, pAfterRow);
    }


    std::string getKey(const DigestRow &row) override {
        return createKey(row);
    }


    [[nodiscard]] std::string createKey(const DigestRow &row) const {
        auto result = createKey(row.filename, row.classname);
        return result;
    }

private:
    std::unordered_map<StringPoolItem, std::set<const DigestRow *>> _jarIndex;
    std::unordered_map<StringPoolItem, std::set<const DigestRow *>> _classnameIndex;

    //TODO 
    std::unordered_map<std::string, std::set<const DigestRow *>> _digestIndex;
};

}

#endif
