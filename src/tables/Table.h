#ifndef TARRACSH_TABLE_H
#define TARRACSH_TABLE_H
#include <optional>
#include <shared_mutex>
#include <unordered_map>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include "../app/App.h"
#include "Database.h"
#include "StringPool.h"

#include "../utils/DigestUtils.h"
#include "columns/Columns.h"

#pragma pack( push, 1 )

namespace org::kapa::tarracsh::db::tables {


constexpr const char *TableExtension = ".kapamd";


struct Row {
};

struct AutoIncrementedRow : Row {
    columns::UInt64Col id{-1ull};
};

#define KAPA_TABLE_SIGNATURE 0xca5ab1a9ca
#define MAX_TABLE_NAME 128

struct TableLayout {

    struct Header {
        uint64_t signature{KAPA_TABLE_SIGNATURE};
        char name[MAX_TABLE_NAME];
        int columnCount{0};

        Header() {
            memset(&name, 0, MAX_TABLE_NAME);
        }
    } header;

    columns::Properties columns[1];

    TableLayout()
        : columns{} {

    }

};


#define DECLARE_COLUMN_PROP( name, storage, displayAs ) \
    const Properties name##Prop( #name, storage, displayAs ); \
    _columns.push_back( name##Prop ); \
    _layout.header.columnCount = _columns.size()
#define DECLARE_ROW_VALUE( dataType, columnName ) columns::dataType##Col columnName{};
#define START_AUTOINCREMENT_ROW( tableName ) struct tableName##Row : AutoIncrementedRow {

#define END_ROW };

template <typename T>
class Table {

public:
    explicit Table(db::Database &db, const std::string &tablename)
        : _db(db),
          _stringPool(db.getStringPool()) {
        _layout.header.signature = KAPA_TABLE_SIGNATURE;
        strcpy_s(_layout.header.name, tablename.c_str());
        _filename = db.generateTableFilename(tablename);

    }

    void init() {
        defineColumns();
    }

    virtual ~Table() = default;

    [[nodiscard]] bool isDirty() const { return _isDirty; }

    [[nodiscard]] std::optional<T> get(std::string key) const {
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

    bool write() {
        if (!_isDirty) return true;

        std::lock_guard lock(_mutex);
        fsUtils::backupPrevFile(_filename);

        std::ofstream file(_filename, std::ios::binary);
        file.unsetf(std::ios::skipws);

        writeHeader(file);
        writeSchema(file);
        writeRows(file);
        return true;
    }


    bool clean();

    bool read() {
        std::lock_guard lock(_mutex);

        if (!std::filesystem::exists(_filename)) return true;

        std::ifstream file(_filename, std::ios::binary);
        file.unsetf(std::ios::skipws);

        const auto result = readHeader(file) &&
                            readSchema(file) &&
                            readRows(file);
        return result;
    }

    std::shared_ptr<StringPool> getStringPool() const;

    const T *findByKey(const std::string &key) {
        std::shared_lock lock(_mutex);
        T *result{};
        const auto it = _rows.find(key);
        if (it != _rows.end()) {
            result = &it->second;
        }
        return result;
    }


    void printSchema() {
        printLayout();
    }

    uint64_t size() { return _rows.size(); }


protected:
    db::Database &_db;
    std::unordered_map<std::string, T> _rows;

    std::vector<const T *> _autoIncrementIndex;
    std::unordered_map<const T *, uint64_t> _reverseAutoincrementIndex;

    std::string _filename;
    bool _isDirty{false};
    unsigned int _rowSize = sizeof(T);
    std::shared_mutex _mutex;

    const std::shared_ptr<StringPool> _stringPool;


    std::vector<columns::Properties> _columns;
    TableLayout _layout;

    virtual void defineColumns() {
    }


    void printLayout() {
        std::cout << std::endl;
        std::cout << std::format("table name: {}", _layout.header.name) << std::endl;
        std::cout << std::format("column count: {}", _layout.header.columnCount) << std::endl;
        std::cout << std::endl;
        std::cout << "columns:" << std::endl;
        auto index = 1u;
        for (auto &column : _columns) {
            std::cout << std::format("  no: {}", index) << std::endl;
            std::cout << std::format("  name: {}", column.name) << std::endl;
            std::cout << std::format("  type: {}", StorageTypeToString(column.type)) << std::endl;
            std::cout << std::format("  display as: {}", DisplayAsToString(column.displayAs)) << std::endl;
            std::cout << std::endl;
            index++;
        }

    }

    void writeRows(std::ofstream &file) {
        for (auto pRow : _autoIncrementIndex) {
            file.write(reinterpret_cast<const char *>(pRow), _rowSize);
        }
    }

    void writeHeader(std::ofstream &file) {
        _layout.header.columnCount = _columns.size();
        file.write(reinterpret_cast<const char *>(&_layout.header), sizeof(TableLayout::Header));
    }

    void writeSchema(std::ofstream &file) const {
        const auto buffer = reinterpret_cast<const char *>(&*_columns.begin());
        file.write(buffer, _columns.size() * sizeof(columns::Properties));
    }

    bool readRows(std::ifstream &file) {
        while (!file.eof()) {
            T rowBuffer;
            file.read(reinterpret_cast<char *>(&rowBuffer), _rowSize);
            const auto bytesRead = file.gcount();
            if (bytesRead == 0) {
                continue;
            }

            if (bytesRead != _rowSize) {
                app::App::logln(std::format("Error reading table: {}", _filename), true);
                return false;
            }
            internalAdd(rowBuffer);
        }
        return true;
    }

    bool readHeader(std::ifstream &file) {
        file.read(reinterpret_cast<char *>(&_layout.header), sizeof(TableLayout::Header));

        const auto result = file.gcount() == sizeof(TableLayout::Header)
                            && _layout.header.signature == KAPA_TABLE_SIGNATURE;

        if (!result) {
            app::App::logln(std::format("Invalid table format or signature:{}", _filename), true);
        }

        return result;
    }

    bool readSchema(std::ifstream &file) {
        auto const bytesToRead = _layout.header.columnCount * sizeof(columns::Properties);
        _columns.resize(_layout.header.columnCount);
        file.read(reinterpret_cast<char *>(&*_columns.begin()), bytesToRead);
        const auto result = bytesToRead == file.gcount();

        if (!result) {
            app::App::logln(std::format("Invalid table schema:{}", _filename), true);
        }

        return result;

    }


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

template <typename T> bool Table<T>::clean() {
    std::lock_guard lock(_mutex);
    const auto stringPoolCleaned = _stringPool->clean();
    auto tableFileCleaned = true;
    if (std::filesystem::exists(_filename)) {
        tableFileCleaned = std::filesystem::remove(_filename);
        if (!tableFileCleaned) {
            app::App::logln(std::format("Error removing table file {}", _filename), true);
        }
    }

    const auto result = stringPoolCleaned && tableFileCleaned;

    return result;
}

template <typename T> std::shared_ptr<StringPool> Table<T>::getStringPool() const {
    return _stringPool;
}


}

#pragma pack(pop)

#endif
