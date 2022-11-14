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

#include "../utils/DigestUtils.h"

#pragma pack( push, 1 )

namespace org::kapa::tarracsh::tables {


constexpr const char *TableExtension = ".kapamd";


struct AutoIncrementedRow {
    uint64_t id{-1ull};
};

struct ColumnRef {
    uint64_t id;
};


struct DigestColumn {
    unsigned char buf[DIGEST_LENGTH]{};
    bool operator==(const DigestColumn &right) const = default;

    bool operator==(const std::vector<unsigned char> &left) const {
        if (left.size() != DIGEST_LENGTH) return false;
        const auto result = memcmp(buf, &*left.begin(), DIGEST_LENGTH) == 0;
        return result;
    }

    DigestColumn &operator=(const std::vector<unsigned char> &left) {
        assert(left.size() == DIGEST_LENGTH);
        memcpy(buf, &*left.begin(), DIGEST_LENGTH);
        return *this;
    }
};


enum EntryType { Classfile, Jar, Directory };

template <typename T, typename K>
class Table {

public:
    virtual ~Table() = default;

    explicit Table(std::string filename, const std::shared_ptr<StringPool> stringPool)
        : _filename(std::move(filename)),
          _stringPool(stringPool) {
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

    uint64_t update(const T &row) {
        std::lock_guard lock(_mutex);
        const auto update = internalUpdate(row);
        _isDirty = true;
        return update;
    }

    uint64_t addOrUpdate(const T &row) {

        std::lock_guard lock(_mutex);
        auto key = getKey(row);
        const auto &it = _rows.find(key);

        const auto result =
            it != _rows.end()
                ? internalUpdate(row)
                : internalAdd(row);

        _isDirty = true;

        return result;
    }

    uint64_t getAutoincrement(T *row) {
        const auto result = _reverseAutoincrementIndex[row];
        return result;
    }

    const T *getRow(uint64_t id) {
        const auto result = _autoIncrementIndex[id];
        return result;
    }

    uint64_t add(const T &row) {
        std::lock_guard lock(_mutex);
        const auto result = internalAdd(row);

        _isDirty = true;

        return result;
    }

    virtual void updateIndexes(const T *pBeforeRow, const T *pAfterRow) {
    }

    virtual std::string getKey(const T &row) = 0;

    void write() {
        std::lock_guard lock(_mutex);
        fsUtils::backupPrevFile(_filename);

        std::ofstream file(_filename, std::ios::binary);
        file.unsetf(std::ios::skipws);

        // for (auto &[key, row] : _rows) {
        //     file.write(reinterpret_cast<char *>(&row), _rowSize);
        // }

        for (auto pRow : _autoIncrementIndex) {
            file.write(reinterpret_cast<const char *>(pRow), _rowSize);
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


    const T *findByKey(const K &key) {
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

    std::vector<const T *> _autoIncrementIndex;
    std::unordered_map<const T *, uint64_t> _reverseAutoincrementIndex;

    std::string _filename;
    bool _isDirty{false};
    unsigned int _rowSize = sizeof(T);
    std::shared_mutex _mutex;

    const std::shared_ptr<StringPool> _stringPool;


    uint64_t internalAdd(const T &row) {

        auto key = getKey(row);
        const auto &it = _rows.find(key);
        assert(!_rows.contains(key));

        T *pRow = &(_rows.insert({key, row}).first->second);
        const auto result = _autoIncrementIndex.size();
        pRow->id = result;
        _autoIncrementIndex.push_back(pRow);
        _reverseAutoincrementIndex[pRow] = result;

        return result;
    }

    uint64_t internalUpdate(const T &row) {

        auto key = getKey(row);
        assert(_rows.contains(key));
        _rows[key] = row;
        const auto result = _reverseAutoincrementIndex[&row];

        return result;
    }
};


}

#pragma pack(pop)

#endif
