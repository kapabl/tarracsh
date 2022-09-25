#include <filesystem>
#include <iostream>
#include <utility>
#include <string>

#include "ClassFileAnalyzer.h"
#include "JarAnalyzer.h"
#include "DirAnalyzer.h"


using namespace org::kapa::tarracsh::dir;
using namespace org::kapa::tarracsh::tables;
using namespace std;

DirAnalyzer::DirAnalyzer(Options options)
    : _options(move(options))/*, _shaTable(generateShaTablename())*/ {

}

bool DirAnalyzer::isJar(filesystem::directory_entry const &dirEntry) {
    return dirEntry.path().extension() == ".jar";
}

bool DirAnalyzer::isClassfile(filesystem::directory_entry const &dirEntry) {
    return dirEntry.path().extension() == ".class";
}

void DirAnalyzer::regularProcess(filesystem::directory_entry const &dirEntry) {
    Options classfileOptions(_options);
    classfileOptions.classFile = dirEntry.path().string();

    ClassFileAnalyzer classFileAnalyzer(classfileOptions, _results);
    if (classFileAnalyzer.run()) {
        _results.classfiles.parsedCount++;

    } else {
        _results.classfiles.errors++;
    }
}

void DirAnalyzer::publicShaProcess(filesystem::directory_entry const &dirEntry) {

    const auto filename = dirEntry.path().string();
    const auto size = filesystem::file_size(filename);

    const auto lastWriteTime = filesystem::last_write_time(filename);
    const auto timestamp = chrono::duration_cast<chrono::microseconds>(lastWriteTime.time_since_epoch()).count();

    const tables::Md5Row *row = _shaTable->findByKey(filename);
    const bool rowFound = row != nullptr;
    const auto unchangedFile = rowFound && row->fileSize == size && row->lastWriteTime == timestamp;

    _results.publicSha.count++;

    if (!unchangedFile) {

        Options classfileOptions(_options);
        classfileOptions.classFile = filename;

        ClassFileAnalyzer classFileAnalyzer(classfileOptions, _results);
        const auto shaResult = classFileAnalyzer.getPublicDigest();

        if (shaResult.has_value()) {
            const auto isSamePublicSha = rowFound && shaResult.value() == row->md5;

            if (isSamePublicSha) {
                _results.resultLog.writeln(std::format("Same public sha of changed file:{}", filename));
                _results.publicSha.sameSha++;
            } else {

                tables::Md5Row newRow;
                newRow.filename.ptr = _shaTable->getPoolString(filename);
                newRow.type = tables::EntryType::Classfile;
                newRow.lastWriteTime = timestamp;
                newRow.fileSize = size;
                newRow.classname.ptr = _shaTable->getPoolString(classFileAnalyzer.getMainClassname());
                newRow.md5 = shaResult.value();
                _shaTable->addOrUpdate(newRow);

                if (rowFound) {
                    _results.publicSha.differentSha++;
                } else {
                    _results.publicSha.newFile++;
                }
            }

            _results.classfiles.parsedCount++;

        } else {
            _results.classfiles.errors++;
        }
    } else {
        _results.publicSha.unchangedCount++;
    }

}

/**
 * TODO implement thread pool, enqueue "tasks"
 */
void DirAnalyzer::processClassfile(filesystem::directory_entry const &dirEntry) {
    _results.classfiles.count++;
    if (_options.generatePublicSha) {
        publicShaProcess(dirEntry);
    } else {
        regularProcess(dirEntry);
    }

}

std::string DirAnalyzer::generateShaTablename() const {
    std::string result(_options.directory);
    ranges::replace(result, '/', '_');
    ranges::replace(result, ':', '_');
    ranges::replace(result, '\\', '_');
    result += tables::TableExtension;
    result = (std::filesystem::path(_options.outputDir) / result).string();

    return result;
}

void DirAnalyzer::processJarFile(filesystem::directory_entry const &dirEntry) {
    Options jarOptions(_options);
    jarOptions.jarFile = dirEntry.path().string();
    jar::JarAnalyzer jarAnalyzer(jarOptions);
    jarAnalyzer.run();
    _results.jarfiles.count++;
    _results.classfiles.count += jarAnalyzer.getClassfileCount();
}

bool DirAnalyzer::initializePublicShaTable() {
    _shaTable = std::make_shared<PublicMd5Table>(generateShaTablename());
    const auto result = _options.rebuild ? _shaTable->clean() : _shaTable->read();
    return _shaTable->read();
}

void DirAnalyzer::printDirEntryStats() {
    cout << "\033[2K";

    cout << format("classfiles No:\033[36m{}\033[39m|Ok:\033[92m{}\033[39m|Er:\033[91m{}\033[39m--",
                   _results.classfiles.count,
                   _results.classfiles.parsedCount,
                   _results.classfiles.errors);

    cout << format("jars No:\033[36m{}\033[39m|Ok:\033[92m{}\033[39m|Er:\033[91m{}\033[39m--",
                   _results.jarfiles.count,
                   _results.jarfiles.parsedCount,
                   _results.jarfiles.errors);

    if (_options.generatePublicSha) {
        cout << format("public sha No:{}|New:{}|Same:{}|Diff:{}|Unchanged:{}",
                       _results.publicSha.count,
                       _results.publicSha.newFile,
                       _results.publicSha.sameSha,
                       _results.publicSha.differentSha,
                       _results.publicSha.unchangedCount
            );
    }
    cout << "\r" << std::flush;
}

void DirAnalyzer::processDirEntry(filesystem::directory_entry const &dirEntry) {
    if (isJar(dirEntry)) {
        processJarFile(dirEntry);
    } else if (isClassfile(dirEntry)) {
        processClassfile(dirEntry);
    }
}

bool DirAnalyzer::initDirAnalysis() {
    _results.resultLog.setFile(_options.logFile);

    if (_options.generatePublicSha) {
        if (!initializePublicShaTable()) return false;
    }
    return true;
}

void DirAnalyzer::analyze() {
    auto count = 0u;
    for (auto const &dirEntry : filesystem::recursive_directory_iterator(_options.directory)) {
        if (dirEntry.is_regular_file()) {
            processDirEntry(dirEntry);
        }
        count++;

        if (count % 10 == 0) {
            printDirEntryStats();
        }
    }
    printDirEntryStats();
}

void DirAnalyzer::endDirAnalysis() const {
    if (_options.generatePublicSha && _shaTable->isDirty()) {
        _shaTable->write();
    }
}

void DirAnalyzer::run() {

    PrintTimeScope timeScope(true);

    if (!initDirAnalysis()) return;
    analyze();
    endDirAnalysis();

}
