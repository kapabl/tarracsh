#include <ranges>
#include <fmt/chrono.h>

#include "Table.h"

#include <BS_thread_pool.hpp>
#include <map>

#include "../../profiling/ScopedTimer.h"

using namespace kapa::infrastructure::db::table;
using kapa::infrastructure::profiler::ScopedTimer;


void Table::printSchema() {
    printLayout();
}

std::string Table::generateStdOutHeader() const {
    std::string result;
    forEachColumn([&result](const auto &properties, auto index) -> void {
        if (!result.empty()) {
            result += ",";
        }
        result += properties.name;
    });

    return result;
}

void Table::forEach(std::function<bool(AutoIncrementedRow *)> func) {
    auto rowId = 0u;
    while( rowId < _autoIncrementIndex.size()) {
        const auto pRow = _autoIncrementIndex[rowId];
        if (!pRow->isDeleted()) {
            if (!func(pRow)) {
                break;
            }
        }
        rowId++;
    }
}

void Table::forEach(std::function<void(AutoIncrementedRow *)> func) {
    auto rowId = 0u;
    while( rowId < _autoIncrementIndex.size()) {
        const auto pRow = _autoIncrementIndex[rowId];
        if (!pRow->isDeleted()) {
            func(pRow);
        }
        rowId++;
    }
}

std::string Table::generateStdOutRow(AutoIncrementedRow *row, bool displayRaw) const {
    std::string result;

    forEachColumn([&result, &row, &displayRaw, this](const auto &properties, auto index) -> void {
        char *pValue = reinterpret_cast<char *>(row) + properties.offsetInRow;
        if (!result.empty()) {
            result += ",";
        }
        result += properties.valueToString(pValue, _db, displayRaw);
    });

    return result;
}

void Table::list(const std::function<bool(AutoIncrementedRow &)> &filter, const bool displayRaw) {

    std::atomic<int> rowsFound = 0;
    auto rowsScanned = _autoIncrementIndex.size();
    profiler::MillisecondDuration duration{0};
    std::vector<std::vector<AutoIncrementedRow *>> outputByChuck;
    {
        ScopedTimer timer(&duration);

        const auto threadCount = std::thread::hardware_concurrency() * 4 / 5;
        BS::thread_pool threadPool{std::max<unsigned int>(1u, threadCount)};
        outputByChuck.resize(threadCount);

        const auto chunkSize = (rowsScanned + threadCount - 1) / threadCount;
        int chunkIndex = 0;

        do {

            threadPool.push_task(
                    [this, chunkSize, chunkIndex, &filter, &rowsFound, &outputByChuck]() -> void {
                        auto index = chunkIndex * chunkSize;
                        auto end = std::min<unsigned long long>(index + chunkSize, _autoIncrementIndex.size());
                        auto &outputById = outputByChuck[chunkIndex];
                        outputById.reserve(end - index);
                        while (index < end) {
                            const auto pRow = _autoIncrementIndex[index];
                            if (!pRow->isDeleted() && filter(*pRow)) {
                                outputById.push_back(pRow);
                                ++rowsFound;
                            }
                            index++;
                        }
                    });
            chunkIndex++;
        } while (chunkIndex < threadCount);

        threadPool.wait_for_tasks();
    }

    std::cout << fmt::format("table: {}", _layout.header.name) << std::endl;
    std::cout << generateStdOutHeader() << std::endl;

    for (auto &outputByDbId: outputByChuck) {
        for (const auto pRow: outputByDbId) {
            std::cout << generateStdOutRow(pRow, displayRaw) << "\n";
        }
    }

    std::cout << std::flush;

    profiler::MillisecondDuration dbReadDuration{_db.getReadTime()};
    std::cout <<
              fmt::format(
                      "rows found: {}, rows scanned: {}, rows processing time:{}, db read time: {}",
                      rowsFound.load(),
                      rowsScanned,
                      duration,
                      dbReadDuration) << std::endl;
}

uint64_t Table::size() const { return _rows.size(); }

std::string Table::getColumnValue(const uint64_t id, const char *columnName) {
    const auto row = _autoIncrementIndex[id];
    assert(_columnByName.contains(columnName));
    const auto &columnProperties = _columns[_columnByName[columnName]];
    char *pValue = reinterpret_cast<char *>(row) + columnProperties.offsetInRow;
    auto result = columnProperties.valueToString(pValue, _db, false);
    return result;
}

AutoIncrementedRow *Table::allocateRow() const {
    const auto result = (AutoIncrementedRow *) malloc(_rowSize);
    return result;
}

void Table::freeRow(AutoIncrementedRow *row) {
    free(row);
}

bool Table::isValidColumn(const std::string &columnName) const {
    const auto result = _columnByName.contains(columnName);
    return result;
}

auto Table::deleteRow(uint64_t id) -> bool {
    std::lock_guard lock(_mutex);
    auto row = _autoIncrementIndex[id];
    row->flags |= RowFlags::Deleted;
    _isDirty = true;
    if (!_dirtyRows.contains(row->id)) {
        _dirtyRows[row->id] = DirtyType::isUpdate;
    }
    return true;
}


