#ifndef JAR_ANALYZER_H
#define JAR_ANALYZER_H
#include "../Tarracsh.h"
#include <ranges>
#include "JarEntry.h"

#include "../tables/ClassfileDigestTable.h"

namespace org::kapa::tarracsh::jar {
class JarAnalyzer {
public:
    explicit JarAnalyzer(Options options, Results &results);

    JarAnalyzer(const JarAnalyzer &) = delete;
    JarAnalyzer(const JarAnalyzer &&) = delete;
    JarAnalyzer &operator=(const JarAnalyzer &) = delete;
    JarAnalyzer &operator=(const JarAnalyzer &&) = delete;

    [[nodiscard]] bool isValid() const { return _isValid; }

    ~JarAnalyzer() = default;
    void run();


    unsigned int getClassfileCount() const;


private:
    Results &_results;
    Options _options;
    bool _isValid{true};
    unsigned int _classfileCount{0};

    void parseEntry(const JarEntry &jarEntry) const;
    void analyze();
};
}
#endif
