#ifndef VETCTOR_READER_H
#define VETCTOR_READER_H

#include <vector>
#include <cassert>
#include "StructsCommon.h"


#include "../utils/StringUtils.h"

namespace org::kapa::tarracsh::readers {

using common::u1;
using common::u2;
using common::u4;

struct VectorReader {
    explicit VectorReader(const std::vector<u1> &buffer, const bool isBigEndianArg)
    : bytesVector(buffer), isBigEndian(isBigEndianArg) {}

    const std::vector<u1> &bytesVector;
    int position{0};
    bool isBigEndian;

    template <typename T> void readRaw(T &buffer, unsigned int byteCount) {

        assert(position + byteCount <= bytesVector.size());

        const auto charBuffer = reinterpret_cast<u1 *>(&buffer);

        for (auto i = 0u; i < byteCount; ++i) {
            charBuffer[i] = bytesVector[position];
            position++;
        }
    }

    template <typename T> void readRaw(T &buffer) { readRaw(buffer, sizeof(buffer)); }

    template <typename T> void read(T &buffer, unsigned int byteCount) {

        readRaw(buffer, byteCount);
        if (isBigEndian) {
            switch (byteCount) {
                case 2:
                    buffer = stringUtils::swapShort(buffer);
                    break;
                case 4:
                    buffer = stringUtils::swapLong(buffer);
                    break;
                default:
                    break;
            }
        }
    }

    template <typename T> void read(T &buffer) { read(buffer, sizeof(T)); }
    template <typename T> void readReversed(T &buffer) { readReversed(buffer, sizeof(T)); }

    u1 readU1() {
        u1 result;
        readRaw(result, 1);
        return result;
    }

    u2 readU2() {
        u2 result;
        readRaw(result, 2);
        if (!isBigEndian) {
            result = stringUtils::swapShort(result);
        }
        return result;
    }

    u4 readU4() {
        u4 result;
        readRaw(result, 4);
        if (!isBigEndian) {
            result = stringUtils::swapLong(result);
        }
        return result;
    }

    void jump(const int steps) {
        position += steps;
    }
};

}


#endif
