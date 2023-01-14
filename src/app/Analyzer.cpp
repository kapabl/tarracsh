#include <filesystem>
#include <string>
#include "Analyzer.h"
#include "../infrastructure/filesystem/FilesystemUtils.h"
#include "../infrastructure/db/Database.h"

#include "../domain/classfile/ClassFileInfo.h"
#include "../domain/classfile/ClassFileAnalyzer.h"
#include "jars/ParserTask.h"
#include "jars/DigestTask.h"
#include "jars/GraphTask.h"
#include "jars/Processor.h"
#include "../domain/classfile/readers/FileReader.h"
#include "../db/table/FilesTable.h"
#include "stats/Stats.h"
#include "stats/ScopedTimer.h"

using namespace std;

using namespace kapa::tarracsh::app;
using namespace kapa::tarracsh::db;
using namespace digest;
using namespace callgraph;
using namespace kapa::tarracsh::stats::profiler;


bool Analyzer::isJarInput() const {
    return !_options.jarFile.empty();
}

bool Analyzer::isDirInput() const {
    return !_options.directory.empty();
}

bool Analyzer::isClassfileInput() const {
    return !_options.classFilePath.empty();
}

Analyzer::Analyzer(Config& config)
    : _options(config.getOptions()),
      _results(config.getResults()),
      _digestDb(_options.outputDir, config.getLog()),
      _callGraphDb(_options.outputDir, config.getLog()) {

}

void Analyzer::analyzeClassfile(const std::string &filename) const {
    Options classfileOptions(_options);
    classfileOptions.classFilePath = filename;

    readers::FileReader reader(classfileOptions.classFilePath);

    ClassFileAnalyzer classFileAnalyzer(reader, classfileOptions, _results);
    if (classFileAnalyzer.analyze()) {
        ++_results.classfiles.parsedCount;

    } else {
        ++_results.classfiles.errors;
    }
}

bool Analyzer::isFileUnchanged(const uintmax_t size, const long long timestamp, const digest::FileRow *row) const {
    auto const result = _options.useFileTimestamp &&
                        row != nullptr &&
                        row->fileSize == size &&
                        row->lastWriteTime == timestamp;
    return result;
}

void Analyzer::updateDbInMemory(const ClassFileInfo &classFileInfo,
                                const ClassFileAnalyzer &classFileAnalyzer,
                                const db::digest::columns::DigestCol &digest) {
    const auto files = _digestDb.getFiles();
    digest::FileRow fileRow;
    fileRow.filename = _digestDb.getPoolString(classFileInfo.filename);
    fileRow.type = digest::columns::EntryType::Classfile;
    fileRow.lastWriteTime = classFileInfo.timestamp;
    fileRow.fileSize = classFileInfo.size;
    fileRow.digest = digest;
    fileRow.id = files->addOrUpdate(fileRow);

    digest::ClassfileRow digestRow(fileRow);
    digestRow.size = classFileInfo.size;
    digestRow.lastWriteTime = classFileInfo.timestamp;
    digestRow.digest = digest;
    digestRow.classname = _digestDb.getPoolString(classFileAnalyzer.getMainClassname());
    digestRow.id = _digestDb.getClassfiles()->addOrUpdate(digestRow);
}

void Analyzer::digestClassfile(const std::string &filename) {

    ClassFileInfo classFileInfo(filename);

    const digest::FileRow *row = _digestDb.getFiles()->findByKey(classFileInfo.filename);
    const bool rowFound = row != nullptr;
    const auto isFileChanged = !isFileUnchanged(classFileInfo.size, classFileInfo.timestamp, row);

    ++_results.classfiles.digest.count;

    if (isFileChanged) {

        Options classfileOptions(_options);
        classfileOptions.classFilePath = classFileInfo.filename;
        readers::FileReader reader(classFileInfo.filename);

        ClassFileAnalyzer classFileAnalyzer(reader, classfileOptions, _results);
        const auto publicDigest = classFileAnalyzer.getPublicDigest();

        if (publicDigest.has_value()) {
            const auto isSamePublicDigest = rowFound && publicDigest.value() == row->digest;

            if (isSamePublicDigest) {
                _results.log->writeln(std::format("Same public digestEntry of changed file:{}", classFileInfo.filename));
                ++_results.classfiles.digest.same;
            } else {
                if (rowFound) {
                    ++_results.classfiles.digest.differentDigest;
                } else {
                    ++_results.classfiles.digest.newFile;
                }
            }

            ++_results.classfiles.parsedCount;
            updateDbInMemory(classFileInfo, classFileAnalyzer, publicDigest.value());

        } else {
            ++_results.classfiles.errors;
        }

    } else {
        ++_results.classfiles.digest.unchangedCount;
    }

}

