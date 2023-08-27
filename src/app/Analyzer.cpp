#include <filesystem>
#include <string>
#include "Analyzer.h"
#include "infrastructure/filesystem/Utils.h"
#include "infrastructure/db/Database.h"

#include "classfile/constantpool/printer/ConstantPoolPrinter.h"
#include "classfile/constantpool/printer/nav/HtmlGen.h"
#include "classfile/ParserPrinter.h"
#include "domain/classfile/ClassFileParser.h"
#include "domain/jar/tasks/ParserTask.h"
#include "domain/jar/Processor.h"
#include "domain/classfile/reader/FileReader.h"
#include "domain/stats/Results.h"
#include "infrastructure/profiling/ScopedTimer.h"

using namespace std;

using kapa::tarracsh::app::classfile::constantpool::printer::nav::HtmlGen;


using kapa::tarracsh::app::classfile::ParserPrinter;
using kapa::tarracsh::app::classfile::constantpool::printer::ConstantPoolPrinter;

using kapa::tarracsh::domain::classfile::reader::FileReader;
using kapa::tarracsh::domain::jar::tasks::ParserTask;
using kapa::tarracsh::domain::jar::Processor;
using kapa::tarracsh::domain::Options;
using kapa::infrastructure::profiler::ScopedTimer;

using namespace kapa::tarracsh::app;

Analyzer::Analyzer(Context &config, const std::shared_ptr<infrastructure::db::Database> db)
    : _options(config.getOptions()),
      _inputOptions(_options.getBaseOptions()),
      _results(config.getResults()),
      _db(db) {
}

Analyzer::Analyzer(Context &config)
    : _options(config.getOptions()),
      _inputOptions(_options.getBaseOptions()),
      _results(config.getResults()) {
}

void Analyzer::parseClassfile(const std::string &filename) const {

    FileReader reader(filename);

    ClassFileParser parser(reader, filename, _results.log);
    if (parser.parse()) {
        ++_results.standaloneClassfiles.parsedCount;

    } else {
        ++_results.standaloneClassfiles.errors;
    }

    classFileParserDone(parser);
}

void Analyzer::doClassfile(const std::string &filename) {
    parseClassfile(filename);
}

void Analyzer::processStandaloneClassfile(const std::string &filename) {
    _fileThreadPool.push_task([this, filename] {
        ++_results.standaloneClassfiles.count;
        doClassfile(filename);
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
        HtmlGen htmlGen(parser, _options);
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
        jarOptions.getBaseOptions().input = filename;

        ++_results.jarfiles.count;
        ParserTask jarParserTask(jarOptions, _results, [this](ClassFileParser &parser) -> void {
            classFileParserDone(parser);
        });
        Processor jarProcessor(jarOptions, _results, jarParserTask);
        jarProcessor.run();

    });
}

void Analyzer::endAnalysis() {
}


void Analyzer::processFile(const std::filesystem::directory_entry &dirEntry) {
    if (infrastructure::filesystem::utils::isJar(dirEntry)) {
        processJar(dirEntry.path().string());
    } else if (infrastructure::filesystem::utils::isClassfile(dirEntry)) {
        processStandaloneClassfile(dirEntry.path().string());
    }
}

bool Analyzer::initAnalyzer() const {
    ScopedTimer timer(&_results.profileData->initAnalyzer);
    _results.log->setFile(_options.logFile);
    return true;
}

void Analyzer::serverLog(const std::string &string, const bool doStdout) {
    if (!_options.getBaseOptions().server.isServerMode) return;
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
    if (_inputOptions.isDir) {
        serverLog(fmt::format("processing directory: {}", _inputOptions.input), true);
        processDirInput();
    } else if (_inputOptions.isJar) {
        serverLog(fmt::format("processing jar: {}", _inputOptions.input), true);
        processJar(_inputOptions.input);
    } else if (_inputOptions.isClassfile) {
        serverLog(fmt::format("processing classfile: {}", _inputOptions.input), true);
        processStandaloneClassfile(_inputOptions.input);
    }

    _fileThreadPool.wait_for_tasks();
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
