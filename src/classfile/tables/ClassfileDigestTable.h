#ifndef PUBLIC_SHA_TABLE_H
#define PUBLIC_SHA_TABLE_H
#include <string>
#include <utility>
//TODO include openssl to use md5
//#include <openssl/>
#include "Table.h"

namespace org::kapa::tarracsh::tables {

enum EntryType { Classfile, Jar, Directory };

#define MD5_DIGEST_LENGTH 16

struct MD5 {
    unsigned char buf[MD5_DIGEST_LENGTH]{};
    bool operator==(const MD5 &right) const = default;
};

struct ClassfileDigestRow {

    EntryType type{Classfile};
    StringPoolItem filename;
    StringPoolItem classname;
    StringPoolItem package;
    uint64_t lastWriteTime{};
    uint64_t fileSize{};
    uint64_t count{1};
    int crc{0};
    MD5 md5{};

    ClassfileDigestRow() = default;

    [[nodiscard]] std::string getKey() const;

    void serialize(const std::shared_ptr<StringPool> &stringPool, ClassfileDigestRow &output) const {
        output = *this;
        output.filename.offset = stringPool->toOffset(filename.ptr);
        output.classname.offset = stringPool->toOffset(classname.ptr);
        output.package.offset = stringPool->toOffset(package.ptr);
    }

    void deserialize(const std::shared_ptr<StringPool> &stringPool, const ClassfileDigestRow &input) {
        const auto destination = const_cast<ClassfileDigestRow *>(this);
        *destination = input;
        filename.ptr = stringPool->toPtr(input.filename.offset);
        classname.ptr = stringPool->toPtr(input.classname.offset);
        package.ptr = stringPool->toPtr(input.package.offset);
    }

    void deserialize(const std::shared_ptr<StringPool> &stringPool) {
        filename.ptr = stringPool->toPtr(filename.offset);
        classname.ptr = stringPool->toPtr(classname.offset);
        package.ptr = stringPool->toPtr(package.offset);
    }
};

class ClassfileDigestTable : public Table<ClassfileDigestRow, std::string> {

public:
    ClassfileDigestTable(const std::string &filename)
        : Table(filename) {

    }

    static std::string createKey(const char *filename, const char *classname) {
        std::string result(filename);
        if (classname != nullptr) {
            result += "@";
            result += classname;
        }
        return result;
    }

    static std::string createKey(const ClassfileDigestRow &row) {
        auto result = createKey(row.filename.ptr, row.classname.ptr);
        return result;
    }
};

[[nodiscard]] inline std::string ClassfileDigestRow::getKey() const {
    auto result = ClassfileDigestTable::createKey(*this);
    return result;
}

}

#endif
