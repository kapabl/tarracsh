#include "JarAnalyzer.h"


#include <libzippp/libzippp.h>
#include <BS_thread_pool.hpp>

#include "JarEntry.h"
#include "../ClassFileAnalyzer.h"
#include "../readers/MemoryReader.h"
using namespace libzippp;


using namespace org::kapa::tarracsh;
using namespace tables;
using namespace jar;
using namespace std;


JarAnalyzer::JarAnalyzer(Options options, Results &results)
    : _results(results),
      _options(std::move(options)) {
}

void JarAnalyzer::parseEntry(const JarEntry &jarEntry) const {
    //TODO collect fore stats like total number of methods etc

    Options classfileOptions(_options);
    classfileOptions.classFilePath = jarEntry.getName();
    readers::MemoryReader reader(jarEntry);

    ClassFileAnalyzer classFileAnalyzer(reader, classfileOptions, _results);
    if (classFileAnalyzer.run()) {
        _results.jarfiles.classfiles.parsedCount++;

    } else {
        _results.jarfiles.classfiles.errors++;
    }
}

void JarAnalyzer::analyze() {
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


void JarAnalyzer::run() {
    analyze();
}


unsigned int JarAnalyzer::getClassfileCount() const {
    return _classfileCount;
}
