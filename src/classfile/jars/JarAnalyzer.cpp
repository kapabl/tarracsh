#include "JarAnalyzer.h"


#include <libzippp/libzippp.h>

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
    readers::MemoryReader reader(jarEntry.getZipEntry());

    ClassFileAnalyzer classFileAnalyzer(reader, classfileOptions, _results);
    if (classFileAnalyzer.run()) {
        _results.jarfiles.classfiles.parsedCount++;

    } else {
        _results.jarfiles.classfiles.errors++;
    }
}

void JarAnalyzer::analyze() {

    ZipArchive zipArchive(_options.jarFile);
    zipArchive.open(ZipArchive::ReadOnly);

    const auto entries = zipArchive.getEntries();
    for (auto &entry : entries) {
        JarEntry jarEntry(entry);
        if (jarEntry.isClassfile()) {
            cout << entry.getName() << endl;
            _classfileCount++;
            parseEntry(jarEntry);
        }
    }
}


void JarAnalyzer::run() {
    analyze();
}


unsigned int JarAnalyzer::getClassfileCount() const {
    return _classfileCount;
}