void Table::printLayout() {
    std::cout << std::endl;
    std::cout << fmt::format("table name: {}", _layout.header.name) << std::endl;
    std::cout << fmt::format("column count: {}", _layout.header.columnCount) << std::endl;
    std::cout << fmt::format("row count: {}", calculateRowCount()) << std::endl;
    std::cout << std::endl;
    std::cout << "columns:" << std::endl;
    forEachColumn([](const auto &properties, auto index) -> void {
        std::cout << std::right
                  << std::setw(7) << fmt::format("no: {}, ", index)
                  << std::setw(25) << fmt::format("name: {}, ", properties.name)
                  << std::setw(25) << fmt::format("type: {}, ", column::StorageTypeToString(properties.type))
                  << std::setw(25) << fmt::format("display as: {}", column::displayAsToString(properties.displayAs))
                  << std::endl;
    });
}

void Table::writeRows(FILE *file) {
    if (std::fseek(file, static_cast<long>(getHeaderSize()), SEEK_SET) != 0) {
        _db.log().writeln(fmt::format("File seek error: {}", _filename), true);
    }

    uint64_t autoincrement = 0;
    auto rowsWritten = 0u;
    auto skippedRows = 0u;

    while (autoincrement < _autoIncrementIndex.size()) {

        const auto pRow = _autoIncrementIndex[autoincrement];
        assert(pRow->id == autoincrement);

        if (_dirtyRows.contains(autoincrement)) {
            if (skippedRows > 0) {
                const auto newPosition = getHeaderSize() + autoincrement * _rowSize;
                if (std::fseek(file, static_cast<long>(newPosition), SEEK_SET) != 0) {
                    _db.log().writeln(fmt::format("File seek error: {}", _filename), true);
                    break;
                }
                skippedRows = 0;
            }

            const auto bytesWritten = std::fwrite(pRow, 1, _rowSize, file);
            if (bytesWritten != _rowSize) {
                _db.log().writeln(fmt::format("File write error: {}", _filename), true);
                break;
            }
            rowsWritten++;
        } else {
            skippedRows++;
        }
        ++autoincrement;
    }

    _db.log().writeln(fmt::format("Rows written: {} - {} rows", _filename, rowsWritten), true);
    _dirtyRows.clear();
}

void Table::writeHeader(FILE *file) {
    _layout.header.columnCount = _columns.size();
    std::fwrite(&_layout.header, sizeof(TableLayout::Header), 1, file);
}

void Table::writeSchema(FILE *file) const {
    std::fwrite(&*_columns.begin(), sizeof(column::Properties), _columns.size(), file);
}

bool Table::readRows(FILE *file) {

    while (!std::feof(file)) {
        auto *row = allocateRow();
        const auto blocksRead = std::fread(row, _rowSize, 1, file);
        if (blocksRead == 0) {
            continue;
        }
        if (blocksRead != 1) {
            _db.log().writeln(fmt::format("Error reading table: {}", _filename), true);
            return false;
        }
        auto key = getKey(row);
        internalAdd(row, key);
    }
    return true;
}

bool Table::readHeader(FILE *file) {
    if (std::fread(&_layout.header, sizeof(TableLayout::Header), 1, file) != 1) {
        _db.log().writeln(fmt::format("Error reading table: {}", _filename), true);
        return false;
    }

    const auto result = _layout.header.signature == KAPA_TABLE_SIGNATURE;

    if (!result) {
        _db.log().writeln(fmt::format("Invalid table format or signature:{}", _filename), true);
    }

    return result;
}

bool Table::readSchema(FILE *file) {
    _columns.resize(_layout.header.columnCount);
    std::fread(&*_columns.begin(), sizeof(column::Properties), _columns.size(), file);

    forEachColumn([this](const auto &properties, const auto index) -> void {
        _columnByName[properties.name] = index;
    });
    return true;

}

void Table::internalAdd(AutoIncrementedRow *row, const std::string &key) {

    assert(!_rows.contains(key));

    row->id = _autoIncrementIndex.size();
    _rows[key] = row;
    _autoIncrementIndex.push_back(row);

    if (_autoIncrementIndex[0]->id != 0) {
        std::cout << "*stop here*";
    }

}

void Table::internalUpdate(AutoIncrementedRow *row, const std::string &key) {
    assert(_rows.contains(key));
    const auto destRow = _rows[key];
    if (destRow == row) {
        return;
    }

    row->id = destRow->id;
    //memcpy_s(destRow, _rowSize, row, _rowSize);
    memcpy(destRow, row, _rowSize);
    freeRow(row);
}

Table::Table(db::Database &db, const std::string &name, uint64_t rowSize)
        : _db(db),
          _rowSize(rowSize),
          _name(name),
          _stringPool(db.getStringPool()) {
    _layout.header.signature = KAPA_TABLE_SIGNATURE;
    //strcpy_s(_layout.header.name, name.c_str());
    strcpy(_layout.header.name, name.c_str());
    _filename = db.generateTableFilename(name);

}

