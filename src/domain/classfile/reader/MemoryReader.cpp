#include "MemoryReader.h"

#include <stdexcept>

using namespace kapa::tarracsh::domain::classfile::reader;

MemoryReader::MemoryReader(const char *buffer, const std::streamsize size) {
    if (buffer == nullptr || size <= 0) {
        throw std::invalid_argument("MemoryReader requires a valid buffer");
    }
    _buffer = buffer;
    _size = size;
    readHeader();
}

void MemoryReader::readBytes(char *buffer, const unsigned int count) {
    if (_position + static_cast<std::streamsize>(count) > _size) {
        throw std::runtime_error("Read beyond end of buffer");
    }
    memcpy(buffer, &_buffer[_position], count);
    _position += count;
    _lastReadCount = count;
}
