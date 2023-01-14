#ifndef TARRACSH_ANALYZER_H
#define TARRACSH_ANALYZER_H

#include <filesystem>
#include <string>
#include <BS_thread_pool.hpp>

#include "Config.h"
#include "../db/DigestDb.h"
#include "../db/CallGraphDb.h"
#include "../domain/classfile/ClassFileAnalyzer.h"
#include "../domain/classfile/ClassFileInfo.h"
#include "../infrastructure/db/Database.h"
#include "../db/table/FilesTable.h"


namespace kapa::tarracsh::app {
class Analyzer {
public:

    explicit Analyzer(Config& config);
    
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

private:
    Options _options;
    stats::Results& _results;
    void processJar(const std::string &filename);

    db::digest::DigestDb _digestDb;
    db::callgraph::CallGraphDb _callGraphDb;

    BS::thread_pool _fileThreadPool{std::max<unsigned int>(1u, std::thread::hardware_concurrency() * 4 / 5)};
    // BS::thread_pool _fileThreadPool{std::max<unsigned int>(1u, std::thread::hardware_concurrency() * 3 / 4)};
    //BS::thread_pool _fileThreadPool{ std::max<unsigned int>(1u, std::thread::hardware_concurrency() - 2) };

    bool _isValid{true};

    bool initDb(infrastructure::db::Database &db) const;
    void processFile(const std::filesystem::directory_entry &dirEntry);
    bool initAnalyzer();
    void processDirInput();
    void processJarInput();
    void processClassfileInput();
    void analyzeInput();
    void updateDbs();
    void endAnalysis();

    void analyzeClassfile(const std::string& filename) const;
    bool isFileUnchanged(uintmax_t size, long long timestamp, const db::digest::FileRow *row) const;
    void updateDbInMemory(const ClassFileInfo &classFileInfo, const ClassFileAnalyzer &classFileAnalyzer,
                          const db::digest::columns::DigestCol &digest);
    void digestClassfile(const std::string& filename);
    void processClassfile(const std::string& filename);
};
}
#endif
