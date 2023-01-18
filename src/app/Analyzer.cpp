#include <filesystem>
#include <string>
#include "Analyzer.h"
#include "../infrastructure/filesystem/Utils.h"
#include "../infrastructure/db/Database.h"

#include "classfile/constantpool/printer/ConstantPoolPrinter.h"
#include "classfile/constantpool/printer/nav/HtmlGen.h"
#include "classfile/ParserPrinter.h"
#include "../domain/digest/ClassFileDigest.h"
#include "../domain/classfile/ClassFileParser.h"
#include "../domain/jar/tasks/ParserTask.h"
#include "../domain/jar/tasks/DigestTask.h"
#include "../domain/jar/tasks/GraphTask.h"
#include "../domain/jar/Processor.h"
#include "../domain/classfile/reader/FileReader.h"
#include "../domain/stats/Results.h"
#include "../domain/stats/ScopedTimer.h"

using namespace std;

using kapa::tarracsh::app::classfile::constantpool::printer::nav::HtmlGen;


using kapa::tarracsh::app::classfile::ParserPrinter;
using kapa::tarracsh::app::classfile::constantpool::printer::ConstantPoolPrinter;

using kapa::tarracsh::domain::classfile::reader::FileReader;
using kapa::tarracsh::domain::jar::tasks::DigestTask;
using kapa::tarracsh::domain::jar::tasks::GraphTask;
using kapa::tarracsh::domain::jar::tasks::ParserTask;
using kapa::tarracsh::domain::jar::Processor;
using kapa::tarracsh::domain::db::digest::DigestDb;
using kapa::tarracsh::domain::db::callgraph::CallGraphDb;
using kapa::tarracsh::domain::db::digest::FileRow;
using kapa::tarracsh::domain::db::digest::ClassfileRow;
using kapa::tarracsh::domain::db::digest::columns::DigestCol;
using kapa::tarracsh::domain::db::digest::columns::EntryType;
using kapa::tarracsh::domain::digest::ClassFileDigest;
using kapa::tarracsh::domain::Options;
using kapa::tarracsh::domain::stats::profiler::ScopedTimer;


using namespace kapa::tarracsh::app;


bool Analyzer::isJarInput() const {
    return !_options.jarFile.empty();
}

bool Analyzer::isDirInput() const {
    return !_options.directory.empty();
}

bool Analyzer::isClassfileInput() const {
    return !_options.classFilePath.empty();
}

Analyzer::Analyzer(Config &config, const std::shared_ptr<infrastructure::db::Database> db)
    : _options(config.getOptions()),
      _results(config.getResults()),
      _db(db)
{
}

Analyzer::Analyzer(Config& config)
    : _options(config.getOptions()),
    _results(config.getResults())
{
}

void Analyzer::parseClassfile(const std::string &filename) const {
    domain::Options classfileOptions(_options);
    classfileOptions.classFilePath = filename;

    FileReader reader(classfileOptions.classFilePath);

    ClassFileParser parser(reader, classfileOptions, _results);
    if (parser.parse()) {
        ++_results.standaloneClassfiles.parsedCount;

    } else {
        ++_results.standaloneClassfiles.errors;
    }

    classFileParserDone(parser);
}

bool Analyzer::isFileUnchanged(const uintmax_t size, const long long timestamp, const FileRow *row) const {
    auto const result = _options.useFileTimestamp &&
                        row != nullptr &&
                        row->fileSize == size &&
                        row->lastWriteTime == timestamp;
    return result;
}

DigestDb& Analyzer::getDigestDb() const {
    auto& result = reinterpret_cast<DigestDb&>(*_db);
    return result;
}

CallGraphDb& Analyzer::getCallGraphDb() const {
    auto& result = reinterpret_cast<CallGraphDb&>(*_db);
    return result;
}


void Analyzer::updateDbInMemory(const StandaloneClassFileInfo &classFileInfo,
                                const ClassFileParser &parser,
                                const DigestCol &digest) {

    auto& digestDb = getDigestDb();
    const auto files = digestDb.getFiles();
    FileRow fileRow;
    fileRow.filename = digestDb.getPoolString(classFileInfo.filename);
    fileRow.type = EntryType::Classfile;
    fileRow.lastWriteTime = classFileInfo.timestamp;
    fileRow.fileSize = classFileInfo.size;
    fileRow.digest = digest;
    fileRow.id = files->addOrUpdate(fileRow);

    ClassfileRow digestRow(fileRow);
    digestRow.size = classFileInfo.size;
    digestRow.lastWriteTime = classFileInfo.timestamp;
    digestRow.digest = digest;
    digestRow.classname = digestDb.getPoolString(parser.getMainClassname());
    digestRow.id = digestDb.getClassfiles()->addOrUpdate(digestRow);
}

