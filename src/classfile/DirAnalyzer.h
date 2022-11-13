
#ifndef DIR_ANALYZER_H
#define DIR_ANALYZER_H

#include <filesystem>
#include <string>
#include <BS_thread_pool.hpp>

#include "Tarracsh.h"
#include "../tables/ClassfilesTable.h"
#include "../tables/FilesTable.h"


namespace org::kapa::tarracsh::dir {
class DirAnalyzer {
public:
    explicit DirAnalyzer(Options options);

    DirAnalyzer(const DirAnalyzer &) = delete;
    DirAnalyzer(const DirAnalyzer &&) = delete;
    DirAnalyzer &operator=(const DirAnalyzer &) = delete;
    DirAnalyzer &operator=(const DirAnalyzer &&) = delete;

    [[nodiscard]] bool isValid() const { return _isValid; }

    ~DirAnalyzer() = default;

    void run();

private:
    Options _options;
    stats::Results _results;
    [[nodiscard]] std::string generateFileTablename(const std::string &name) const;
    [[nodiscard]] std::string generateStringPoolFilename(const std::string& name) const;
    // void processJar(std::filesystem::directory_entry dirEntry);
    void processJar(std::string filename);
    std::shared_ptr<tables::FilesTable> _filesTable;
    std::shared_ptr<tables::StringPool> _stringPool;
    std::shared_ptr<tables::ClassfilesTable> _digestTable;

    BS::thread_pool _jarThreadPool;

    bool _isValid{true};

    bool initializeDigestTables();
    void processFile(const std::filesystem::directory_entry &dirEntry);
    bool initDirAnalysis();
    void analyze();
    void endAnalysis() const;

    static bool isClassfile(std::filesystem::directory_entry const& dirEntry);
    void analyze(std::filesystem::directory_entry const &dirEntry);
    bool isFileUnchanged(uintmax_t size, long long timestamp, const tables::FileRow *row) const;
    void digestClassfile(std::filesystem::directory_entry const &classFileDirEntry);
    void processClassfile(std::filesystem::directory_entry const& dirEntry);
    static bool isJar(std::filesystem::directory_entry const& dirEntry);
};
}
#endif
