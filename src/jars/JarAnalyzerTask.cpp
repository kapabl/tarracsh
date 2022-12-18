#include "JarAnalyzerTask.h"


#include <libzippp/libzippp.h>
#include <BS_thread_pool.hpp>

#include "JarEntry.h"
#include "../classfile/ClassFileAnalyzer.h"
#include "../classfile/readers/MemoryReader.h"
using namespace libzippp;


using namespace org::kapa::tarracsh;
using namespace db;
using namespace stats;
using namespace jar;
using namespace std;


JarAnalyzerTask::JarAnalyzerTask(Options options, Results &results)
    : _results(results),
      _options(std::move(options)) {
}

void JarAnalyzerTask::parseEntry(const JarEntry &jarEntry) const {
    //TODO collect fore stats like total number of methods etc

    Options classfileOptions(_options);
    classfileOptions.classFilePath = jarEntry.getName();
    readers::MemoryReader reader(jarEntry);

    ClassFileAnalyzer classFileAnalyzer(reader, classfileOptions, _results);
    if (classFileAnalyzer.run()) {
        ++_results.jarfiles.classfiles.parsedCount;

    } else {
        ++_results.jarfiles.classfiles.errors;
    }
}

void JarAnalyzerTask::analyze() {
    //TODO
    // ZipArchive zipArchive(_options.jarFile);
    // zipArchive.open(ZipArchive::ReadOnly);
    //
    // const auto entries = zipArchive.getEntries();
    // BS::thread_pool pool;
    //
    // for (auto &entry : entries) {
    //     pool.push_task([this, entry] {
    //         const JarEntry jarEntry(entry);
    //         if (jarEntry.isClassfile()) {
    //             cout << entry.getName() << endl;
    //             _classfileCount++;
    //             parseEntry(jarEntry);
    //         }
    //     });
    // }
    // pool.wait_for_tasks();
}


void JarAnalyzerTask::run() {
    analyze();
}


unsigned int JarAnalyzerTask::getClassfileCount() const {
    return _classfileCount;
}
