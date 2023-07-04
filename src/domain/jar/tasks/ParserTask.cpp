#include "ParserTask.h"


#include <libzippp/libzippp.h>
#include <BS_thread_pool.hpp>

#include "../JarEntry.h"
#include "../../classfile/ClassFileParser.h"
#include "../../classfile/reader/MemoryReader.h"
using namespace libzippp;



using namespace kapa::tarracsh::domain;

using namespace classfile;
using namespace db;
using namespace db::digest;
using namespace jar::tasks;

using namespace std;


ParserTask::ParserTask(Options jarOptions, Results &results, const std::function<void(ClassFileParser&)> onParserDone)
    : Task( jarOptions, results ),
      _onParserDone(onParserDone) {
}

bool ParserTask::start() {
    return true;
}

void ParserTask::parseEntry(const JarEntry &jarEntry) const {
    reader::MemoryReader reader(jarEntry);
    ++_results.jarfiles.classfiles.count;
    //TODO instead of jarEntry.getName() this can be jarfile@jarEntry.getName()
    ClassFileParser classFileParser(reader, jarEntry.getName(), _results.log);
    if (classFileParser.parse()) {
        ++_results.jarfiles.classfiles.parsedCount;
    } else {
        ++_results.jarfiles.classfiles.errors;
    }
    _onParserDone(classFileParser);
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
    //     _results.log->writeln(fmt::format("Same public digestEntry of changed jar file:{}", filename));
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
