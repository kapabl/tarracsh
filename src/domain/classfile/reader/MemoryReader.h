#ifndef TARRACSH_FILE_READER_H
#define TARRACSH_FILE_READER_H

#include "ClassFileReader.h"
#include <cstddef>
#include <cstring>


namespace kapa::tarracsh::domain::classfile::reader {

class MemoryReader final : public ClassFileReader {

public:
    MemoryReader(const char *buffer, std::streamsize size);

    MemoryReader(const MemoryReader &) = delete;
    MemoryReader(const MemoryReader &&) = delete;
    MemoryReader &operator=(const MemoryReader &) = delete;
    MemoryReader &operator=(const MemoryReader &&) = delete;


    void readBytes(char *buffer, const unsigned int count) override;


private:
    const char *_buffer{nullptr};
    std::streamsize _position{0};

};


}
#endif
