#ifndef TARRACSH_STRING_POOL_H
#define TARRACSH_STRING_POOL_H
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include "../StringUtils.h"


#pragma pack( push, 1 )

namespace org::kapa::tarracsh::tables {


constexpr const char* StringPoolExtension = ".string-pool";


class StringPool {


public:
    constexpr static uint64_t StartPoolSize = 1024u * 1024u * 500u;
    constexpr static uint64_t ReadChunkSize = 1024u * 4u;

    explicit StringPool(std::string filename)
        : _filename(std::move(filename)) {
        _pool = static_cast<char *>(malloc(StartPoolSize));

        add("");
    }

    ~StringPool() {
        free(_pool);
    }

    uint64_t toOffset(const char *stringPtr) const {

        const auto result = stringPtr - _pool;
        return result;

    }

    [[nodiscard]] char *toPtr(const uint64_t offset) const {

        const auto result = &_pool[offset];
        return result;
    }

    char* add(const std::wstring& wStringValue) {
        const std::string stringValue = stringUtils::utf16ToUtf8(wStringValue);
        const auto result = add(stringValue);
        return result;
    }

 
    char *add(const std::string &stringValue) {

        char *result = nullptr;
        const auto it = _hash.find(stringValue);

        if (it == _hash.end()) {

            if (stringValue.length() + _position >= _poolSize) {
                reallocPool();
            }

            _hash[stringValue] = _position;
            result = &_pool[_position];
            if (stringValue.length() > 0)
            {
                const char* begin = &*stringValue.begin();
                std::memcpy(result, begin, stringValue.length());
            }
            
            _position += stringValue.length();
            _pool[_position] = 0;
            _position++;

        } else {
            result = &_pool[it->second];
        }

        return result;

    }

    bool read() {

        if (!std::filesystem::exists(_filename)) return true;

        const auto fileSize = std::filesystem::file_size(std::filesystem::path(_filename));

        if (fileSize > _poolSize) {
            free(_pool);
            _poolSize = fileSize;
            _pool = static_cast<char *>(malloc(_poolSize));
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
        return true;

    }

    void write() const {
        std::ofstream file(_filename, std::ios::binary);
        file.unsetf(std::ios::skipws);
        file.write(_pool, _position);
    }

    bool clean() {
        auto result = true;
        if (std::filesystem::exists(_filename)) {
            result = std::filesystem::remove(_filename);
            if ( !result ) {
                std::cout << std::format("Error removing string pool file: {}", _filename) << std::endl;
            }
        }
        return result;
    }


private:
    char *_pool{nullptr};
    uint64_t _position{0};

    std::unordered_map<std::string, uint64_t> _hash;

    uint64_t _poolSize{StartPoolSize};
    std::string _filename;


    void reallocPool() {
        _poolSize = _poolSize * 2;
        _pool = static_cast<char *>(realloc(_pool, _poolSize));
    }

};

}

union StringPoolItem {
    char* ptr{ nullptr };
    uint64_t offset;
};

#undef CHUNK_SIZE

#pragma pack(pop)

#endif
