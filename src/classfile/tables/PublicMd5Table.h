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
    bool operator==(const MD5 & right) const = default;
};

struct Md5Row {


    EntryType type{Classfile};
    PoolStringItem filename;
    PoolStringItem classname;
    PoolStringItem package;
    uint64_t lastWriteTime{};
    uint64_t fileSize{};
    MD5 md5{};

    Md5Row()
       {
    }


    [[nodiscard]] std::string getKey() const {
        std::string result(filename.ptr);
        return result;
    }

    void serialize(const std::shared_ptr<StringPool>& stringPool,  Md5Row &output) const {
        output = *this;
        output.filename.offset = stringPool->toOffset(filename.ptr);
        output.classname.offset = stringPool->toOffset(classname.ptr);
        output.package.offset = stringPool->toOffset(package.ptr);
    }

    void deserialize(const std::shared_ptr<StringPool>& stringPool, const Md5Row &input) {
        const auto destination = const_cast<Md5Row *>(this);
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

class PublicMd5Table : public Table<Md5Row, std::string> {

public:
    PublicMd5Table( const std::string& filename):
        Table( filename) {

        
    }


};

}

#endif
