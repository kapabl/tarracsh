//
// Created by xman on 6/24/2022.
//

#ifndef TARRASH_CONSTANTPOOL_H
#define TARRASH_CONSTANTPOOL_H


#include <utility>
#include <cstring>


#include "ClassFileStructure.h"

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

    int getCount() { return _count; }

    void setCount(u2 count) { _count = count; }

    template<typename T>
    void add(T &data, int size) {

        auto reservedSpace = size;
        while( reservedSpace > 0 ) {
            _buffer.push_back( 0 );
            --reservedSpace;
        }
        auto start = &*(_buffer.end() - size);
        memcpy( start, &data, size );

    }


    template<typename T>
    void addRecord(T &data) {
        _constantPoolIndex.push_back(reinterpret_cast<ConstantPoolRecord *>(_buffer.size()));
        add(data, sizeof(data));
    }

    template<typename T>
    void addRecord(T &data, int size ) {
        _constantPoolIndex.push_back(reinterpret_cast<ConstantPoolRecord *>(_buffer.size()));
        add(data, size);
    }


    template<typename T>
    void add2(T &data) {
        u1 *byte = reinterpret_cast<u1 *>( &data );
        _buffer.insert(_buffer.end(), byte, byte + 2);
//        _buffer.push_back(byte[1]);
//        _buffer.push_back(byte[0]);
    }

    template<typename T>
    void add4(T &data) {
        u1 *byte = reinterpret_cast<u1 *>( &data );
        _buffer.insert(_buffer.end(), byte, byte + 4);
//        _buffer.push_back(byte[0]);
//        _buffer.push_back(byte[1]);
//        _buffer.push_back(byte[2]);
//        _buffer.push_back(byte[3]);
    }

    template<typename T>
    void addN(T &data, int count) {
        u1 *byte = reinterpret_cast<u1 *>( &data );
        while (count > 0) {
            _buffer.push_back(byte[0]);
            byte++;
            count--;
        }
    }


};


#endif //TARRASH_CONSTANTPOOL_H
