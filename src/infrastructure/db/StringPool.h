#ifndef TARRACSH_STRING_POOL_H
#define TARRACSH_STRING_POOL_H
#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <mutex>
#include <shared_mutex>
#include <string>
#include <unordered_map>
#include "../string/StringUtils.h"
#include "../filesystem/Utils.h"


#pragma pack( push, 1 )

namespace kapa::infrastructure::db {


constexpr const char *StringPoolExtension = ".string-pool";

namespace table::column {

typedef uint64_t StringCol;
// struct StringCol {
//     //char* ptr;
//     uint64_t offset{0u};
// };
}

class StringPool {


public:
    constexpr static uint64_t StartPoolSize = 1024u * 1024u * 500u;
    constexpr static uint64_t ReadChunkSize = 1024u * 4u;

    explicit StringPool(std::string filename);
    [[nodiscard]] std::string getFilename();

    ~StringPool();

    table::column::StringCol add(const std::string &stringValue);
    bool read();
    [[nodiscard]] bool write() const;
    bool clean();
    [[nodiscard]] const char *getCString(const table::column::StringCol &item);
    void backup();


private:
    bool _isDirty{false};
    char *_pool{nullptr};
    uint64_t _position{0};

    std::unordered_map<std::string_view, uint64_t> _hash;

    uint64_t _size{StartPoolSize};
    std::string _filename;
    std::shared_mutex _sharedMutex;


    table::column::StringCol internalAdd(const std::string &stringValue);

    void reallocPool();

};

}


#undef CHUNK_SIZE

#pragma pack(pop)

#endif
