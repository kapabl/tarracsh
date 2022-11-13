#include "JarProcessor.h"


#include <BS_thread_pool.hpp>
#include <unordered_set>
#include <libzippp/libzippp.h>

#include "JarEntry.h"
#include "../ClassFileAnalyzer.h"
using namespace libzippp;


using namespace org::kapa::tarracsh;
using namespace tables;
using namespace stats;
using namespace jar;
using namespace std;


JarProcessor::JarProcessor(Options options, Results &results,
                           JarTask &jarTask)
    : _jarTask(jarTask),
      _results(results),
      _options(std::move(options)) {
}

constexpr int MaxConcurrentBuffers = 40;

void JarProcessor::waitForAvailableBuffer() {
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

void JarProcessor::run() {
    if ( _jarTask.start() ) {

        ZipArchive zipArchive(_options.jarFile);
        zipArchive.open(ZipArchive::ReadOnly);

        const auto entries = zipArchive.getEntries();
        std::atomic<int> index{ 0 };

        for (auto& entry : entries) {

            if (index % 1000 == 0) {
                _results.print(_options);
            }

            waitForAvailableBuffer();

            auto buffer = static_cast<char*>(entry.readAsBinary());
            _inProgressBufferCount++;

            _threadPool.push_task([
                this,
                    entry,
                    &index,
                    buffer] {
                    const JarEntry jarEntry(entry, buffer);
                    if (jarEntry.isClassfile()) {
                        ++_classfileCount;
                        ++index;
                        _jarTask.processEntry(jarEntry, _taskMutex);
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
    _jarTask.end();
    _results.jarfiles.parsedCount++;
    _results.print(_options);
}


unsigned int JarProcessor::getClassfileCount() const {
    return _classfileCount;
}
