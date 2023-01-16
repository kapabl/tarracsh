#ifndef JAR_ANALYZER_H
#define JAR_ANALYZER_H
#include "../../stats/Results.h"
#include "../../Options.h"
#include "../JarEntry.h"
#include "Task.h"
#include "../../classfile/ClassFileParser.h"


namespace kapa::tarracsh::domain::jar::tasks {

using kapa::tarracsh::domain::stats::Results;

class ParserTask: public Task {
public:
    explicit ParserTask(
        Options jarOptions, 
        Results& results, 
        std::function<void(classfile::ClassFileParser&)> onParserDone );

    void processEntry(const JarEntry& jarEntry, std::mutex& taskMutex) override;
    bool start() override;
    void end() override;

private:
    Results &_results;
    Options _jarOptions;
    bool _isValid{true};
    unsigned int _classfileCount{0};

    std::function<void(classfile::ClassFileParser&)> _onParserDone;
    void parseEntry(const JarEntry &jarEntry) const;

    void prepareOptions(const jar::JarEntry& jarEntry, Options& options) const;
};
}
#endif
