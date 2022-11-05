#ifndef TARRACSH_TABLE_H
#define TARRACSH_TABLE_H
#include <optional>
#include <shared_mutex>
#include <unordered_map>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>


#include "StringPool.h"


#pragma pack( push, 1 )

namespace org::kapa::tarracsh::tables {


constexpr const char *TableExtension = ".kapamd";


template <typename T, typename K>
class Table {

public:
    virtual ~Table() = default;

    explicit Table(const std::string &filename)
        : _filename(filename) {
        _stringPool = std::make_shared<StringPool>(filename + StringPoolExtension);
    }

    [[nodiscard]] bool isDirty() const { return _isDirty; }

    [[nodiscard]] std::optional<T> get(const K key) const {
        std::optional<T> result;
        const auto &it = _rows.find(key);
        if (it != _rows.end()) {
            result = *it;
        }
        return result;
    }

    bool update(const T &row) {
        std::lock_guard lock(_mutex);
        auto key = getKey(row);
        const auto &it = _rows.find(key);
        if (it != _rows.end()) {
            const T *pBeforeRow = &it->second;
            _rows[key] = row;
            _isDirty = true;
            //updateIndexes(pBeforeRow, &row);
            return true;
        }
        return false;
    }

    void addOrUpdate(const T &row) {
        std::lock_guard lock(_mutex);
        auto key = getKey(row);
        const auto &it = _rows.find(key);
        const T *pBeforeRow = nullptr;
        if (it != _rows.end()) {
            pBeforeRow = &it->second;
        }
        //updateIndexes(pBeforeRow, &row);
        _rows[key] = row;
        _isDirty = true;
    }

    bool add(const T &row) {
        std::lock_guard lock(_mutex);
        return internalAdd(row);
    }

    virtual void updateIndexes(const T *pBeforeRow, const T *pAfterRow) {
    }

    virtual std::string getKey(const T &row) = 0;


    void backupPrevFiles() const {
        backupPrevFile(_filename);
        const auto stringPoolFilename = _stringPool->getFilename();
        backupPrevFile(stringPoolFilename);
    }

    void write() {
        std::lock_guard lock(_mutex);
        backupPrevFiles();

        _stringPool->write();

        std::ofstream file(_filename, std::ios::binary);
        file.unsetf(std::ios::skipws);

        for (auto &[key, row] : _rows) {
            file.write(reinterpret_cast<char *>(&row), _rowSize);
        }
    }


    bool clean() {
        std::lock_guard lock(_mutex);
        const auto stringPoolCleaned = _stringPool->clean();
        auto tableFileCleaned = true;
        if (std::filesystem::exists(_filename)) {
            tableFileCleaned = std::filesystem::remove(_filename);
            if (!tableFileCleaned) {
                std::cout << std::format("Error removing table file {}", _filename) << std::endl;
            }
        }

        const auto result = stringPoolCleaned && tableFileCleaned;

        return result;
    }

    bool read() {
        std::lock_guard lock(_mutex);
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
            }

            if (bytesRead != _rowSize) {
                std::cout << std::format("Error reading table: {}", _filename) << std::endl;
                return false;
            }
            internalAdd(rowBuffer);
        }

        return true;
    }

    std::shared_ptr<StringPool> getStringPool() { return _stringPool; }

    [[nodiscard]] StringPoolItem getPoolString(const std::string &value) const {

        const auto result = _stringPool->add(value);
        return result;
    }

    [[nodiscard]] StringPoolItem getPoolString(const std::wstring &value) const {
        const auto result = _stringPool->add(value);
        return result;
    }


    T *findByKey(const K &key) {
        std::shared_lock lock(_mutex);
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
    std::shared_mutex _mutex;

    std::shared_ptr<StringPool> _stringPool;

    void backupPrevFile(const std::string &filename) const {
        if (!std::filesystem::exists(filename)) {
            return;
        }
        const auto prevFilename = std::filesystem::path(filename + ".prev");
        std::filesystem::remove(prevFilename);

        std::filesystem::rename(
            std::filesystem::path(filename),
            prevFilename);
    }

    bool internalAdd(const T& row) {
        auto key = getKey(row);
        const auto& it = _rows.find(key);
        if (it == _rows.end()) {
            _rows.insert({ key, row });
            //updateIndexes(nullptr, &row);
            _isDirty = true;
            return true;
        }
        return false;
    }
};


}

#pragma pack(pop)

#endif
