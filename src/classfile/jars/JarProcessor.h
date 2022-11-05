#ifndef JAR_PROCESSOR_H
#define JAR_PROCESSOR_H
#include "../Tarracsh.h"
#include "../Stats.h"

#include <ranges>
#include "JarEntry.h"
#include "JarTask.h"

#include "../tables/DigestTable.h"


namespace org::kapa::tarracsh::jar {
class JarProcessor {
public:
    explicit JarProcessor(
        Options options, 
        Results &results,
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
    Results &_results;
    Options _options;
    bool _isValid{true};
    std::atomic<unsigned int> _classfileCount{0};

    [[nodiscard]] tables::DigestRow* getRow(const JarEntry& jarEntry) const;
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
