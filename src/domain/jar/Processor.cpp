#include "Processor.h"

#include <BS_thread_pool.hpp>
#include <unordered_set>
#include <libzippp/libzippp.h>

#include "JarEntry.h"
#include "../domain/classfile/ClassFileParser.h"
using namespace libzippp;


using namespace kapa::tarracsh::domain::jar;
using namespace std;


Processor::Processor(Options options, Results &results,
                           Task &task)
    : _task(task),
      _results(results),
      _jarOptions(std::move(options)) {
}

constexpr int MaxConcurrentBuffers = 40;

void Processor::waitForAvailableBuffer() {
    unique_lock jarLock(_jarMutex);
    _jarCv.wait(jarLock, [this] {

        while (!_finishedBuffers.empty()) {
            auto it = _finishedBuffers.begin();
            delete[] *it;
            _finishedBuffers.erase(it);
            _inProgressBufferCount--;

        }
        return _inProgressBufferCount < MaxConcurrentBuffers;
    });

}

void Processor::run() {
    if (_task.start()) {

        ZipArchive zipArchive(_jarOptions.getInputOptions().input);
        zipArchive.open(ZipArchive::ReadOnly);

        const auto entries = zipArchive.getEntries();
        std::atomic<int> index{0};

        for (auto &entry : entries) {

            if (index % 100 == 0) {
                if (_jarOptions.canPrintProgress()) {
                    _results.print();
                }
            }

            waitForAvailableBuffer();

            auto buffer = static_cast<char *>(entry.readAsBinary());
            _inProgressBufferCount++;

            _threadPool.push_task([
                this,
                entry,
                &index,
                buffer] 
                {
                const JarEntry jarEntry(entry, buffer);
                if (jarEntry.isClassfile()) {
                    ++_classfileCount;
                    ++index;
                    _task.processEntry(jarEntry, _taskMutex);
                }

                {
                    std::unique_lock jarLock(_jarMutex);
                    _finishedBuffers.insert(buffer);
                    _jarCv.notify_one();
                }
            });
        }

        _threadPool.wait_for_tasks();
    }
    _task.end();
    ++_results.jarfiles.parsedCount;
    if (_jarOptions.canPrintProgress()) {
        _results.print();
    }
}


unsigned int Processor::getClassfileCount() const {
    return _classfileCount;
}
