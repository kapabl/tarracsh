#ifndef TARRACSH_JAR_ENTRY_H
#define TARRACSH_JAR_ENTRY_H
#include <BS_thread_pool.hpp>
#include <chrono>
#include <unordered_set>
#include <libzippp/libzippp.h>
#include "../utils/StringUtils.h"

namespace org::kapa::tarracsh::jar {
class JarEntry {
public:
    const char *ClassFileExt = ".class";

    JarEntry(const libzippp::ZipEntry &entry, char* buffer)
        : _entry(entry), _buffer(buffer) {

        _isClassfile = _entry.isFile() && _entry.getName().ends_with(ClassFileExt);

        const auto modifiedTime = std::chrono::system_clock::from_time_t(entry.getDate());
        _lastWriteTime = std::chrono::duration_cast<std::chrono::microseconds>(modifiedTime.time_since_epoch()).count();
        _classname = stringUtils::pathToClassname(entry.getName());

    }

    ~JarEntry() {
        // ::delete [] _buffer;
        // _buffer = nullptr;
    }

    [[nodiscard]] const libzippp::ZipEntry &getZipEntry() const { return _entry; }
    [[nodiscard]] unsigned long long getSize() const { return _entry.getSize(); }
    [[nodiscard]] char *getBuffer() const { return _buffer; }
    [[nodiscard]] int getCRC() const { return _entry.getCRC(); }
    [[nodiscard]] std::string getName() const { return _entry.getName(); }
    [[nodiscard]] uint64_t getLastWriteTime() const { return _lastWriteTime; }

    [[nodiscard]] bool isClassfile() const { return _isClassfile; }
    [[nodiscard]] std::string getClassname() const { return _classname; }


private:
    const libzippp::ZipEntry &_entry;
    bool _isClassfile;
    std::string _classname;
    uint64_t _lastWriteTime{};
    char *_buffer{nullptr};

};
}
#endif
