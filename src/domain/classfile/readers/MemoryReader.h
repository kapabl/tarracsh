#ifndef TARRACSH_FILE_READER_H
#define TARRACSH_FILE_READER_H

#include "ClassFileReader.h"
#include <filesystem>
#include <fstream>
#include <libzippp/libzippp.h>
#include "../app/jars/JarEntry.h"


namespace kapa::tarracsh::readers {

class MemoryReader final : public ClassFileReader {

public:
    explicit MemoryReader(const jar::JarEntry &jarEntry)
        : ClassFileReader() {

        _size = static_cast<long long>(jarEntry.getSize());
        _buffer = jarEntry.getBuffer();
        readHeader();
    }

    MemoryReader(const MemoryReader &) = delete;
    MemoryReader(const MemoryReader &&) = delete;
    MemoryReader &operator=(const MemoryReader &) = delete;
    MemoryReader &operator=(const MemoryReader &&) = delete;


    void readBytes(char *buffer, const unsigned int count) override {

        memcpy(buffer, &_buffer[_position], count);
        //
        // for (int i = 0; i < count; i++) {
        //     buffer[i] = _buffer[_position];
        //     _position++;
        // }
        _position += count;
        _lastReadCount = count;
    }


private:
    char *_buffer{nullptr};
    std::streamsize _position{0};

};


}
#endif
