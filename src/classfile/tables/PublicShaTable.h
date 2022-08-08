#ifndef PUBLIC_SHA_TABLE_H
#define PUBLIC_SHA_TABLE_H
#include <string>
#include <utility>
#include "Table.h"

namespace org::kapa::tarracsh::tables {

enum EntryType { Classfile, Jar, Directory };

struct Sha256 {
    uint64_t _18b{};
    uint64_t _28b{};
    uint64_t _38b{};
    uint64_t _48b{};
    bool operator==(const Sha256 & sha256) const = default;
};

struct ShaRow {


    EntryType type{Classfile};
    PoolStringItem filename;
    PoolStringItem classname;
    PoolStringItem package;
    uint64_t lastWriteTime{};
    uint64_t fileSize{};
    Sha256 sha256;

    ShaRow()
        : sha256{0, 0, 0, 0} {
    }


    [[nodiscard]] std::string getKey() const {
        std::string result(filename.ptr);
        return result;
    }

    void serialize(const std::shared_ptr<StringPool>& stringPool,  ShaRow &output) const {
        output = *this;
        output.filename.offset = stringPool->toOffset(filename.ptr);
        output.classname.offset = stringPool->toOffset(classname.ptr);
        output.package.offset = stringPool->toOffset(package.ptr);
    }

    void deserialize(const std::shared_ptr<StringPool>& stringPool, const ShaRow &input) {
        const auto destination = const_cast<ShaRow *>(this);
        *destination = input;
        filename.ptr = stringPool->toPtr(input.filename.offset);
        classname.ptr = stringPool->toPtr(input.classname.offset);
        package.ptr = stringPool->toPtr(input.package.offset);
    }

    void deserialize(const std::shared_ptr<StringPool>& stringPool ) {
        filename.ptr = stringPool->toPtr(filename.offset);
        classname.ptr = stringPool->toPtr(classname.offset);
        package.ptr = stringPool->toPtr(package.offset);
    }
};

class PublicShaTable : public Table<ShaRow, std::string> {

public:
    PublicShaTable( const std::string& filename):
        Table( filename) {

        
    }


};

}

#endif
