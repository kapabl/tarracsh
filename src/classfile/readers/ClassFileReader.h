#ifndef TARRACSH_CLASS_FILE_READER_H
#define TARRACSH_CLASS_FILE_READER_H

#include <iostream>

#include "../constpool/ConstpoolStructures.h"

namespace org::kapa::tarracsh::readers {

class ClassFileReader {

public:
    ClassFileReader() = default;

    ClassFileReader(const ClassFileReader &) = delete;
    ClassFileReader(const ClassFileReader &&) = delete;
    ClassFileReader &operator=(const ClassFileReader &) = delete;
    ClassFileReader &operator=(const ClassFileReader &&) = delete;


    virtual ~ClassFileReader() = default;

    virtual void readBytes(char *buffer, unsigned int count) = 0;

    u2 readU2();
    u2 readU2Reversed();
    u4 readU4();
    u1 readU1();

    [[nodiscard]] std::streamsize size() const { return _size; }
    [[nodiscard]] bool isValid() const { return _isValid; }
    [[nodiscard]] uint64_t isBigEndian() const { return _isBigEndian; }
    [[nodiscard]] uint64_t getLastReadCount() const { return _lastReadCount; }


    void readRaw(char* buffer, const unsigned int count) {

        if (_bytesRead + count > _size) {
            throw std::runtime_error("Error reading beyond size");
        }

        readBytes(buffer, count);

        _bytesRead += count;
    }

    template <typename T> void readRaw(T &buffer, const unsigned int count) {
        readRaw(reinterpret_cast<char*>(&buffer), count);
    }

    template <typename T = u2>
    void read(u2 &buffer) {
        readRaw(buffer, 2);
        if (!_isBigEndian) {
            buffer = stringUtils::swapShort(buffer);
        }
    }

    template <typename T = u2>
    void readReversed(u2 &buffer) {
        readRaw(buffer, 2);
        if (_isBigEndian) {
            buffer = stringUtils::swapShort(buffer);
        }
    }

    template <typename T = u4>
    void read(u4 &buffer) {
        readRaw(buffer, 4);
        if (!_isBigEndian) {
            buffer = stringUtils::swapLong(buffer);
        }
    }

    void readHeader() {
        readRaw(_header, sizeof(_header.magic));
        _isBigEndian = _header.magic == 0x0cafebabe;
        _isValid = _isBigEndian || _header.magic == stringUtils::swapLong(0x0cafebabe);
        if (_isValid) {
            _header.minorVersion = readU2();
            _header.majorVersion = readU2();
        } else {
            std::cout << "Invalid class file " << std::endl;
            _isValid = false;
        }

    }


protected:
    bool _isBigEndian{true};
    std::streamsize _size{};
    unsigned int _bytesRead{};
    std::streamsize _lastReadCount{};
    ClassFileHeader _header{};
    bool _isValid{};

    template <typename T> void readRaw(T &buffer) { readRaw(buffer, sizeof(buffer)); }


};


}
#endif
