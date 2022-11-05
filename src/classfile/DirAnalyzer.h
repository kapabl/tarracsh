
#ifndef DIR_ANALYZER_H
#define DIR_ANALYZER_H

#include <filesystem>
#include <string>

#include "Tarracsh.h"
#include "tables/DigestTable.h"


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
    Results _results;
    [[nodiscard]] std::string generateDigestTablename() const;
    void processJar(std::filesystem::directory_entry const &dirEntry);
    std::shared_ptr<tables::DigestTable> _digestTable;

    bool _isValid{true};

    bool initializeDigestTables();
    void processFile(std::filesystem::directory_entry const &dirEntry);
    bool initDirAnalysis();
    void analyze();
    void endAnalysis() const;

    static bool isClassfile(std::filesystem::directory_entry const& dirEntry);
    void analyze(std::filesystem::directory_entry const &dirEntry);
    bool isFileUnchanged(uintmax_t size, long long timestamp, const tables::DigestRow *row) const;
    void digestClassfile(std::filesystem::directory_entry const &classFileDirEntry);
    void processClassfile(std::filesystem::directory_entry const& dirEntry);
    static bool isJar(std::filesystem::directory_entry const& dirEntry);
};
}
#endif
