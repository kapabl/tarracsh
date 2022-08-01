#ifndef TARRACSH_STRING_POOL_H
#define TARRACSH_STRING_POOL_H
#include <optional>
#include <string>
#include <unordered_map>


#pragma pack( push, 1 )

namespace org::kapa::tarracsh::tables {


class StringPool {

public:
    explicit StringPool(unsigned int poolSize)
        : _poolSize(poolSize) {
        _pool = static_cast<unsigned char *>(malloc(poolSize));
    }

    ~StringPool() {
        free(_pool);
    }

    uint64_t toOffset(std::string *stringPtr) {

        auto result = 0;
        //TODO
        return result;

    }

    std::string *toPtr(uint64_t offset) {

        //TODO
        return nullptr;

    }


    std::string *add(const std::string &stringValue) {
        if (_isSealed) return nullptr;
        if (stringValue.length() + _position >= _poolSize) {
            reallocPool();
        }



    }

    void read() {
        //TODO
    }

    void write() {
        //TODO
    }


private:
    unsigned char *_pool;
    unsigned int _position{0};

    std::unordered_map<std::string, unsigned int> _hash;

    bool _isSealed{false};
    unsigned int _poolSize;

    void seal() {
        _isSealed = true;
        //TODO seal the pool for translations string/offsets?
    }

    void reallocPool() {
        _poolSize = _poolSize * 2;
        auto newPool = static_cast<unsigned char *>(realloc(_pool, _poolSize));
        auto offset = newPool - _pool;

        //TODO reallocate 

    }

};

}

#pragma pack(pop)

#endif
