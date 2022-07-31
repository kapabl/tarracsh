
#ifndef JARPARSER_H
#define JARPARSER_H

#include <string>

#include "Tarracsh.h"


namespace org::kapa::tarracsh::jar {
class JarAnalyzer {
public:
    explicit JarAnalyzer(Options options);

    JarAnalyzer(const JarAnalyzer &) = delete;
    JarAnalyzer(const JarAnalyzer &&) = delete;
    JarAnalyzer &operator=(const JarAnalyzer &) = delete;
    JarAnalyzer &operator=(const JarAnalyzer &&) = delete;

    [[nodiscard]] bool isValid() const { return _isValid; }

    ~JarAnalyzer() = default;
    void run();
    void output();
    unsigned int getClassfileCount();

private:
    Options _options;
    bool _isValid{true};
    unsigned int _classfileCount{0};
};
}
#endif
