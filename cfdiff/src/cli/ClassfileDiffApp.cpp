#include "CfDiff.h"
#include "ClassfileDiffApp.h"
#include "infrastructure/filesystem/Utils.h"
#include "domain/classfile/ClassFileParser.h"
#include "domain/classfile/reader/FileReader.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

using kapa::infrastructure::app::cli::CliApp;
using kapa::infrastructure::app::cli::ExitCode;
using kapa::tarracsh::domain::Options;
using kapa::tarracsh::domain::classfile::ClassFileParser;
using kapa::tarracsh::domain::classfile::reader::FileReader;
using kapa::infrastructure::log::Log;

using kapa::infrastructure::filesystem::utils::ensureDir;


using namespace kapa::cldiff::app;

std::unique_ptr<ClassfileDiffApp> ClassfileDiffApp::_app;


void ClassfileDiffApp::setupCliOptions() {
    set_version_flag("--version", "version " CF_DIFF_VERSION);
    set_help_all_flag("--help-all");

    add_flag("--ignore-classname", _ignoreClassname, "Ignore classname during comparison");
    add_flag("-p,--public-only", _publicOnly, "yes/no, default yes");

    allow_extras();

    callback([this]() {
        const auto &args = remaining();
        if (args.size() != 2) {
            throw CLI::ValidationError("Exactly two files are required.");
        }
        _leftFile = args[0];
        _rightFile = args[1];
    });
}


void ClassfileDiffApp::validateFile(const std::string &filename) const {
    if (!std::filesystem::exists(filename)) {
        throw CLI::ValidationError(std::format("File does not exist: {}", filename), CLI::ExitCodes::FileError);
    }
}

ExitCode ClassfileDiffApp::parseCli(int argc, char **argv) {
    ExitCode result = 0;
    try {
        parse(argc, argv);

        validateFile(_leftFile);
        validateFile(_rightFile);

    } catch (const CLI::ParseError &e) {
        result = exit(e);
    }
    return result;
}

void ClassfileDiffApp::outputResult() const {
    _log->writeln("Equalities found:", true );
    for (const auto& equality : _equalities) {
        _log->writeln(equality, true);
    }
    _log->writeln("", true);
    _log->writeln("", true);

    if (_differences.empty()) {
        _log->writeln("No differences found", true);
    } else {
        _log->writeln("Differences found:");
        for (const auto &difference : _differences) {
            _log->writeln(difference, true);
        }
    }
}

ExitCode ClassfileDiffApp::start(int argc, char *argv[]) {
    setupCliOptions();
    auto exitCode = parseCli(argc, argv);
    if (exitCode != 0) {
        return exitCode;
    }

    FileReader leftReader(_leftFile);
    ClassFileParser leftParser(leftReader, _leftFile, _log);
    if (leftParser.parse()) {
        FileReader rightReader(_rightFile);
        ClassFileParser rightParser(rightReader, _rightFile, _log);
        if (rightParser.parse()) {
            this->compare(leftParser, rightParser);
            outputResult();
        } else {
            _log->writeln("Failed to parse right file");
            exitCode = 1;
        }

    } else {
        _log->writeln("Failed to parse left file");
        exitCode = 1;
    }

    return exitCode;
}

ExitCode ClassfileDiffApp::run(int argc, char **argv) {
    const auto log = std::make_shared<Log>();
    _app.reset(new ClassfileDiffApp("", "cfdiff", log));

    const auto result = _app->start(argc, argv);

    if (_app->_options.pause) {
        std::cin.get();
    }

    return result;

}

ClassfileDiffApp::ClassfileDiffApp(const std::string &description, const std::string &name, std::shared_ptr<Log> log)
    : CliApp(description, name), _log(log), _results(_options) {

    _results.log = log;
}

void ClassfileDiffApp::compareSuperClass(ClassFileParser &leftParser,
    ClassFileParser &rightParser) {
    //TODO
}

void ClassfileDiffApp::compareMainClassAccessFlags(ClassFileParser &leftParser,
    ClassFileParser &rightParser) {
    //TODO
    if ( leftParser.getMainClassInfo()) {
               //TODO
    } else {
               //TODO
    }
}

void ClassfileDiffApp::compareMainClass(ClassFileParser &leftParser, ClassFileParser &rightParser) {
    //TODO
    if ( _ignoreClassname) {
        _equalities.emplace_back("Classname ignored");
    } else {
        //TODO 
    }
    compareSuperClass(leftParser, rightParser);
    compareMainClassAccessFlags(leftParser, rightParser);
}

void ClassfileDiffApp::compareMethods(ClassFileParser &leftParser, ClassFileParser &rightParser) {
    //TODO
}

void ClassfileDiffApp::compareFields(ClassFileParser &leftParser, ClassFileParser &rightParser) {
    //TODO
}

void ClassfileDiffApp::compareMembers(ClassFileParser &leftParser, ClassFileParser &rightParser) {
    compareMethods(leftParser, rightParser);
    compareFields(leftParser, rightParser);
}

void ClassfileDiffApp::compare(ClassFileParser &leftParser,
                               ClassFileParser &rightParser) {
    compareMainClass(leftParser, rightParser);
    compareMembers(leftParser, rightParser);
}


void ClassfileDiffApp::init() const {

    auto result = true;

#ifdef _WIN32
    prepareConsoleForVT100();
    prepareConsoleForUTF8();
#endif

    ensureDir(_options.outputDir);
    _log->init(_options.logFile);

}
