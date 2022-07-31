
#ifndef DIR_ANALYZER_H
#define DIR_ANALYZER_H

#include <filesystem>
#include <string>

#include "Tarracsh.h"


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

    bool _isValid{true};

    static bool isClassfile(std::filesystem::directory_entry const& dirEntry);
    void processClassfile(std::filesystem::directory_entry const& dirEntry);
    static bool isJar(std::filesystem::directory_entry const& dirEntry);
};
}
#endif
