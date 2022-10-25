#ifndef TARRACSH_FILE_READER_H
#define TARRACSH_FILE_READER_H

#include "ClassFileReader.h"
#include <filesystem>
#include <fstream>

namespace org::kapa::tarracsh::readers {

class FileReader final : public ClassFileReader {

public:
    explicit FileReader(const std::string &filename)
        : ClassFileReader() {
        _file.open(filename, std::ifstream::binary);
        _size = std::filesystem::file_size( filename);
        readHeader();
        
    }

    ~FileReader() override {
        _file.close();
    }

    FileReader(const FileReader &) = delete;
    FileReader(const FileReader &&) = delete;
    FileReader &operator=(const FileReader &) = delete;
    FileReader &operator=(const FileReader &&) = delete;


    void readBytes(char *buffer, unsigned int count) override {
        _file.read(buffer, count);
        _lastReadCount = _file.gcount();
    }


private:
    std::ifstream _file;


};


}
#endif
