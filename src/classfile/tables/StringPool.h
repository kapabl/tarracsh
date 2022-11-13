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
#include "../StringUtils.h"
#include "../FilesystemUtils.h"


#pragma pack( push, 1 )

namespace org::kapa::tarracsh::tables {


constexpr const char *StringPoolExtension = ".string-pool";

typedef uint64_t StringPoolItem;
// struct StringPoolItem {
//     //char* ptr;
//     uint64_t offset{0u};
// };


class StringPool {


public:
    constexpr static uint64_t StartPoolSize = 1024u * 1024u * 500u;
    constexpr static uint64_t ReadChunkSize = 1024u * 4u;

    explicit StringPool(std::string filename)
        : _filename(std::move(filename)) {
        _pool = static_cast<char *>(malloc(StartPoolSize));

        add("");
    }

    [[nodiscard]] std::string getFilename() { return _filename; }

    ~StringPool() {
        free(_pool);
    }

    uint64_t toOffset(const char *stringPtr) const {

        const auto result = stringPtr - _pool;
        return result;

    }

    [[nodiscard]] char *toPtr(const uint64_t offset) const {

        assert(offset < _size);

        const auto result = &_pool[offset];
        return result;
    }

    StringPoolItem add(const std::wstring &wStringValue) {
        const std::string stringValue = stringUtils::utf16ToUtf8(wStringValue);
        const auto result = internalAdd(stringValue);
        return result;
    }


    StringPoolItem add(const std::string &stringValue) {
        const auto result = internalAdd(stringValue);
        return result;
    }

    bool read() {

        if (!std::filesystem::exists(_filename)) return true;

        const auto fileSize = file_size(std::filesystem::path(_filename));

        if (fileSize > _size) {
            free(_pool);
            _size = fileSize;
            _pool = static_cast<char *>(malloc(_size));
        }

        std::ifstream file(_filename, std::ios::binary);
        file.unsetf(std::ios::skipws);
        _position = 0;

        while (!file.eof()) {
            file.read(&_pool[_position], ReadChunkSize);
            _position += file.gcount();
        }

        uint64_t currentPosition = 0;

        while (currentPosition < _position) {
            std::string value(&_pool[currentPosition]);
            _hash[value] = currentPosition;
            currentPosition += value.length() + 1;
        }
        _isDirty = false;
        return true;

    }

    void write() const {
        fsUtils::backupPrevFile(_filename);
        std::ofstream file(_filename, std::ios::binary);
        file.unsetf(std::ios::skipws);
        file.write(_pool, _position);
    }

    [[nodiscard]] bool isDirty() const { return _isDirty; }

    bool clean() {
        auto result = true;
        if (std::filesystem::exists(_filename)) {
            result = std::filesystem::remove(_filename);
            if (!result) {
                std::cout << std::format("Error removing string pool file: {}", _filename) << std::endl;
            }
        }
        return result;
    }

    [[nodiscard]] const char *getCString(const StringPoolItem &item) {
        std::shared_lock readersLock(_sharedMutex);
        const auto result = &_pool[item];
        return result;
    }


private:
    bool _isDirty{false};
    char *_pool{nullptr};
    uint64_t _position{0};

    std::unordered_map<std::string, uint64_t> _hash;

    uint64_t _size{StartPoolSize};
    std::string _filename;
    std::shared_mutex _sharedMutex;


    StringPoolItem internalAdd(const std::string &stringValue) {
        std::unique_lock writeLock(_sharedMutex);

        StringPoolItem result;
        const auto it = _hash.find(stringValue);

        if (it == _hash.end()) {

            if (stringValue.length() + _position >= _size) {
                reallocPool();
            }

            _hash[stringValue] = _position;
            _isDirty = true;
            result = _position;
            if (stringValue.length() > 0) {
                const char *begin = &*stringValue.begin();
                std::memcpy(&_pool[result], begin, stringValue.length());
            }

            _position += stringValue.length();
            _pool[_position] = 0;
            _position++;

        } else {
            result = it->second;
        }

        return result;

    }

    void reallocPool() {
        _size = _size * 2;
        _pool = static_cast<char *>(realloc(_pool, _size));
    }

};

}


#undef CHUNK_SIZE

#pragma pack(pop)

#endif
