#ifndef DIR_ANALYZER_H
#define DIR_ANALYZER_H

#include <filesystem>
#include <string>
#include <BS_thread_pool.hpp>

#include "../app/Tarracsh.h"
#include "../tables/DigestDb.h"
#include "../tables/CallGraphDb.h"
#include "../classfile/ClassFileInfo.h"
#include "../tables/FilesTable.h"


namespace org::kapa::tarracsh::dir {
class Analyzer {
public:
    bool isJarInput() const;
    bool isDirInput() const;
    bool isClassfileInput() const;
    explicit Analyzer(Options options, stats::Results& results );

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

    db::DigestDb _digestDb;
    db::CallGraphDb _callGraphDb;

    BS::thread_pool _fileThreadPool{std::max<unsigned int>(1u, std::thread::hardware_concurrency() * 3 / 4)};
    //BS::thread_pool _fileThreadPool{ std::max<unsigned int>(1u, std::thread::hardware_concurrency() - 2) };

    bool _isValid{true};

    bool initDb(db::Database &db);
    void processFile(const std::filesystem::directory_entry &dirEntry);
    bool initAnalyzer();
    void processDir();
    void analyze();
    void updateDbs();
    void endAnalysis();

    void analyze(const std::string& filename) const;
    bool isFileUnchanged(uintmax_t size, long long timestamp, const db::tables::FileRow *row) const;
    void updateDbInMemory(const ClassFileInfo &classFileInfo, const ClassFileAnalyzer &classFileAnalyzer,
                          const db::tables::columns::DigestCol &digest);
    void digestClassfile(const std::string& filename);
    void processClassfile(const std::string& filename);
};
}
#endif
