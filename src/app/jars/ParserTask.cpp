#include "ParserTask.h"


#include <libzippp/libzippp.h>
#include <BS_thread_pool.hpp>

#include "JarEntry.h"
#include "../domain/classfile/ClassFileAnalyzer.h"
#include "../domain/classfile/readers/MemoryReader.h"
using namespace libzippp;


using namespace kapa::tarracsh;
using namespace db;
using namespace stats;
using namespace jar;
using namespace std;


ParserTask::ParserTask(Options jarOptions, Results &results)
    : _results(results),
      _jarOptions(std::move(jarOptions)) {
}

bool ParserTask::start() {
    return true;
}

void ParserTask::parseEntry(const JarEntry &jarEntry) const {
    Options options(_jarOptions);
    options.classFilePath = jarEntry.getName();
    options.jarFile = _jarOptions.jarFile;
    readers::MemoryReader reader(jarEntry);
    ++_results.jarfiles.classfiles.count;
    ClassFileAnalyzer classFileAnalyzer(reader, options, _results);
    if (classFileAnalyzer.analyze()) {
        ++_results.jarfiles.classfiles.parsedCount;
    } else {
        ++_results.jarfiles.classfiles.errors;
    }
}

void ParserTask::processEntry(const JarEntry &jarEntry, std::mutex &taskMutex) {
    parseEntry(jarEntry);
}


void ParserTask::end() {
    // ++_results.jarfiles.digest.count;
    // _results.jarfiles.classfileCount += _jarFileRow->classfileCount;
    //
    // if (_isFileUnchanged) {
    //     _results.jarfiles.classfiles.digest.count += _jarFileRow->classfileCount;
    //     _results.jarfiles.classfiles.digest.unchangedCount += _jarFileRow->classfileCount;
    //     ++_results.jarfiles.digest.unchangedCount;
    //     return;
    // }
    //
    // digestUtils::DigestBuffer buffer;
    // for (auto& [buf] : _digestMap | views::values) {
    //     buffer.append(buf, DIGEST_LENGTH);
    // }
    // const auto digest = digestUtils::digest(buffer);
    //
    // const auto& filename = _options.jarFile;
    // const auto isSameDigest = !_isNewJarFile && _jarFileRow->digest == digest;
    //
    // if (isSameDigest) {
    //     _results.log->writeln(std::format("Same public digestEntry of changed jar file:{}", filename));
    //     ++_results.jarfiles.digest.same;
    // }
    // else {
    //     _jarFileRow->digest = digest;
    //     if (_isNewJarFile) {
    //         ++_results.jarfiles.digest.newFile;
    //     }
    //     else {
    //         ++_results.jarfiles.digest.differentDigest;
    //     }
    // }
    //TODO

}