void Analyzer::processClassfile(const std::string &filename) {
    _fileThreadPool.push_task([this, filename] {
        ++_results.classfiles.count;
        if (_options.isPublicDigest) {
            digestClassfile(filename);
        } else if (_options.isCallGraph) {
            //TODO
            //callGraph(dirEntry);
        } else {
            analyzeClassfile(filename);
        }

        if (_options.canPrintProgress()) {
            _results.print();
        }
    });

}

void Analyzer::processJar(const std::string &filename) {
    _fileThreadPool.push_task([this,filename] {
        Options jarOptions(_options);
        jarOptions.jarFile = filename;

        ++_results.jarfiles.count;
        if (_options.isPublicDigest) {
            jar::DigestTask jarDigestTask(jarOptions, _results, _digestDb);
            jar::Processor jarProcessor(jarOptions, _results, jarDigestTask);
            jarProcessor.run();
        } else if (_options.isCallGraph) {
            jar::GraphTask jarGraphTask(jarOptions, _results, _callGraphDb);
            jar::Processor jarProcessor(jarOptions, _results, jarGraphTask);
            jarProcessor.run();
        } else {
            jar::ParserTask jarParserTask(jarOptions, _results);
            jar::Processor jarProcessor(jarOptions, _results, jarParserTask);
            jarProcessor.run();
        }
    });
}


bool Analyzer::initDb(infrastructure::db::Database &db) const {

    ScopedTimer timer(&_results.profileData->initDb);
    auto result = true;

    db.init();

    if (_options.rebuild) {
        db.clean();
    } else {
        result = db.read();
    }

    return result;
}



void Analyzer::processFile(const std::filesystem::directory_entry &dirEntry) {
    if (infrastructure::filesystem::utils::isJar(dirEntry)) {
        processJar(dirEntry.path().string());
    } else if (infrastructure::filesystem::utils::isClassfile(dirEntry)) {
        processClassfile(dirEntry.path().string());
    }
}

bool Analyzer::initAnalyzer() {
    ScopedTimer timer(&_results.profileData->initAnalyzer);
    _results.log->setFile(_options.logFile);

    if (_options.isPublicDigest) {
        if (!initDb(_digestDb)) return false;
    } else if (_options.isCallGraph) {
        if (!initDb(_callGraphDb)) return false;
    }
    return true;
}


void Analyzer::processDirInput() {
    for (auto const &dirEntry : std::filesystem::recursive_directory_iterator(_options.directory)) {
        if (dirEntry.is_regular_file()) {
            processFile(dirEntry);
        }
    }
}

void Analyzer::processJarInput() {
    processJar(_options.jarFile);
}

void Analyzer::processClassfileInput() {
    processClassfile(_options.classFilePath);
}

void Analyzer::analyzeInput() {
    if (isDirInput()) {
        processDirInput();
    } else if (isJarInput()) {
        processJarInput();
    } else if (isClassfileInput()) {
        processClassfileInput();
    }

    _fileThreadPool.wait_for_tasks();
}


void Analyzer::updateDbs() {
    if (_options.isPublicDigest) {
        ScopedTimer timer(&_results.profileData->writeDigestDb);
        _digestDb.write();
    } else if (_options.isCallGraph) {
        ScopedTimer timer(&_results.profileData->writeCallGraphDb);
        _callGraphDb.write();
    }
}

void Analyzer::endAnalysis() {
    if (!_options.dryRun) {
        updateDbs();
    }
}

void Analyzer::run() {
    {
        ScopedTimer timer(&_results.profileData->analyzerTime);

        if (initAnalyzer()) {
            analyzeInput();
            endAnalysis();
        }
    }

    if (_options.canPrintProgress()) {
        _results.forcePrint();
        _results.printAll();
    }

    if (_options.printDiffReport) {
        _results.report->print();
    }

}
