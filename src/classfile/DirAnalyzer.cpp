#include <filesystem>
#include <iostream>
#include <utility>
#include <string>

#include "ClassFileAnalyzer.h"
#include "jars/JarAnalyzer.h"
#include "DirAnalyzer.h"

#include "FilesystemUtils.h"
#include "jars/JarProcessor.h"
#include "readers/FileReader.h"


using namespace org::kapa::tarracsh::dir;
using namespace org::kapa::tarracsh::tables;
using namespace std;

DirAnalyzer::DirAnalyzer(Options options)
    : _options(move(options))/*, _digestTable(generateDigestTablename())*/ {

}

bool DirAnalyzer::isJar(filesystem::directory_entry const &dirEntry) {
    return dirEntry.path().extension() == ".jar";
}

bool DirAnalyzer::isClassfile(filesystem::directory_entry const &dirEntry) {
    return dirEntry.path().extension() == ".class";
}

void DirAnalyzer::analyze(filesystem::directory_entry const &dirEntry) {
    Options options(_options);
    options.classFilePath = dirEntry.path().string();

    readers::FileReader reader(options.classFilePath);

    ClassFileAnalyzer classFileAnalyzer(reader, options, _results);
    if (classFileAnalyzer.run()) {
        _results.classfiles.parsedCount++;

    } else {
        _results.classfiles.errors++;
    }
}

bool DirAnalyzer::isFileUnchanged(const uintmax_t size, const long long timestamp, const DigestRow *row) const {
    auto const result = _options.useFileTimestamp &&
                        row != nullptr &&
                        row->fileSize == size &&
                        row->lastWriteTime == timestamp;
    return result;
}

void DirAnalyzer::digestClassfile(filesystem::directory_entry const &dirEntry) {

    const auto filename = dirEntry.path().string();
    const auto size = filesystem::file_size(filename);

    const auto timestamp = fsUtils::getLastWriteTimestamp(filename);

    const tables::DigestRow *row = _digestTable->findByKey(filename);
    const bool rowFound = row != nullptr;
    const auto unchangedFile = isFileUnchanged(size, timestamp, row);

    _results.classfiles.digest.count++;

    if (!unchangedFile) {

        Options classfileOptions(_options);
        classfileOptions.classFilePath = filename;
        readers::FileReader reader(filename);

        ClassFileAnalyzer classFileAnalyzer(reader, classfileOptions, _results);
        const auto publicDigest = classFileAnalyzer.getPublicDigest();

        if (publicDigest.has_value()) {
            const auto isSamePublicDigest = rowFound && publicDigest.value() == row->md5;

            if (isSamePublicDigest) {
                _results.resultLog.writeln(std::format("Same public digestEntry of changed file:{}", filename));
                _results.classfiles.digest.same++;
            } else {

                DigestRow newRow;
                newRow.filename = _digestTable->getPoolString(filename);
                newRow.type = tables::EntryType::Classfile;
                newRow.lastWriteTime = timestamp;
                newRow.fileSize = size;
                newRow.classname = _digestTable->getPoolString(classFileAnalyzer.getMainClassname());
                newRow.md5 = publicDigest.value();
                _digestTable->addOrUpdate(newRow);

                if (rowFound) {
                    _results.classfiles.digest.differentDigest++;
                } else {
                    _results.classfiles.digest.newFile++;
                }
            }

            _results.classfiles.parsedCount++;

        } else {
            _results.classfiles.errors++;
        }
    } else {
        _results.classfiles.digest.unchangedCount++;
    }

}

/**
 * TODO implement thread _threadPool, enqueue "tasks"
 */
void DirAnalyzer::processClassfile(filesystem::directory_entry const &dirEntry) {
    _results.classfiles.count++;
    if (_options.generatePublicDigest) {
        digestClassfile(dirEntry);
    } else {
        analyze(dirEntry);
    }

}

std::string DirAnalyzer::generateDigestTablename() const {
    std::string result(_options.directory);
    ranges::replace(result, '/', '_');
    ranges::replace(result, ':', '_');
    ranges::replace(result, '\\', '_');
    result += tables::TableExtension;
    result = (std::filesystem::path(_options.outputDir) / result).string();

    return result;
}

void DirAnalyzer::processJar(filesystem::directory_entry const &dirEntry) {

    Options jarOptions(_options);
    jarOptions.jarFile = dirEntry.path().string();

    _results.jarfiles.count++;
    if (_options.generatePublicDigest) {
        jar::JarDigestTask jarDigestTask(jarOptions, _results, _digestTable);
        jar::JarProcessor jarProcessor(jarOptions, _results, jarDigestTask);
        jarProcessor.run();
        _results.jarfiles.classfileCount += jarProcessor.getClassfileCount();
    } else {
        //TODO
        // jar::JarAnalyzeTask jarAnalyzeTask(jarOptions, _results);
        // jar::JarProcessor jarProcessor(jarOptions, _results, jarAnalyzeTask);
        // jarProcessor.run();
        //_results.jarfiles.classfileCount += jarAnalyzer.getClassfileCount();
    }
}

bool DirAnalyzer::initializeDigestTables() {
    _digestTable = std::make_shared<DigestTable>(generateDigestTablename());
    const auto result = _options.rebuild ? _digestTable->clean() : _digestTable->read();
    return result;
}


void DirAnalyzer::processFile(filesystem::directory_entry const &dirEntry) {
    if (isJar(dirEntry)) {
        processJar(dirEntry);
    } else if (isClassfile(dirEntry)) {
        processClassfile(dirEntry);
    }
}

bool DirAnalyzer::initDirAnalysis() {
    _results.resultLog.setFile(_options.logFile);

    if (_options.generatePublicDigest) {
        if (!initializeDigestTables()) return false;
    }
    return true;
}

void DirAnalyzer::analyze() {
    auto count = 0u;
    for (auto const &dirEntry : filesystem::recursive_directory_iterator(_options.directory)) {
        if (dirEntry.is_regular_file()) {
            processFile(dirEntry);
        } else if (dirEntry.is_directory()) {
            //TODO recursive?
        }
        count++;

        if (count % 10 == 0) {
            _results.print(_options);
        }
    }
    _results.print(_options);
}

void DirAnalyzer::endAnalysis() const {
    if (_options.generatePublicDigest && _digestTable->isDirty()) {
        _digestTable->write();
    }
}

void DirAnalyzer::run() {

    PrintTimeScope timeScope(true);

    if (!initDirAnalysis()) return;
    analyze();
    endAnalysis();

}
