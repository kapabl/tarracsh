
#ifndef DIR_ANALYZER_H
#define DIR_ANALYZER_H

#include <filesystem>
#include <string>

#include "Tarracsh.h"
#include "tables/PublicMd5Table.h"


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
    [[nodiscard]] std::string generateShaTablename() const;
    void processJarFile(std::filesystem::directory_entry const &dirEntry);
    std::shared_ptr<tables::PublicMd5Table> _shaTable;

    bool _isValid{true};

    bool initializePublicShaTable();
    void printDirEntryStats();
    void processDirEntry(std::filesystem::directory_entry const &dirEntry);
    bool initDirAnalysis();
    void analyze();
    void endDirAnalysis() const;

    static bool isClassfile(std::filesystem::directory_entry const& dirEntry);
    void regularProcess(std::filesystem::directory_entry const &dirEntry);
    void publicShaProcess(std::filesystem::directory_entry const &dirEntry);
    void processClassfile(std::filesystem::directory_entry const& dirEntry);
    static bool isJar(std::filesystem::directory_entry const& dirEntry);
};
}
#endif
