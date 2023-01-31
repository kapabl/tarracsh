#include <filesystem>
#include <string>
#include "Analyzer.h"
#include "../infrastructure/filesystem/Utils.h"
#include "../infrastructure/db/Database.h"
#include "../infrastructure/db/table/Table.inl"

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
using kapa::tarracsh::domain::db::digest::columns::EntryType;
using kapa::tarracsh::domain::digest::ClassFileDigest;
using kapa::tarracsh::domain::Options;
using kapa::tarracsh::domain::stats::profiler::ScopedTimer;

using kapa::infrastructure::db::tables::columns::DigestCol;


using namespace kapa::tarracsh::app;

Analyzer::Analyzer(Context &config, const std::shared_ptr<infrastructure::db::Database> db)
    : _options(config.getOptions()),
      _inputOptions(_options.getInputOptions()),
      _results(config.getResults()),
      _db(db) {
}

Analyzer::Analyzer(Context &config)
    : _options(config.getOptions()),
      _inputOptions(_options.getInputOptions()),
      _results(config.getResults()) {
}

void Analyzer::parseClassfile(const std::string &filename) const {
    domain::Options classfileOptions(_options);
    classfileOptions.digest.input = filename;

    FileReader reader(filename);

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

DigestDb &Analyzer::getDigestDb() const {
    auto &result = reinterpret_cast<DigestDb &>(*_db);
    return result;
}

CallGraphDb &Analyzer::getCallGraphDb() const {
    auto &result = reinterpret_cast<CallGraphDb &>(*_db);
    return result;
}


void Analyzer::updateDbInMemory(const StandaloneClassFileInfo &classFileInfo,
                                const ClassFileParser &parser,
                                const DigestCol &digest) const {

    auto &digestDb = getDigestDb();
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
    auto &digestDb = getDigestDb();

    StandaloneClassFileInfo fileInfo(filename);

    const FileRow *fileRow = digestDb.getFiles()->findByKey(fileInfo.filename);
    const bool fileExists = fileRow != nullptr;
    const auto isFileChanged = !isFileUnchanged(fileInfo.size, fileInfo.timestamp, fileRow);

    ++_results.standaloneClassfiles.digest.count;

    if (isFileChanged) {

        Options classfileOptions(_options);
        classfileOptions.digest.input = fileInfo.filename;
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
                _results.report->asModifiedClassfile(isSamePublicDigest, strongClassname);
            } else {
                _results.report->asNewClassfile(strongClassname);
            }

            ++_results.standaloneClassfiles.parsedCount;
            if (!_options.digest.dryRun) {
                updateDbInMemory(fileInfo, classFileParser, digest);
            }

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

void Analyzer::classFileParserDone(ClassFileParser &parser) const {
    if (!parser.succeeded()) return;

    if (_options.parse.printConstantPool) {
        ConstantPoolPrinter constantPoolPrinter(parser);
        constantPoolPrinter.print();
    } else if (_options.parse.printCPoolHtmlNav) {
        HtmlGen htmlGen(parser);
        htmlGen.print();
    }

    if (_options.parse.print) {
        ParserPrinter parserPrinter(parser);
        parserPrinter.print();
    }
}

void Analyzer::processJar(const std::string &filename) {
    _fileThreadPool.push_task([this,filename] {
        Options jarOptions(_options);
        jarOptions.getInputOptions().input = filename;

        ++_results.jarfiles.count;
        if (_options.isPublicDigest) {
            DigestTask jarDigestTask(jarOptions, _results, reinterpret_cast<DigestDb &>(*_db));
            Processor jarProcessor(jarOptions, _results, jarDigestTask);
            jarProcessor.run();
        } else if (_options.isCallGraph) {
            //TODO
            // GraphTask jarGraphTask(jarOptions, _results, _callGraphDb);
            // Processor jarProcessor(jarOptions, _results, jarGraphTask);
            // jarProcessor.run();
        } else {
            ParserTask jarParserTask(jarOptions, _results, [this](ClassFileParser &parser) -> void {
                classFileParserDone(parser);
            });
            Processor jarProcessor(jarOptions, _results, jarParserTask);
            jarProcessor.run();
        }
    });
}


void Analyzer::processFile(const std::filesystem::directory_entry &dirEntry) {
    if (infrastructure::filesystem::utils::isJar(dirEntry)) {
        processJar(dirEntry.path().string());
    } else if (infrastructure::filesystem::utils::isClassfile(dirEntry)) {
        processClassfile(dirEntry.path().string());
    }
}

bool Analyzer::initAnalyzer() const {
    ScopedTimer timer(&_results.profileData->initAnalyzer);
    _results.log->setFile(_options.logFile);
    return true;
}

void Analyzer::serverLog(const std::string &string, const bool doStdout) const {
    if (!_options.digest.server.isServerMode) return;
    _results.log->writeln(string, doStdout);

}

void Analyzer::processDirInput() {
  
    for (auto const &dirEntry : std::filesystem::recursive_directory_iterator(_inputOptions.input)) {
        if (dirEntry.is_regular_file()) {
            processFile(dirEntry);
        }
    }
}

void Analyzer::analyzeInput() {
    if (_options.digest.isDir) {
        serverLog(std::format("processing directory: {}", _options.digest.input), true);
        processDirInput();
    } else if (_options.digest.isJar) {
        serverLog(std::format("processing jar: {}", _options.digest.input), true);
        processJar(_inputOptions.input);
    } else if (_options.digest.isClassfile) {
        serverLog(std::format("processing classfile: {}", _options.digest.input), true);
        processClassfile(_inputOptions.input);
    }

    _fileThreadPool.wait_for_tasks();
}


void Analyzer::updateDbs() {

    if (_options.isPublicDigest) {
        ScopedTimer timer(&_results.profileData->writeDigestDb);
        _db->stop();
        //_db->write();
    } else if (_options.isCallGraph) {
        ScopedTimer timer(&_results.profileData->writeCallGraphDb);
        _db->stop();
        //_db->write();
    }
}

void Analyzer::endAnalysis() {
    if (!_options.digest.dryRun) {
        if (!_options.digest.server.isServerMode) {
            updateDbs();
        }
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
}

void Analyzer::runWithPrint() {
    run();
    if (_options.canPrintProgress()) {
        _results.printProgress();
        _results.printAll();
    }

}
