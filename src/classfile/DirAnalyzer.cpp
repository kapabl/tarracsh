#include "DirAnalyzer.h"

#include <filesystem>
#include <iostream>
#include <utility>

#include "ClassFileAnalyzer.h"
#include "JarAnalyzer.h"
#include "tables/Table.h"

using namespace org::kapa::tarracsh::dir;
using namespace std;

DirAnalyzer::DirAnalyzer(Options options)
    : _options(move(options)), _shaTable(generateShaTablename()) {

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

    //TODO check table first

    auto filename = dirEntry.path().string();
    Options classfileOptions(_options);
    classfileOptions.classFile = filename;

    ClassFileAnalyzer classFileAnalyzer(classfileOptions, _results);

    const auto shaResult = classFileAnalyzer.getPublicSha();
    if (shaResult.has_value() ) {
        tables::ShaRow row( _shaTable.getStringPool(), filename);
        row.type = tables::EntryType::Classfile;
        row.creationDatetime = 0; //TODO
        row.fileSize = 0; //TODO
        _shaTable.add(row);
        _results.classfileCount++;

    } else {
        _results.classfileErrors++;
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
    std::string result(_options.outputDir);
    ranges::replace(result, '/', '_');
    ranges::replace(result, ':', '_');
    ranges::replace(result, '\\', '_');
    result = (std::filesystem::path(_options.outputDir) / result).string();

    return result;
}

void DirAnalyzer::run() {

    const auto start = chrono::high_resolution_clock::now();

    _results.resultLog.setFile(_options.logFile);

    for (auto const &dirEntry : filesystem::recursive_directory_iterator(_options.directory)) {
        if (dirEntry.is_regular_file()) {

            if (isJar(dirEntry)) {
                Options jarOptions(_options);
                jarOptions.jarFile = dirEntry.path().string();
                jar::JarAnalyzer jarAnalyzer(jarOptions);
                jarAnalyzer.run();
                _results.jarfileCount++;
                _results.classfileCount += jarAnalyzer.getClassfileCount();
            } else if (isClassfile(dirEntry)) {
                processClassfile(dirEntry);
            }

            cout << "\033[2K";
            cout << format("class files: {}, jar:{}", _results.classfileCount, _results.jarfileCount);
            cout << format(", class errors: {}, jar errors:{}", _results.classfileErrors, _results.jarfileErrors);
            cout << "\r" << std::flush;

        }
    }
    const auto end = chrono::high_resolution_clock::now();
    auto totalTime = chrono::duration_cast<chrono::seconds>(end - start);
    cout << endl << format("total time: {}", totalTime) << endl;

    if (_options.generatePublicSha) {
        _shaTable.write();
    }
}
