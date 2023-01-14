#ifndef JAR_ANALYZER_H
#define JAR_ANALYZER_H
#include "../app/stats/Stats.h"
#include "JarEntry.h"
#include "Task.h"


namespace kapa::tarracsh::jar {

class ParserTask: public Task {
public:
    explicit ParserTask(
        Options jarOptions, stats::Results& results );

    void processEntry(const JarEntry& jarEntry, std::mutex& taskMutex) override;
    bool start() override;
    void end() override;



private:
    stats::Results &_results;
    Options _jarOptions;
    bool _isValid{true};
    unsigned int _classfileCount{0};

    void parseEntry(const JarEntry &jarEntry) const;
};
}
#endif
