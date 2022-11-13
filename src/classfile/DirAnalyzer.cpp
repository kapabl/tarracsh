#include <filesystem>
#include <iostream>
#include <utility>
#include <string>

#include "ClassFileAnalyzer.h"
#include "../jars/JarAnalyzerTask.h"
#include "DirAnalyzer.h"

#include "FilesystemUtils.h"
#include "../jars/JarDigestTask.h"
#include "../jars/JarProcessor.h"
#include "readers/FileReader.h"
#include "../tables/FilesTable.h"


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
        ++_results.classfiles.parsedCount;

    } else {
        ++_results.classfiles.errors;
    }
}

bool DirAnalyzer::isFileUnchanged(const uintmax_t size, const long long timestamp, const FileRow *row) const {
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

    const FileRow *row = _filesTable->findByKey(filename);
    const bool rowFound = row != nullptr;
    const auto unchangedFile = isFileUnchanged(size, timestamp, row);

    ++_results.classfiles.digest.count;

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
                ++_results.classfiles.digest.same;
            } else {

                FileRow fileRow;
                fileRow.filename = _filesTable->getPoolString(filename);
                fileRow.type = Classfile;
                fileRow.lastWriteTime = timestamp;
                fileRow.fileSize = size;
                fileRow.md5 = publicDigest.value();
                fileRow.id = _filesTable->addOrUpdate(fileRow);

                ClassfileRow digestRow(fileRow);
                digestRow.size = size;
                digestRow.lastWriteTime = timestamp;
                digestRow.md5 = publicDigest.value();
                digestRow.classname = _digestTable->getPoolString(classFileAnalyzer.getMainClassname());
                digestRow.id = _digestTable->addOrUpdate(digestRow);

                if (rowFound) {
                    ++_results.classfiles.digest.differentDigest;
                } else {
                    ++_results.classfiles.digest.newFile;
                }
            }

            ++_results.classfiles.parsedCount;

        } else {
            ++_results.classfiles.errors;
        }
    } else {
        ++_results.classfiles.digest.unchangedCount;
    }

}

/**
 * TODO implement thread _threadPool, enqueue "tasks"
 */
void DirAnalyzer::processClassfile(filesystem::directory_entry const &dirEntry) {
    ++_results.classfiles.count;
    if (_options.generatePublicDigest) {
        digestClassfile(dirEntry);
    } else {
        analyze(dirEntry);
    }

}

std::string DirAnalyzer::generateFileTablename(const string &name) const {
    std::string result(name + TableExtension);
    // std::string result(_options.directory);
    // ranges::replace(result, '/', '_');
    // ranges::replace(result, ':', '_');
    // ranges::replace(result, '\\', '_');
    // result += "-" + name + TableExtension;
    result = (std::filesystem::path(_options.outputDir) / result).string();

    return result;
}

std::string DirAnalyzer::generateStringPoolFilename(const string &name) const {
    std::string result(name + StringPoolExtension);
    // std::string result(_options.directory);
    // ranges::replace(result, '/', '_');
    // ranges::replace(result, ':', '_');
    // ranges::replace(result, '\\', '_');
    // result += "-" + name + StringPoolExtension;
    result = (std::filesystem::path(_options.outputDir) / result).string();

    return result;
}

// void DirAnalyzer::processJar(filesystem::directory_entry dirEntry) {
void DirAnalyzer::processJar(std::string filename) {
    _jarThreadPool.push_task([this,filename] {
        Options jarOptions(_options);
        // jarOptions.jarFile = dirEntry.path().string();
        jarOptions.jarFile = filename;

        ++_results.jarfiles.count;
        if (_options.generatePublicDigest) {
            jar::JarDigestTask jarDigestTask(jarOptions, _results, _digestTable, _filesTable);
            jar::JarProcessor jarProcessor(jarOptions, _results, jarDigestTask);
            jarProcessor.run();
            //_results.jarfiles.classfileCount += jarProcessor.getClassfileCount();
        } else {
            //TODO
            // jar::JarAnalyzeTask jarAnalyzeTask(jarOptions, _results);
            // jar::JarProcessor jarProcessor(jarOptions, _results, jarAnalyzeTask);
            // jarProcessor.run();
            //_results.jarfiles.classfileCount += jarAnalyzer.getClassfileCount();
        }
    });
}

bool DirAnalyzer::initializeDigestTables() {
    auto result = true;

    _stringPool = std::make_shared<StringPool>(generateStringPoolFilename("sp"));
    _filesTable = std::make_shared<FilesTable>(generateFileTablename("files"), _stringPool);
    _digestTable = std::make_shared<ClassfilesTable>(generateFileTablename("digest"), _stringPool, _filesTable);

    if (_options.rebuild) {
        _stringPool->clean();
        _filesTable->clean();
        _digestTable->clean();
    } else {
        result = _stringPool->read() &&
                 _filesTable->read() &&
                 _digestTable->read();
    }

    return result;
}


void DirAnalyzer::processFile(const filesystem::directory_entry &dirEntry) {
    if (isJar(dirEntry)) {
        // processJar(dirEntry);
        processJar(dirEntry.path().string());
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
        }
        count++;
        if (count % 1000 == 0) {
            _results.print(_options);
        }
    }
    _jarThreadPool.wait_for_tasks();
    _results.print(_options);
    _results.printAll(_options);
}

void DirAnalyzer::endAnalysis() const {
    if (_options.generatePublicDigest) {
        if (_stringPool->isDirty()) {
            _stringPool->write();
        }
        if (_filesTable->isDirty()) {
            _filesTable->write();
        }
        if (_digestTable->isDirty()) {
            _digestTable->write();
        }
    }
}

void DirAnalyzer::run() {

    PrintTimeScope timeScope(true);

    if (initDirAnalysis()) {
        analyze();
        endAnalysis();
    }
    cout << endl << ((1.0 * _results.classfiles.count +
                      _results.jarfiles.classfileCount) / (timeScope.getElapsedTime().count() * 1.0))
        << " classfile/s: " << endl;

}
