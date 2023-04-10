#ifndef JAR_PROCESSOR_H
#define JAR_PROCESSOR_H
#include <BS_thread_pool.hpp>
#include <unordered_set>

#include "../stats/Results.h"

#include "JarEntry.h"
#include "tasks/Task.h"

#include "../db/table/ClassfilesTable.h"


namespace kapa::tarracsh::domain::jar {

using kapa::tarracsh::domain::stats::Results;

class Processor {
public:
    explicit Processor(
        Options options,
        Results &results,
        Task &task
       );

    Processor(const Processor &) = delete;
    Processor(const Processor &&) = delete;
    Processor &operator=(const Processor &) = delete;
    Processor &operator=(const Processor &&) = delete;

    [[nodiscard]] bool isValid() const { return _isValid; }

    ~Processor() = default;
    void run();

    unsigned int getClassfileCount() const;


private:
    Task& _task;
    Results &_results;
    Options _jarOptions;
    bool _isValid{true};
    std::atomic<unsigned int> _classfileCount{0};

    [[nodiscard]] db::table::ClassfileRow* getRow(const JarEntry& jarEntry) const;
    void waitForAvailableBuffer();


    // BS::thread_pool _threadPool{ std::max<unsigned int>( 1u, std::thread::hardware_concurrency()*1/4) };
    BS::thread_pool _threadPool{ std::max<unsigned int>( 1u, std::thread::hardware_concurrency()*2/5) };
    // BS::thread_pool _threadPool{ 2 };
    std::mutex _taskMutex;
    std::mutex _jarMutex;
    std::condition_variable _jarCv;
    std::unordered_set<char*> _finishedBuffers;
    int _inProgressBufferCount{ 0 };

};
}
#endif
