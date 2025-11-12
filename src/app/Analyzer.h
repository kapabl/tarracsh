#ifndef TARRACSH_ANALYZER_H
#define TARRACSH_ANALYZER_H

#include <filesystem>
#include <string>
#include <BS_thread_pool.hpp>

#include "Context.h"
#include "domain/classfile/ClassFileParser.h"
#include "domain/classfile/StandaloneClassFileInfo.h"
#include "infrastructure/db/Database.h"
#include "infrastructure/db/columns/Columns.h"

using kapa::tarracsh::domain::classfile::ClassFileParser;
using kapa::tarracsh::domain::classfile::StandaloneClassFileInfo;


namespace kapa::tarracsh::app {
class Analyzer {
public:

    explicit Analyzer(Context& config);
    explicit Analyzer(Context& config, std::shared_ptr<infrastructure::db::Database> db);
    
    Analyzer(const Analyzer &) = delete;
    Analyzer(const Analyzer &&) = delete;
    Analyzer &operator=(const Analyzer &) = delete;
    Analyzer &operator=(const Analyzer &&) = delete;

    [[nodiscard]] bool isValid() const { return _isValid; }

    virtual ~Analyzer() = default;

    void run();
    void runWithPrint();
    virtual void printAllResults();

protected:
    domain::Options _options;
    domain::SubCommandOptions _inputOptions;
    domain::stats::Results& _results;


    std::shared_ptr<infrastructure::db::Database> _db;

    // BS::thread_pool _fileThreadPool{std::max<unsigned int>(1u, std::thread::hardware_concurrency() * 4 / 5)};
    // BS::thread_pool _fileThreadPool{std::max<unsigned int>(1u, std::thread::hardware_concurrency() * 3 / 4)};
    BS::thread_pool<> _fileThreadPool{ std::max<unsigned int>(1u, std::thread::hardware_concurrency() - 2) };

    bool _isValid{true};

    void processFile(const std::filesystem::directory_entry &dirEntry);
    [[nodiscard]] virtual bool initAnalyzer();
    void serverLog(const std::string & string, bool doStdout = false);

    virtual void processStandaloneClassFile(const std::string& filename);
    virtual void processDirInput();
    virtual void processJar(const std::string& filename);
    virtual void endAnalysis();
    virtual void doClassFile(const std::string& filename);

    virtual void analyzeInput();
    void parseClassFile(const std::string& filename) const;
    void classFileParserDone(ClassFileParser &parser) const;

    std::thread _printProgressThread;
    std::atomic<bool> _isAnalazingInputs{false};

};
}
#endif
