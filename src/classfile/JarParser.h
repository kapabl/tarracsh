
#ifndef JARPARSER_H
#define JARPARSER_H

#include <string>
namespace org::kapa::tarrash::jar {
class JarParser {
public:
    explicit JarParser(std::string fileName, std::string classPath);

    JarParser(const JarParser &) = delete;
    JarParser(const JarParser &&) = delete;
    JarParser &operator=(const JarParser &) = delete;
    JarParser &operator=(const JarParser &&) = delete;

    [[nodiscard]] bool isValid() const { return _isValid; }

    ~JarParser() = default;
    void run();

private:
    std::string _fileName;
    std::string _classPath;
    bool _isValid{true};
};
}
#endif
