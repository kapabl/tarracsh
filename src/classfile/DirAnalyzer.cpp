

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
        _results.classfileCount++;

    } else {
        _results.classfileErrors++;
    }
}

void DirAnalyzer::publicShaProcess(filesystem::directory_entry const &dirEntry) {

    const auto filename = dirEntry.path().string();
    const auto size = filesystem::file_size(filename);

    const auto lastWriteTime = filesystem::last_write_time(filename);
    const auto timestamp = chrono::duration_cast<chrono::microseconds>(lastWriteTime.time_since_epoch()).count();

    const tables::ShaRow* row = _shaTable->findByKey(filename);
    const bool rowFound = row != nullptr;
    const auto unchangedFile = rowFound && row->fileSize == size && row->lastWriteTime == timestamp;

    if ( !unchangedFile) {

        Options classfileOptions(_options);
        classfileOptions.classFile = filename;

        ClassFileAnalyzer classFileAnalyzer(classfileOptions, _results);
        const auto shaResult = classFileAnalyzer.getPublicSha();
        
        if (shaResult.has_value()) {
            const auto isSamePublicSha = rowFound && shaResult.value() == row->sha256;

            if ( isSamePublicSha ) {
                _results.resultLog.writeln(std::format("Same public sha of changed file:{}", filename));
            }
            else {
                tables::ShaRow newRow;
                newRow.filename.ptr = _shaTable->getPoolString(filename);
                newRow.type = tables::EntryType::Classfile;
                newRow.lastWriteTime = timestamp;
                newRow.fileSize = size;
                newRow.classname.ptr = _shaTable->getPoolString(classFileAnalyzer.getFullClassname());
                newRow.sha256 = shaResult.value();
                _shaTable->add(newRow);
            }

            _results.classfileCount++;

        }
        else {
            _results.classfileErrors++;
        }
    }

}

/**
 * TODO implement thread pool, enqueue "tasks"
 */
void DirAnalyzer::processClassfile(filesystem::directory_entry const &dirEntry) {

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
    _results.jarfileCount++;
    _results.classfileCount += jarAnalyzer.getClassfileCount();
}

bool DirAnalyzer::initializePublicShaTable() {
    _shaTable = std::make_shared<PublicShaTable>(generateShaTablename());
    return _shaTable->read();
}

void DirAnalyzer::processDirEntry(filesystem::directory_entry const &dirEntry) {
    if (isJar(dirEntry)) {
        processJarFile(dirEntry);
    } else if (isClassfile(dirEntry)) {
        processClassfile(dirEntry);
    }

    cout << "\033[2K";
    cout << format("class files: {}, jar:{}", _results.classfileCount, _results.jarfileCount);
    cout << format(", class errors: {}, jar errors:{}", _results.classfileErrors, _results.jarfileErrors);
    cout << "\r" << std::flush;
}

void DirAnalyzer::run() {

    const auto start = chrono::high_resolution_clock::now();

    _results.resultLog.setFile(_options.logFile);

    if (_options.generatePublicSha) {
        if (!initializePublicShaTable()) return;
    }

    for (auto const &dirEntry : filesystem::recursive_directory_iterator(_options.directory)) {
        if (dirEntry.is_regular_file()) {
            processDirEntry(dirEntry);
        }
    }
    const auto end = chrono::high_resolution_clock::now();
    auto totalTime = chrono::duration_cast<chrono::seconds>(end - start);
    cout << endl << format("total time: {}", totalTime) << endl;

    if (_options.generatePublicSha) {
        _shaTable->write();
    }
}
