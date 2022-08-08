#ifndef TARRACSH_TABLE_H
#define TARRACSH_TABLE_H
#include <optional>
#include <string>
#include <unordered_map>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>


#include "StringPool.h"


#pragma pack( push, 1 )

namespace org::kapa::tarracsh::tables {

union PoolStringItem {
    char *ptr{nullptr};
    uint64_t offset;
};

constexpr const char *TableExtension = ".kapamd";


template <typename T, typename K>
class Table {

public:
    explicit Table(const std::string &filename)
        : _filename(filename) {
        _stringPool = std::make_shared<StringPool>(filename + StringPoolExtension);
    }

    [[nodiscard]] bool isDirty() const { return _isDirty; }

    std::optional<T> get(const K key) const {
        std::optional<T> result;
        const auto &it = _rows.find(key);
        if (it != _rows.end()) {
            result = *it;
        }
        return result;
    }

    bool update(const T &row) {
        auto key = row.getKey();
        const auto &it = _rows.find(key);
        if (it != _rows.end()) {
            _rows[key] = row;
            _isDirty = true;
            return true;
        }
        return false;
    }

    void addOrUpdate(const T& row) {
        auto key = row.getKey();
        _rows[key] = row;
        _isDirty = true;
    }

    bool add(const T &row) {
        auto key = row.getKey();
        const auto &it = _rows.find(key);
        if (it == _rows.end()) {
            _rows.insert({key, row});
            _isDirty = true;
            return true;
        }
        return false;
    }


    void write() {
        if (std::filesystem::exists(_filename)) {
            backupPrevFile();
        }

        _stringPool->write();

        std::ofstream file(_filename, std::ios::binary);
        file.unsetf(std::ios::skipws);

        for (auto &[key, row] : _rows) {
            T outputRow;
            row.serialize(_stringPool, outputRow);
            file.write(reinterpret_cast<char *>(&outputRow), _rowSize);
        }
    }

    bool read() {

        if (!_stringPool->read()) return false;

        if (!std::filesystem::exists(_filename)) return true;

        std::ifstream file(_filename, std::ios::binary);
        file.unsetf(std::ios::skipws);

        while (!file.eof()) {
            T rowBuffer;
            file.read(reinterpret_cast<char *>(&rowBuffer), _rowSize);
            const auto bytesRead = file.gcount();
            if (bytesRead == 0) {
                continue;
            };

            if (bytesRead != _rowSize) {
                std::cout << std::format("Error reading table: {}", _filename) << std::endl;
                return false;
            }
            rowBuffer.deserialize(_stringPool);

            const auto &key = rowBuffer.getKey();
            _rows[key] = rowBuffer;
        }

        return true;
    }

    std::shared_ptr<StringPool> getStringPool() { return _stringPool; }

    [[nodiscard]] char *getPoolString(const std::string &value) const {
        const auto result = _stringPool->add(value);
        return result;
    }

    [[nodiscard]] char *getPoolString(const std::wstring &value) const {
        const auto result = _stringPool->add(value);
        return result;
    }


    T *findByKey(const K &key) {
        T *result{};
        const auto it = _rows.find(key);
        if (it != _rows.end()) {
            result = &it->second;
        }
        return result;
    }


protected:
    std::unordered_map<K, T> _rows;
    std::string _filename;
    bool _isDirty{false};
    unsigned int _rowSize = sizeof(T);

    std::shared_ptr<StringPool> _stringPool;

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
