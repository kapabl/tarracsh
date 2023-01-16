#ifndef TARRACSH_JAR_ENTRY_H
#define TARRACSH_JAR_ENTRY_H
#include <chrono>
#include <libzippp/libzippp.h>


namespace kapa::tarracsh::domain::jar {
class JarEntry {
public:
    const char *ClassFileExt = ".class";
    const int ExtensionLength = 6;


    JarEntry(const libzippp::ZipEntry &entry, char* buffer)
        : _entry(entry), _buffer(buffer) {

        const auto name = _entry.getName();
        _isClassfile = _entry.isFile() && name.ends_with(ClassFileExt);

        const auto modifiedTime = std::chrono::system_clock::from_time_t(entry.getDate());
        _lastWriteTime = std::chrono::duration_cast<std::chrono::microseconds>(modifiedTime.time_since_epoch()).count();

        if (_isClassfile) {
            // _jvmClassname = stringUtils::pathToClassname(name.substr( 0, name.length() - ExtensionLength ));
            _jvmClassname = name.substr( 0, name.length() - ExtensionLength );
        }

        _isMultiRelease = name.find("META-INF/versions/") == 0;


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
    [[nodiscard]] std::string getClassname() const { return _jvmClassname; }
    [[nodiscard]] bool isMultiReleaseEntry() const {
        return _isMultiRelease;
    }



private:
    libzippp::ZipEntry _entry;
    bool _isClassfile;
    std::string _jvmClassname;
    uint64_t _lastWriteTime{};
    char *_buffer{nullptr};
    bool _isMultiRelease{ false };

};
}
#endif
