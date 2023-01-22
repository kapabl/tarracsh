#ifndef TARRACSH_ANALYZER_H
#define TARRACSH_ANALYZER_H

#include <filesystem>
#include <string>
#include <BS_thread_pool.hpp>

#include "Context.h"
#include "../domain/classfile/ClassFileParser.h"
#include "../domain/classfile/StandaloneClassFileInfo.h"
#include "../infrastructure/db/Database.h"
#include "../domain/db/DigestDb.h"
#include "../domain/db/CallGraphDb.h"


using kapa::tarracsh::domain::classfile::ClassFileParser;
using kapa::tarracsh::domain::classfile::StandaloneClassFileInfo;


namespace kapa::tarracsh::app {
class Analyzer {
public:

    explicit Analyzer(Context& config);
    explicit Analyzer(Context& config, std::shared_ptr<infrastructure::db::Database> db);
    
    [[nodiscard]] bool isJarInput() const;
    [[nodiscard]] bool isDirInput() const;
    [[nodiscard]] bool isClassfileInput() const;


    Analyzer(const Analyzer &) = delete;
    Analyzer(const Analyzer &&) = delete;
    Analyzer &operator=(const Analyzer &) = delete;
    Analyzer &operator=(const Analyzer &&) = delete;

    [[nodiscard]] bool isValid() const { return _isValid; }

    ~Analyzer() = default;

    void run();
    void runWithPrint();

private:
    domain::Options _options;
    domain::stats::Results& _results;
    void processJar(const std::string &filename);

    std::shared_ptr<infrastructure::db::Database> _db;

    // BS::thread_pool _fileThreadPool{std::max<unsigned int>(1u, std::thread::hardware_concurrency() * 4 / 5)};
    // BS::thread_pool _fileThreadPool{std::max<unsigned int>(1u, std::thread::hardware_concurrency() * 3 / 4)};
    BS::thread_pool _fileThreadPool{ std::max<unsigned int>(1u, std::thread::hardware_concurrency() - 2) };

    bool _isValid{true};

    [[nodiscard]] bool initDb(infrastructure::db::Database &db) const;
    void processFile(const std::filesystem::directory_entry &dirEntry);
    [[nodiscard]] bool initAnalyzer() const;
    void serverLog(const std::string & string, bool doStdout = false) const;
    void processDirInput();
    void analyzeInput();
    void updateDbs();
    void endAnalysis();

    void parseClassfile(const std::string& filename) const;
    [[nodiscard]] bool isFileUnchanged(uintmax_t size, long long timestamp, const domain::db::digest::FileRow *row) const;
    [[nodiscard]] domain::db::digest::DigestDb &getDigestDb() const;
    [[nodiscard]] domain::db::callgraph::CallGraphDb &getCallGraphDb() const;
    void updateDbInMemory(
        const StandaloneClassFileInfo &classFileInfo, 
        const ClassFileParser &parser,
        const domain::db::digest::columns::DigestCol &digest) const;
    void digestClassfile(const std::string& filename);
    void processClassfile(const std::string& filename);
    void classFileParserDone(ClassFileParser &parser) const;
};
}
#endif
