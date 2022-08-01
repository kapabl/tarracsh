#ifndef TARRACSH_TABLE_H
#define TARRACSH_TABLE_H
#include <optional>
#include <string>
#include <unordered_map>
#include <filesystem>
#include <fstream>

#include "StringPool.h"


#pragma pack( push, 1 )

namespace org::kapa::tarracsh::tables {

enum EntryType { Classfile, Jar, Directory };


struct ShaRow {


    EntryType type{Classfile};

    union {
        std::string *filenamePtr{nullptr};
        uint64_t filenamePoolOffset;
    };

    int64_t creationDatetime{};
    int64_t fileSize{};

    struct SHA {
        uint64_t _18b;
        uint64_t _28b;
        uint64_t _38b;
        uint64_t _48b;
    } sha256;


    ShaRow(StringPool& stringPool, std::string& filename) {
        filenamePtr = stringPool.add(filename);
    }

    [[nodiscard]] std::string* getKey() const { return filenamePtr; }
    

    void serialize(StringPool &stringPool, ShaRow &output) const {
        output = *this;
        output.filenamePoolOffset = stringPool.toOffset(filenamePtr);
    }

    void deserialize(StringPool &stringPool, const ShaRow &input) {
        const auto destination = const_cast<ShaRow *>(this);
        *destination = input;
        filenamePtr = stringPool.toPtr(input.filenamePoolOffset);
    }

    void deserialize(StringPool &stringPool) {
        filenamePtr = stringPool.toPtr(filenamePoolOffset);
    }
};

template <typename T, typename TKey>
class Table {

public:
    explicit Table(const std::string filename)
        : _filename(filename) {
        read();
    }

    std::optional<T> get(const TKey key) const {
        std::optional<T> result;
        const auto &it = _rows.find(key);
        if (it != _rows.end()) {
            result = *it;
        }
        return result;
    }

    bool add(const T &row) const {
        auto key = row.getKey();
        const auto &it = _rows.find(key);
        if (it == _rows.end()) {
            _rows[key] = row;
            return true;
        }
        return false;
    }

    bool update(const T &row) const {
        auto key = row.getKey();
        const auto &it = _rows.find(key);
        if (it != _rows.end()) {
            _rows[key] = row;
            return true;
        }
        return false;
    }


    void write() const {
        backupPrevFile();

        std::ofstream file(_filename, std::ios::binary);
        file.unsetf(std::ios::skipws);

        for (auto &[key, row] : _rows) {
            T outputRow;
            row.serialize(_stringPool, &outputRow);
            file.write(&outputRow, _rowSize);
        }
    }

    void read() {
        std::ifstream file(_filename, std::ios::binary);
        file.unsetf(std::ios::skipws);

        while (!file.eof()) {
            T rowBuffer;
            file.read(&rowBuffer, _rowSize);
            rowBuffer.deserialize( _stringPool );
            _rows[rowBuffer.getKey()] = rowBuffer;
        }
    }

    StringPool& getStringPool() { return _stringPool; }


private:
    std::unordered_map<TKey, T> _rows;
    std::string _filename;
    unsigned int _rowSize = sizeof(T);

    StringPool _stringPool;

    void backupPrevFile() const {
        const auto prevFilename = std::filesystem::path(_filename) / ".prev";
        std::filesystem::remove(prevFilename);
        std::filesystem::rename(
            std::filesystem::path(_filename),
            prevFilename);
    }
};


}

#pragma pack(pop)

#endif
