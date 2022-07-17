
#ifndef JARPARSER_H
#define JARPARSER_H

#include <string>
namespace org::kapa::tarracsh::jar {
class JarAnalyzer {
public:
    explicit JarAnalyzer(std::string fileName, std::string classPath);

    JarAnalyzer(const JarAnalyzer &) = delete;
    JarAnalyzer(const JarAnalyzer &&) = delete;
    JarAnalyzer &operator=(const JarAnalyzer &) = delete;
    JarAnalyzer &operator=(const JarAnalyzer &&) = delete;

    [[nodiscard]] bool isValid() const { return _isValid; }

    ~JarAnalyzer() = default;
    void run();

private:
    std::string _fileName;
    std::string _classPath;
    bool _isValid{true};
};
}
#endif