void Analyzer::digestClassfile(const std::string &filename) {
    auto& digestDb = getDigestDb();

    StandaloneClassFileInfo fileInfo(filename);

    const FileRow *fileRow = digestDb.getFiles()->findByKey(fileInfo.filename);
    const bool fileExists = fileRow != nullptr;
    const auto isFileChanged = !isFileUnchanged(fileInfo.size, fileInfo.timestamp, fileRow);

    ++_results.standaloneClassfiles.digest.count;

    if (isFileChanged) {

        Options classfileOptions(_options);
        classfileOptions.classFilePath = fileInfo.filename;
        FileReader reader(fileInfo.filename);

        ClassFileParser classFileParser(reader, classfileOptions, _results);
        if (classFileParser.parse()) {

            const auto strongClassname = domain::digestUtils::getStrongClassname(
                filename.c_str(),
                classFileParser.getMainClassname().c_str());

            const ClassFileDigest classFileDigest(classFileParser);
            const auto digest = classFileDigest.digest();

            if (fileExists) {
                const auto isSamePublicDigest = digest == fileRow->digest;
                if (isSamePublicDigest) {
                    _results.log->writeln(std::format("Same public digest of changed file:{}",
                                                      fileInfo.filename));
                }
                _results.report->asModifiedClassfile(filename, isSamePublicDigest, strongClassname);
            } else {
                _results.report->asNewClassfile(filename, strongClassname);
            }

            ++_results.standaloneClassfiles.parsedCount;
            updateDbInMemory(fileInfo, classFileParser, digest);

        } else {
            _results.report->asFailedClassfile(filename);
        }

    } else {
        _results.report->asUnchangedClassfile(filename);
    }

}

void Analyzer::processClassfile(const std::string &filename) {
    _fileThreadPool.push_task([this, filename] {
        ++_results.standaloneClassfiles.count;
        if (_options.isPublicDigest) {
            digestClassfile(filename);
        } else if (_options.isCallGraph) {
            //TODO
            //callGraph(dirEntry);
        } else {
            parseClassfile(filename);
        }

        if (_options.canPrintProgress()) {
            _results.print();
        }
    });

}

void Analyzer::classFileParserDone(ClassFileParser& parser) const {
    if (!parser.succeeded()) return;

    if (_options.printConstantPool) {
        ConstantPoolPrinter constantPoolPrinter(parser);
        constantPoolPrinter.print();
    }
    else if (_options.printCPoolHtmlNav) {
        HtmlGen htmlGen(parser);
        htmlGen.print();
    }

    if (_options.printClassParse) {
        ParserPrinter parserPrinter(parser);
        parserPrinter.print();
    }
}

void Analyzer::processJar(const std::string &filename) {
    _fileThreadPool.push_task([this,filename] {
        Options jarOptions(_options);
        jarOptions.jarFile = filename;

        ++_results.jarfiles.count;
        if (_options.isPublicDigest) {
            DigestTask jarDigestTask(jarOptions, _results, reinterpret_cast<domain::db::digest::DigestDb&>(_db));
            Processor jarProcessor(jarOptions, _results, jarDigestTask);
            jarProcessor.run();
        } else if (_options.isCallGraph) {
            //TODO
            // GraphTask jarGraphTask(jarOptions, _results, _callGraphDb);
            // Processor jarProcessor(jarOptions, _results, jarGraphTask);
            // jarProcessor.run();
        } else {
            ParserTask jarParserTask(jarOptions, _results,[this]( ClassFileParser& parser) -> void {
                classFileParserDone(parser);
            });
            Processor jarProcessor(jarOptions, _results, jarParserTask);
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
//TODO extract
    // if (_options.isPublicDigest) {
    //     if (!initDb(_digestDb)) return false;
    // } else if (_options.isCallGraph) {
    //     if (!initDb(_callGraphDb)) return false;
    // }
    return true;
}


void Analyzer::processDirInput() {
    for (auto const &dirEntry : std::filesystem::recursive_directory_iterator(_options.directory)) {
        if (dirEntry.is_regular_file()) {
            processFile(dirEntry);
        }
    }
}

void Analyzer::analyzeInput() {
    if (isDirInput()) {
        processDirInput();
    } else if (isJarInput()) {
        processJar(_options.jarFile);
    } else if (isClassfileInput()) {
        processClassfile(_options.classFilePath);
    }

    _fileThreadPool.wait_for_tasks();
}


void Analyzer::updateDbs() {
    //TODO extract
    // if (_options.isPublicDigest) {
    //     ScopedTimer timer(&_results.profileData->writeDigestDb);
    //     _digestDb.write();
    // } else if (_options.isCallGraph) {
    //     ScopedTimer timer(&_results.profileData->writeCallGraphDb);
    //     _callGraphDb.write();
    // }
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
