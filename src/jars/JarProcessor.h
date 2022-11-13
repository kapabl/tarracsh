#ifndef JAR_PROCESSOR_H
#define JAR_PROCESSOR_H
#include "../app/Tarracsh.h"
#include "../classfile/Stats.h"

#include "JarEntry.h"
#include "JarTask.h"

#include "../tables/ClassfilesTable.h"


namespace org::kapa::tarracsh::jar {
class JarProcessor {
public:
    explicit JarProcessor(
        Options options,
        stats::Results &results,
        JarTask &jarTask
       );

    JarProcessor(const JarProcessor &) = delete;
    JarProcessor(const JarProcessor &&) = delete;
    JarProcessor &operator=(const JarProcessor &) = delete;
    JarProcessor &operator=(const JarProcessor &&) = delete;

    [[nodiscard]] bool isValid() const { return _isValid; }

    ~JarProcessor() = default;
    void run();

    unsigned int getClassfileCount() const;


private:
    JarTask& _jarTask;
    stats::Results &_results;
    Options _options;
    bool _isValid{true};
    std::atomic<unsigned int> _classfileCount{0};

    [[nodiscard]] tables::ClassfileRow* getRow(const JarEntry& jarEntry) const;
    void waitForAvailableBuffer();


    BS::thread_pool _threadPool;
    std::mutex _taskMutex;
    std::mutex _jarMutex;
    std::condition_variable _jarCv;
    std::unordered_set<char*> _finishedBuffers;
    int _inProgressBufferCount{ 0 };

};
}
#endif
