#include "JarDigester.h"


#include <libzippp/libzippp.h>

#include "JarEntry.h"
#include "../ClassFileAnalyzer.h"
#include "../FilesystemUtils.h"
#include "../readers/MemoryReader.h"
using namespace libzippp;


using namespace org::kapa::tarracsh;
using namespace tables;
using namespace jar;
using namespace std;


JarDigester::JarDigester(Options options, Results &results, std::shared_ptr<ClassfileDigestTable> digestTable)
    : _digestTable(std::move(digestTable)),
      _results(results),
      _options(std::move(options)) {
}

ClassfileDigestRow *JarDigester::getRow(const JarEntry &entry) const {
    auto *result = _digestTable->findByKey(ClassfileDigestTable::createKey(
        _options.jarFile.c_str(),
        entry.getClassname().c_str()));

    return result;
}

optional<MD5> JarDigester::parseEntry(const JarEntry &jarEntry, const ClassfileDigestRow *row) const {
    optional<MD5> result;
    Options classfileOptions(_options);
    classfileOptions.classFilePath = jarEntry.getName();
    readers::MemoryReader reader(jarEntry.getZipEntry());

    ClassFileAnalyzer classFileAnalyzer(reader, classfileOptions, _results);
    const auto digest = classFileAnalyzer.getPublicDigest();

    if (digest.has_value()) {
        const bool alreadyExists = row != nullptr;
        const auto isSameDigest = alreadyExists && digest.value() == row->md5;

        if (isSameDigest) {
            _results.resultLog.writeln(std::format("Same public digestEntry of changed file:{}",
                                                   ClassfileDigestTable::createKey(*row)));
            _results.jarfiles.classfiles.digest.same++;
            result = row->md5;
        } else {
            const auto classname = classFileAnalyzer.getMainClassname();
            ClassfileDigestRow newRow;
            newRow.filename.ptr = _digestTable->getPoolString(_options.jarFile);
            newRow.type = EntryType::Classfile;
            newRow.lastWriteTime = jarEntry.getLastWriteTime();
            newRow.fileSize = jarEntry.getSize();
            newRow.classname.ptr = _digestTable->getPoolString(classname);
            newRow.md5 = digest.value();
            _digestTable->addOrUpdate(newRow);

            result = newRow.md5;

            if (alreadyExists) {
                _results.jarfiles.classfiles.digest.differentDigest++;
            } else {
                _results.jarfiles.classfiles.digest.newFile++;
            }
        }

        _results.jarfiles.classfiles.parsedCount++;

    } else {
        _results.jarfiles.classfiles.errors++;
    }
    return result;
}

bool JarDigester::isUnchanged(const JarEntry &jarEntry, const tables::ClassfileDigestRow *row) {
    return row->fileSize == jarEntry.getSize() &&
           row->crc == jarEntry.getCRC() &&
           row->lastWriteTime == jarEntry.getLastWriteTime();
}

optional<MD5> JarDigester::digestEntry(const JarEntry &jarEntry) const {
    optional<MD5> result;
    _results.jarfiles.classfiles.digest.count++;
    const auto *row = getRow(jarEntry);

    if (row != nullptr && isUnchanged(jarEntry, row)) {
        _results.jarfiles.classfiles.digest.unchangedCount++;
        result = row->md5;
        return result;
    }

    result = parseEntry(jarEntry, row);

    return result;

}

bool JarDigester::isFileUnchanged(const uintmax_t size, const long long timestamp,
                                  const tables::ClassfileDigestRow *row) const {
    auto const result = _options.useFileTimestamp &&
                        row != nullptr &&
                        row->fileSize == size &&
                        row->lastWriteTime == timestamp;
    return result;
}


optional<MD5> JarDigester::getPublicDigest() {
    optional<MD5> result;

    ZipArchive zipArchive(_options.jarFile);
    zipArchive.open(ZipArchive::ReadOnly);

    const auto entries = zipArchive.getEntries();
    map<string, MD5> digestMap;
    _results.print(_options);
    auto index = 0;
    for (auto &entry : entries) {
        JarEntry jarEntry(entry);
        if (jarEntry.isClassfile()) {
            //cout << entry.getName() << endl;
            _classfileCount++;
            auto classDigest = digestEntry(jarEntry);
            digestMap[jarEntry.getClassname()] = classDigest.value();
            index++;

            if (index % 10 == 0) {
                _results.print(_options);
            }
        }
    }

    _results.print(_options);

    Poco::MD5Engine md5Engine;
    Poco::DigestOutputStream stream(md5Engine);

    for (const auto &[buf] : digestMap | views::values) {
        stream.write(reinterpret_cast<const char *>(buf), MD5_DIGEST_LENGTH);
    }

    stream.close();
    MD5 md5Bytes;
    memcpy(md5Bytes.buf, &*md5Engine.digest().begin(), MD5_DIGEST_LENGTH);
    result = md5Bytes;

    return result;
}


void JarDigester::digest() {
    const auto &filename = _options.jarFile;
    const auto size = filesystem::file_size(filename);
    const auto timestamp = fsUtils::getLastWriteTimestamp(filename);
    const ClassfileDigestRow *row = _digestTable->findByKey(filename);
    const bool isExistingJar = row != nullptr;
    const auto unchangedFile = isFileUnchanged(size, timestamp, row);
    if (!unchangedFile) {

        const auto digest = getPublicDigest();

        if (digest.has_value()) {
            const auto isSameDigest = isExistingJar && digest.value() == row->md5;

            if (isSameDigest) {
                _results.resultLog.writeln(std::format("Same public digestEntry of changed jar file:{}", filename));
                _results.jarfiles.digest.same++;
            } else {

                ClassfileDigestRow newRow;
                newRow.filename.ptr = _digestTable->getPoolString(filename);
                newRow.type = EntryType::Jar;
                newRow.lastWriteTime = timestamp;
                newRow.fileSize = size;
                newRow.md5 = digest.value();
                _digestTable->addOrUpdate(newRow);

                if (isExistingJar) {
                    _results.jarfiles.digest.differentDigest++;
                } else {
                    _results.jarfiles.digest.newFile++;
                }
            }
            _results.jarfiles.digest.count++;
        } else {
            _results.jarfiles.errors++;
        }
    } else {
        _results.jarfiles.classfiles.digest.count += static_cast<int>(row->count);
    }
}


void JarDigester::run() {
    digest();
}


unsigned int JarDigester::getClassfileCount() const {
    return _classfileCount;
}
