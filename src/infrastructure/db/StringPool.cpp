#include "StringPool.h"

using namespace kapa::infrastructure::db;

StringPool::StringPool(std::string filename): _filename(std::move(filename)) {
    _pool = static_cast<char *>(malloc(StartPoolSize));

    add("");
}

std::string StringPool::getFilename() { return _filename; }

StringPool::~StringPool() {
    free(_pool);
}

tables::columns::StringCol StringPool::add(const std::string &stringValue) {
    const auto result = internalAdd(stringValue);
    return result;
}

bool StringPool::read() {

    if (!std::filesystem::exists(_filename)) return true;

    const auto fileSize = file_size(std::filesystem::path(_filename));

    if (fileSize > _size) {
        free(_pool);
        _size = fileSize;
        _pool = static_cast<char *>(malloc(_size));
    }

    std::ifstream file(_filename, std::ios::binary);
    file.unsetf(std::ios::skipws);
    _position = 0;

    while (!file.eof()) {
        file.read(&_pool[_position], ReadChunkSize);
        _position += file.gcount();
    }

    uint64_t currentPosition = 0;

    while (currentPosition < _position) {
        std::string_view value(&_pool[currentPosition]);
        _hash[value] = currentPosition;
        currentPosition += value.length() + 1;
    }
    _isDirty = false;
    return true;

}

bool StringPool::write() const {

    if (!_isDirty) return true;
               
    std::ofstream file(_filename, std::ios::binary);
    file.unsetf(std::ios::skipws);
    file.write(_pool, _position);

    return true;
}

const char * StringPool::getCString(const tables::columns::StringCol &item) {
    std::shared_lock readersLock(_sharedMutex);
    const auto result = &_pool[item];
    return result;
}

tables::columns::StringCol StringPool::internalAdd(const std::string &stringValue) {
    std::unique_lock writeLock(_sharedMutex);

    tables::columns::StringCol result;
    const auto it = _hash.find(stringValue);

    if (it == _hash.end()) {

        if (stringValue.length() + _position >= _size) {
            reallocPool();
        }
        result = _position;
        if (stringValue.length() > 0) {
            const char* begin = &*stringValue.begin();
            std::memcpy(&_pool[result], begin, stringValue.length());
        }

        _position += stringValue.length();
        _pool[_position] = 0;
        _position++;

        const std::string_view key(&_pool[result]);        
        _hash[key] = result;
        _isDirty = true;

    } else {
        result = it->second;
    }

    return result;

}

void StringPool::reallocPool() {
    _size = _size * 2;
    _pool = static_cast<char *>(realloc(_pool, _size));
}

bool StringPool::clean() {
    auto result = true;
    if (std::filesystem::exists(_filename)) {
        result = std::filesystem::remove(_filename);
        if (!result) {
            std::cout << std::format("Error removing string pool file: {}", _filename) << std::endl;
        }
    }
    return result;
}

void StringPool::backup() {
    filesystem::utils::backupPrevFile(_filename);
}
