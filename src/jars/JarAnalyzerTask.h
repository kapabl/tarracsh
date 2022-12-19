#ifndef JAR_ANALYZER_H
#define JAR_ANALYZER_H
#include "../app/Tarracsh.h"
#include "../app/Stats.h"
#include "JarEntry.h"
#include "JarTask.h"


namespace org::kapa::tarracsh::jar {
class JarAnalyzerTask: public JarTask {
public:
    explicit JarAnalyzerTask(
        Options options, stats::Results& results );

    void processEntry(const JarEntry& jarEntry, std::mutex& taskMutex) override;
    bool start() override;
    void end() override;



private:
    stats::Results &_results;
    Options _options;
    bool _isValid{true};
    unsigned int _classfileCount{0};

    void parseEntry(const JarEntry &jarEntry) const;
};
}
#endif
