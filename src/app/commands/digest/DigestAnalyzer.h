#ifndef TARRACSH_DIGEST_ANALYZER_H
#define TARRACSH_DIGEST_ANALYZER_H

#include <BS_thread_pool.hpp>

#include "app/Analyzer.h"
#include "domain/db/table/Files.h"
#include "domain/db/DigestDb.h"


namespace kapa::tarracsh::app::commands::digest {
class DigestAnalyzer final: public Analyzer {
public:

    explicit DigestAnalyzer(Context& config);

    explicit DigestAnalyzer(Context& config, std::shared_ptr<infrastructure::db::Database> db);
    
    DigestAnalyzer(const DigestAnalyzer &) = delete;
    DigestAnalyzer(const DigestAnalyzer &&) = delete;
    DigestAnalyzer &operator=(const DigestAnalyzer &) = delete;
    DigestAnalyzer &operator=(const DigestAnalyzer &&) = delete;

    ~DigestAnalyzer() override = default;
private:
    void endAnalysis() override;
    void analyzeStandaloneClassfile(const std::string& filename) override;
    void processJar(const std::string &filename) override;
    void digestClassfile(const std::string &filename) const;
    bool isFileUnchanged(const uintmax_t size, const long long timestamp, const domain::db::table::FileRow *row) const;
    domain::db::digest::DigestDb &getDigestDb() const;
    void updateDbInMemory(const StandaloneClassFileInfo &classFileInfo, const ClassFileParser &parser,
                          const infrastructure::db::table::column::DigestCol &digest) const;

};
}
#endif
