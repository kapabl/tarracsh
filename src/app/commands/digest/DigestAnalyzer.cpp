#include <filesystem>
#include "DigestAnalyzer.h"
#include "infrastructure/db/Database.h"

#include "app/classfile/constantpool/printer/ConstantPoolPrinter.h"
#include "app/classfile/constantpool/printer/nav/HtmlGen.h"
#include "app/classfile/ParserPrinter.h"
#include "domain/Utils.h"
#include "domain/digest/ClassFileDigest.h"
#include "domain/jar/tasks/ParserTask.h"
#include "domain/jar/tasks/DigestTask.h"
#include "domain/jar/tasks/GraphTask.h"
#include "domain/jar/Processor.h"
#include "domain/classfile/reader/FileReader.h"
#include "infrastructure/profiling/ScopedTimer.h"

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
using kapa::tarracsh::domain::db::table::FileRow;
using kapa::tarracsh::domain::db::table::ClassfileRow;
using kapa::tarracsh::domain::db::digest::column::EntryType;
using kapa::tarracsh::domain::digest::ClassFileDigest;
using kapa::tarracsh::domain::Options;
using kapa::infrastructure::profiler::ScopedTimer;

using kapa::infrastructure::db::table::column::DigestCol;


using namespace kapa::tarracsh::app::commands::digest;

DigestAnalyzer::DigestAnalyzer(Context &config, const std::shared_ptr<infrastructure::db::Database> db)
    : Analyzer(config, db) {
}

DigestAnalyzer::DigestAnalyzer(Context &config)
    : Analyzer(config) {
}

void DigestAnalyzer::endAnalysis() {
    if (!_options.digest.dryRun) {
        if (!_options.digest.server.isServerMode) {
            ScopedTimer timer(&_results.profileData->writeCallGraphDb);
            _db->stop();
            //_db->write();
        }
    }
}

void DigestAnalyzer::processJar(const std::string &filename) {
    _fileThreadPool.push_task([this, filename] {
        Options jarOptions(_options);
        jarOptions.getBaseOptions().input = filename;

        ++_results.jarfiles.count;

        DigestTask jarDigestTask(jarOptions, _results, reinterpret_cast<DigestDb &>(*_db));
        Processor jarProcessor(jarOptions, _results, jarDigestTask);
        jarProcessor.run();

    });
}


void DigestAnalyzer::doClassfile(const std::string& filename) {
    auto& digestDb = getDigestDb();

    StandaloneClassFileInfo fileInfo(filename);

    const FileRow* fileRow = static_cast<FileRow*>(digestDb.getFiles()->findByKey(fileInfo.filename));
    const bool fileExists = fileRow != nullptr;
    const auto isFileChanged = !isFileUnchanged(fileInfo.size, fileInfo.timestamp, fileRow);

    ++_results.standaloneClassfiles.taskResult.count;

    if (isFileChanged) {

        FileReader reader(fileInfo.filename);
        ClassFileParser classFileParser(reader, fileInfo.filename, _results.log);

        if (classFileParser.parse()) {

            const auto strongClassname = domain::utils::getStrongClassname(
                filename.c_str(),
                classFileParser.getMainClassname().c_str());

            const ClassFileDigest classFileDigest(classFileParser);
            const auto digest = classFileDigest.digest();

            if (fileExists) {
                const auto isSamePublicDigest = digest == fileRow->digest;
                if (isSamePublicDigest) {
                    _results.log->writeln(fmt::format("Same public digest of changed file:{}",
                        fileInfo.filename));
                }
                _results.report->asModifiedClassfile(isSamePublicDigest, strongClassname);
            }
            else {
                _results.report->asNewClassfile(strongClassname);
            }

            ++_results.standaloneClassfiles.parsedCount;
            if (!_options.digest.dryRun) {
                updateDbInMemory(fileInfo, classFileParser, digest);
            }

        }
        else {
            _results.report->asFailedClassfile(filename);
        }

    }
    else {
        _results.report->asUnchangedClassfile(filename);
    }

}

bool DigestAnalyzer::isFileUnchanged(const uintmax_t size, const long long timestamp, const FileRow* row) const {
    auto const result = _options.useFileTimestamp &&
        row != nullptr &&
        row->fileSize == size &&
        row->lastWriteTime == timestamp;
    return result;
}

DigestDb& DigestAnalyzer::getDigestDb() const {
    auto& result = reinterpret_cast<DigestDb&>(*_db);
    return result;
}


void DigestAnalyzer::updateDbInMemory(const StandaloneClassFileInfo& classFileInfo,
    const ClassFileParser& parser,
    const DigestCol& digest) const {

    auto& digestDb = getDigestDb();
    const auto files = digestDb.getFiles();
    auto& fileRow = static_cast<FileRow&>(*files->allocateRow());
    new(&fileRow) FileRow();

    fileRow.filename = digestDb.getPoolString(classFileInfo.filename);
    fileRow.type = EntryType::Classfile;
    fileRow.lastWriteTime = classFileInfo.timestamp;
    fileRow.fileSize = classFileInfo.size;
    fileRow.digest = digest;
    files->addOrUpdate(&fileRow);

    auto& classfileRow = static_cast<ClassfileRow&>(*digestDb.getClassfiles()->allocateRow());
    new(&classfileRow) ClassfileRow(fileRow);

    classfileRow.size = classFileInfo.size;
    classfileRow.lastWriteTime = classFileInfo.timestamp;
    classfileRow.digest = digest;
    classfileRow.classname = digestDb.getPoolString(parser.getMainClassname());
    digestDb.getClassfiles()->addOrUpdate(&classfileRow);
}
