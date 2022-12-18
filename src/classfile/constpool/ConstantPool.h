#ifndef TARRACSH_CONSTANT_POOL_H
#define TARRACSH_CONSTANT_POOL_H

#include <unicode/unistr.h>
#include <cstring>
#include "ConstantPool.h"
#include "../readers/ClassFileReader.h"

#include "ConstpoolStructures.h"

namespace org::kapa::tarracsh {

class ConstantPool {
public:
    ConstantPool();
    ~ConstantPool();
    void relocate();
    ConstantPoolRecord &operator[](const u2 index) const;
    void setCount(const u2 count) { _count = count; }
    [[nodiscard]] u2 getCount() const { return _count; }
    [[nodiscard]] int getPoolSize() const { return _constantPoolIndex.size(); }

    template <typename T>
    void add(T &data, const int size) {
        reserve(size);
        memcpy(&_buffer[_position], &data, size);
        _position += size;

    }
    void reserve(const int size) {
        if (size + _position >= _size) {
            _size <<= 1;
            _buffer = static_cast<u1 *>(realloc(_buffer, _size));
        }
    }

    template <typename T>
    void addRecord(T &data) {
        addRecord(data, sizeof(data));
    }

    template <typename T>
    void addRecord(T &data, int size) {
        _constantPoolIndex.push_back(
            reinterpret_cast<ConstantPoolRecord *>(_position) // NOLINT(performance-no-int-to-ptr)
            );
        add(data, size);
    }

    [[nodiscard]] std::string getClassInfoName(const u2 classInfoIndex) const;
    [[nodiscard]] std::string getClassname(const u2 nameIndex) const;
    [[nodiscard]] std::string getString(const u2 stringIndex, const bool withEscaped = false) const;
    [[nodiscard]] std::string getTypeString(const u2 stringIndex) const;
    [[nodiscard]] std::string getConstantValueString(const u2 constantIndex) const;

    void addEmptyIndex();
    void addUtf8Record(u2 length, readers::ClassFileReader & reader);

private:
    [[maybe_unused]] u2 _count{};
    // std::vector<u1> _buffer;
    u1 *_buffer;
    uint64_t _size{1024 * 1024};
    uint64_t _position{0};

    std::vector<ConstantPoolRecord *> _constantPoolIndex;

    [[nodiscard]] ConstantPoolRecord &getEntry(const u2 index) const;

};


}
#endif //TARRACSH_CONSTANT_POOL_H
