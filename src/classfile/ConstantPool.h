//
// Created by xman on 6/24/2022.
//

#ifndef TARRASH_CONSTANTPOOL_H
#define TARRASH_CONSTANTPOOL_H


#include <utility>
#include <cstring>


#include "ClassFileStructure.h"

namespace org { namespace kapa { namespace tarrash {

class ConstantPool {

private:
    u2 _count;
    vector <u1> _buffer;

    vector<ConstantPoolRecord *> _constantPoolIndex;

public:

    ConstantPool() {
        _buffer.reserve(64 * 1024);
    }

    void relocate() {
        u1 *baseAddress = reinterpret_cast<u1 *>(_buffer.data());
        for (auto &pConstantPoolRecord: _constantPoolIndex) {
            std::intptr_t offset = reinterpret_cast<std::intptr_t>(pConstantPoolRecord);
            pConstantPoolRecord = reinterpret_cast<ConstantPoolRecord *>( baseAddress + offset );
        }
    }

    ConstantPoolRecord& operator[] ( u2 index ) {
        auto result = getEntry( index );
        return *result;
    };

    ConstantPoolRecord* getEntry(int index) {
        assert(index - 1 < _constantPoolIndex.size());
        return _constantPoolIndex[index - 1];
    }

    void setCount(u2 count) { _count = count; }

    template<typename T>
    void add(T &data, int size) {

        auto reservedSpace = size;
        while (reservedSpace > 0) {
            _buffer.push_back(0);
            --reservedSpace;
        }
        auto start = &*(_buffer.end() - size);
        memcpy(start, &data, size);

    }


    template<typename T>
    void addRecord(T &data) {
        _constantPoolIndex.push_back(reinterpret_cast<ConstantPoolRecord *>(_buffer.size()));
        add(data, sizeof(data));
    }

    template<typename T>
    void addRecord(T &data, int size) {
        _constantPoolIndex.push_back(reinterpret_cast<ConstantPoolRecord *>(_buffer.size()));
        add(data, size);
    }

};

}}}
#endif //TARRASH_CONSTANTPOOL_H