Table::~Table() {
    for (const auto row: _rows | std::views::values) {
        freeRow(row);
    }
}

std::string Table::getName() const { return _name; }

void Table::init() {
    defineColumns();
}

void Table::forEachColumn(const std::function<void(const column::Properties &, const int index)> &func) const {
    auto index = 0u;
    while (index < _layout.header.columnCount) {
        func(_columns[index], index);
        index++;
    }
}

uint64_t Table::getFileSize() const {
    const auto result = std::filesystem::file_size(_filename);
    return result;
}

uint64_t Table::getHeaderSize() const {
    const auto result = sizeof(TableLayout) + (_layout.header.columnCount - 1) * sizeof(column::Properties);
    return result;
}

uint64_t Table::calculateRowCount() const {
    const auto result = (getFileSize() - getHeaderSize()) / _rowSize;
    return result;
}

AutoIncrementedRow *Table::get(std::string key) const {
    AutoIncrementedRow *result{nullptr};

    const auto &it = _rows.find(key);
    if (it != _rows.end()) {
        result = it->second;
    }
    return result;
}

void Table::update(AutoIncrementedRow *row) {
    std::lock_guard lock(_mutex);
    const auto key = getKey(row);
    internalUpdate(row, key);
    _isDirty = true;
    if (!_dirtyRows.contains(row->id)) {
        _dirtyRows[row->id] = DirtyType::isUpdate;
    }
}

uint64_t Table::addOrUpdate(AutoIncrementedRow *row) {

    std::lock_guard lock(_mutex);
    const auto key = getKey(row);

    if (_rows.contains(key)) {
        internalUpdate(row, key);
        if (!_dirtyRows.contains(row->id)) {
            _dirtyRows[row->id] = DirtyType::isUpdate;
        }
    } else {
        internalAdd(row, key);
        _dirtyRows[row->id] = DirtyType::isNew;
    }

    const auto result = row->id;

    _isDirty = true;
    return result;
}

const AutoIncrementedRow *Table::getRow(uint64_t id) {
    const auto result = _autoIncrementIndex[id];
    return result;
}

uint64_t Table::add(AutoIncrementedRow *row) {
    std::lock_guard lock(_mutex);
    const auto key = getKey(row);
    internalAdd(row, key);
    const auto result = row->id;

    _isDirty = true;
    _dirtyRows[row->id] = DirtyType::isNew;

    return result;
}

bool Table::write() {
    if (!_isDirty) return true;

    std::lock_guard lock(_mutex);

    const auto fileExists = std::filesystem::exists(_filename);
    const auto mode = fileExists ? "rb+" : "wb+";

    //FILE *file = nullptr;
    //if (fopen_s(&file, _filename.c_str(), mode) == 0) {
    FILE *file = fopen(_filename.c_str(), mode);
    if (file != nullptr) {
        if (std::fseek(file, 0, SEEK_SET) != 0) {
            _db.log().writeln(fmt::format("Error seeking file {}", _filename), true);
            return false;
        }
        if (!fileExists) {
            writeHeader(file);
            writeSchema(file);
        }
        writeRows(file);
        if (fclose(file) == -1) {
            _db.log().writeln(fmt::format("Error closing file {}", _filename), true);
            return false;
        }

        _isDirty = false;
    } else {
        _db.log().writeln(fmt::format("Error opening file {}", _filename), true);
        return false;
    }
    return true;
}

bool Table::clean() {
    std::lock_guard lock(_mutex);
    const auto stringPoolCleaned = _stringPool->clean();
    auto tableFileCleaned = true;
    if (std::filesystem::exists(_filename)) {
        tableFileCleaned = std::filesystem::remove(_filename);
        if (!tableFileCleaned) {
            _db.log().writeln(fmt::format("Error removing table file {}", _filename), true);
        }
    }

    const auto result = stringPoolCleaned && tableFileCleaned;

    return result;
}

void Table::backup() {
    filesystem::utils::backupPrevFile(_filename);
}

bool Table::read() {
    std::lock_guard lock(_mutex);

    if (!std::filesystem::exists(_filename)) return true;
    // FILE *file = nullptr;
    // auto result = fopen_s(&file, _filename.c_str(), "rb") == 0;
    auto *file = fopen(_filename.c_str(), "rb");
    auto result = file != nullptr;
    if (result) {
        result = readHeader(file) &&
                 readSchema(file) &&
                 readRows(file);
        if (fclose(file) == -1) {
            _db.log().writeln(fmt::format("Error closing file {}", _filename), true);
            return false;
        }
    } else {
        _db.log().writeln(fmt::format("Error opening file {}", _filename), true);
    }
    return result;
}

std::shared_ptr<kapa::infrastructure::db::StringPool> Table::getStringPool() const {
    return _stringPool;
}

AutoIncrementedRow *Table::findByKey(const std::string &key) {
    std::shared_lock lock(_mutex);
    AutoIncrementedRow *result{nullptr};
    const auto it = _rows.find(key);
    if (it != _rows.end()) {
        result = it->second;
    }
    return result;
}
