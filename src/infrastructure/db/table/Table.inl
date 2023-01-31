#ifndef KAPA_TABLE_INL
#define KAPA_TABLE_INL

#include "Table.h"

namespace kapa::infrastructure::db::tables {


template <typename T>
void Table<T>::printSchema() {
    printLayout();
}

template <typename T> std::string Table<T>::generateStdOutHeader() const {
    std::string result;
    for(const auto& columnProperties: _columns ) {
        if (!result.empty()) {
            result += ",";
        }
        result += columnProperties.name;
    }
    return result;
}


template <typename T> std::string Table<T>::generateStdOutRow(T* pRow) {
    std::string result;

    for(auto& columnProperties: _columns) {
        char* pValue = reinterpret_cast<char*>(pRow) + columnProperties.offsetInRow;
        if ( !result.empty()) {
            result += ",";
        }
        result += columnProperties.valueToString(pValue, _db);
    }

    return result;
}

template <typename T> void Table<T>::list() {
    std::cout << std::format("table: {}", _layout.header.name) << std::endl;
    std::cout << generateStdOutHeader() << std::endl;
    for( auto pRow: _autoIncrementIndex ) {
        std::cout << generateStdOutRow( pRow ) << std::endl;
    }
}


template <typename T>
void Table<T>::printLayout() {
    std::cout << std::endl;
    std::cout << std::format("table name: {}", _layout.header.name) << std::endl;
    std::cout << std::format("column count: {}", _layout.header.columnCount) << std::endl;
    std::cout << std::format("row count: {}", calculateRowCount()) << std::endl;
    std::cout << std::endl;
    std::cout << "columns:" << std::endl;
    auto index = 1u;
    for (auto &column : _columns) {
        std::cout << std::right
            << std::setw(7) << std::format("no: {}, ", index)
            << std::setw(25) << std::format("name: {}, ", column.name)
            << std::setw(25) << std::format("type: {}, ", StorageTypeToString(column.type))
            << std::setw(25) << std::format("display as: {}", displayAsToString(column.displayAs))
            << std::endl;
        index++;
    }
}

template <typename T> void Table<T>::writeRows(FILE *file) {
    if (std::fseek(file, getHeaderSize(), SEEK_SET) != 0) {
        _db.log().writeln(std::format("File seek error: {}", _filename), true);
    }

    uint64_t autoincrement = 0;
    auto rowsWritten = 0u;
    auto skippedRows = 0u;
    while (autoincrement < _autoIncrementIndex.size()) {

        const auto pRow = _autoIncrementIndex[autoincrement];
        assert(pRow->id == autoincrement);

        if (_dirtyRows.contains(autoincrement)) {
            if ( skippedRows > 0 ) {
                const auto newPosition = getHeaderSize() + autoincrement * RowSize;
                if (std::fseek(file, newPosition, SEEK_SET) != 0) {
                    _db.log().writeln(std::format("File seek error: {}", _filename), true);
                    break;
                }
#ifdef _DEBUG
                if (std::ftell(file) != newPosition) {
                    _db.log().writeln(std::format("File seek failed: {}, expected position: {}, got: {}",
                        _filename, newPosition, std::ftell(file)), true);
                }
#endif
                skippedRows = 0;
            }
#ifdef _DEBUG
            if ( _dirtyRows[ autoincrement] == DirtyType::isUpdate ) {
                T row;
                const auto readBytes = std::fread(&row, 1, RowSize, file);
                assert(readBytes == RowSize);
                assert(row.id == autoincrement);
                std::fseek(file, -RowSize, SEEK_CUR);
            }
            
#endif
            const auto bytesWritten = std::fwrite(pRow, 1, RowSize, file);
            if ( bytesWritten != RowSize ) {
                _db.log().writeln(std::format("File write error: {}", _filename), true);
                break;
            }
            rowsWritten++;
        } else {
            skippedRows++;
        }
        ++autoincrement;
    }

    _db.log().writeln(std::format("Rows written: {} - {} rows", _filename, rowsWritten), true);
    _dirtyRows.clear();
}

template <typename T> void Table<T>::writeHeader(FILE *file) {
    _layout.header.columnCount = _columns.size();
    std::fwrite(&_layout.header, sizeof(TableLayout::Header), 1, file);
}

template <typename T> void Table<T>::writeSchema(FILE *file) const {
    const auto buffer = reinterpret_cast<const char *>(&*_columns.begin());
    std::fwrite(buffer, sizeof(columns::Properties), _columns.size(), file);
}

template <typename T> bool Table<T>::readRows(FILE *file) {
    while (!std::feof(file)) {
        T row;
        const auto bytesRead = std::fread(reinterpret_cast<char*>(&row), 1, RowSize, file);
        if ( bytesRead == 0 ) {
            continue;
        }
        if (bytesRead != RowSize ) {
            _db.log().writeln(std::format("Error reading table: {}", _filename), true);
            return false;
        }
        auto key = getKey(row);
        internalAdd(row, key);
    }
    return true;
}

template <typename T> bool Table<T>::readHeader(FILE *file) {
    if (std::fread(&_layout.header, sizeof(TableLayout::Header), 1, file) != 1) {
        _db.log().writeln(std::format("Error reading table: {}", _filename), true);
        return false;
    }

    const auto result = _layout.header.signature == KAPA_TABLE_SIGNATURE;

    if (!result) {
        _db.log().writeln(std::format("Invalid table format or signature:{}", _filename), true);
    }

    return result;
}

template <typename T> bool Table<T>::readSchema(FILE *file) {
    auto const bytesToRead = _layout.header.columnCount * sizeof(columns::Properties);
    _columns.resize(_layout.header.columnCount);
    if (std::fread(&*_columns.begin(), bytesToRead, 1, file) != 1) {
        _db.log().writeln(std::format("Error reading table: {}", _filename), true);
        return false;
    }

    return true;

}

template <typename T> void Table<T>::internalAdd(T &row, const std::string &key) {

    const auto &it = _rows.find(key);
    assert(!_rows.contains(key));

    T *pRow = &(_rows.insert({key, row}).first->second);
    const auto id = _autoIncrementIndex.size();
    pRow->id = id;
    row.id = id;
    _autoIncrementIndex.push_back(pRow);
    _reverseAutoincrementIndex[pRow] = id;

}

template <typename T> void Table<T>::internalUpdate(T &row, const std::string &key) {
    assert(_rows.contains(key));
    const auto id = _rows[key].id;
    row.id = id;
    _rows[key] = row;
}

template <typename T> Table<T>::Table(db::Database &db, const std::string &tablename):
    _db(db),
    _stringPool(db.getStringPool()) {
    _layout.header.signature = KAPA_TABLE_SIGNATURE;
    strcpy_s(_layout.header.name, tablename.c_str());
    _filename = db.generateTableFilename(tablename);

}

template <typename T> void Table<T>::init() {
    defineColumns();
}

template <typename T> uint64_t Table<T>::getFileSize() const {
    const auto result = std::filesystem::file_size(_filename);
    return result;
}

template <typename T> uint64_t Table<T>::getHeaderSize() const {
    const auto result = sizeof(TableLayout) + (_layout.header.columnCount - 1) * sizeof(columns::Properties);
    return result;
}

template <typename T> uint64_t Table<T>::calculateRowCount() const {
    const auto result = (getFileSize() - getHeaderSize()) / RowSize;
    return result;
}

template <typename T> std::optional<T> Table<T>::get(std::string key) const {
    std::optional<T> result;
    const auto &it = _rows.find(key);
    if (it != _rows.end()) {
        result = *it;
    }
    return result;
}

template <typename T> void Table<T>::update(T &row) {
    std::lock_guard lock(_mutex);
    auto key = getKey(row);
    internalUpdate(row, key);
    _isDirty = true;
    if (!_dirtyRows.contains(row.id)) {
        _dirtyRows[row.id] = DirtyType::isUpdate;
    }
}

template <typename T> uint64_t Table<T>::addOrUpdate(T &row) {

    std::lock_guard lock(_mutex);
    auto key = getKey(row);

    if (_rows.contains(key)) {
        internalUpdate(row, key);
        if (!_dirtyRows.contains(row.id)) {
            _dirtyRows[row.id] = DirtyType::isUpdate;
        }
    } else {
        internalAdd(row, key);
        _dirtyRows[row.id] = DirtyType::isNew;
    }

    const auto result = row.id;

    _isDirty = true;
    return result;
}

template <typename T> const T * Table<T>::getRow(uint64_t id) {
    const auto result = _autoIncrementIndex[id];
    return result;
}

template <typename T> uint64_t Table<T>::add(T &row) {
    std::lock_guard lock(_mutex);
    auto key = getKey(row);
    internalAdd(row, key);
    const auto result = row.id;

    _isDirty = true;
    _dirtyRows[row.id] = DirtyType::isNew;

    return result;
}

template <typename T> bool Table<T>::write() {
    if (!_isDirty) return true;

    std::lock_guard lock(_mutex);

    const auto fileExists = std::filesystem::exists(_filename);
    const auto mode = fileExists ? "rb+" : "wb+";

    FILE *file = nullptr;
    if (fopen_s(&file, _filename.c_str(), mode) == 0) {
        if ( std::fseek(file, 0, SEEK_SET) != 0 ) {
            _db.log().writeln(std::format("Error seeking file {}", _filename), true);
            return false;
        }
        if (!fileExists) {
            writeHeader(file);
            writeSchema(file);
        }
        writeRows(file);
        if ( fclose(file) == -1 ) {
            _db.log().writeln(std::format("Error closing file {}", _filename), true);
            return false;
        }

        _isDirty = false;
    } else {
        _db.log().writeln(std::format("Error opening file {}", _filename), true);
        return false;
    }
    return true;
}

template <typename T> bool Table<T>::clean() {
    std::lock_guard lock(_mutex);
    const auto stringPoolCleaned = _stringPool->clean();
    auto tableFileCleaned = true;
    if (std::filesystem::exists(_filename)) {
        tableFileCleaned = std::filesystem::remove(_filename);
        if (!tableFileCleaned) {
            _db.log().writeln(std::format("Error removing table file {}", _filename), true);
        }
    }

    const auto result = stringPoolCleaned && tableFileCleaned;

    return result;
}

template <typename T> void Table<T>::backup() {
    filesystem::utils::backupPrevFile(_filename);
}

template <typename T> bool Table<T>::read() {
    std::lock_guard lock(_mutex);

    if (!std::filesystem::exists(_filename)) return true;
    FILE *file = nullptr;
    auto result = fopen_s(&file, _filename.c_str(), "rb") == 0;
    if (result) {
        result = readHeader(file) &&
                 readSchema(file) &&
                 readRows(file);
        if (fclose(file) == -1) {
            _db.log().writeln(std::format("Error closing file {}", _filename), true);
            return false;
        }
    } else {
        _db.log().writeln(std::format("Error opening file {}", _filename), true);
    }
    return result;
}

template <typename T> std::shared_ptr<StringPool> Table<T>::getStringPool() const {
    return _stringPool;
}

template <typename T> const T *Table<T>::findByKey(const std::string &key) {
    std::shared_lock lock(_mutex);
    T *result{};
    const auto it = _rows.find(key);
    if (it != _rows.end()) {
        result = &it->second;
    }
    return result;
}
}
#endif