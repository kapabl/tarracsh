#ifndef JAR_ANALYZER_H
#define JAR_ANALYZER_H
#include "../Tarracsh.h"
#include "../Stats.h"
#include "JarEntry.h"


namespace org::kapa::tarracsh::jar {
class JarAnalyzerTask {
public:
    explicit JarAnalyzerTask(Options options, Results &results);

    JarAnalyzerTask(const JarAnalyzerTask &) = delete;
    JarAnalyzerTask(const JarAnalyzerTask &&) = delete;
    JarAnalyzerTask &operator=(const JarAnalyzerTask &) = delete;
    JarAnalyzerTask &operator=(const JarAnalyzerTask &&) = delete;

    [[nodiscard]] bool isValid() const { return _isValid; }

    ~JarAnalyzerTask() = default;
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
