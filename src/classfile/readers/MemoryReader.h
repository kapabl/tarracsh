#ifndef TARRACSH_FILE_READER_H
#define TARRACSH_FILE_READER_H

#include "ClassFileReader.h"
#include <filesystem>
#include <fstream>
#include <libzippp/libzippp.h>

namespace org::kapa::tarracsh::readers {

class MemoryReader final : public ClassFileReader {

public:
    explicit MemoryReader(const libzippp::ZipEntry &entry)
        : ClassFileReader() {

        _size = static_cast<long long>(entry.getSize());
        _buffer = static_cast<char *>(entry.readAsBinary());
        readHeader();

    }

    ~MemoryReader() override {
        delete[] _buffer;
    }

    MemoryReader(const MemoryReader &) = delete;
    MemoryReader(const MemoryReader &&) = delete;
    MemoryReader &operator=(const MemoryReader &) = delete;
    MemoryReader &operator=(const MemoryReader &&) = delete;


    void readBytes(char *buffer, const unsigned int count) override {
        for (int i = 0; i < count; i++) {
            buffer[i] = _buffer[_position];
            _position++;
        }
        _lastReadCount = count;
    }


private:
    char *_buffer{nullptr};
    std::streamsize _position{0};

};


}
#endif
