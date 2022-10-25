#ifndef JAR_DIGESTER_H
#define JAR_DIGESTER_H
#include "../Tarracsh.h"
#include "../Stats.h"

#include <ranges>
#include "JarEntry.h"

#include "../tables/ClassfileDigestTable.h"


namespace org::kapa::tarracsh::jar {
class JarDigester {
public:
    explicit JarDigester(Options options, Results &results, std::shared_ptr<tables::ClassfileDigestTable> digestTable);

    JarDigester(const JarDigester &) = delete;
    JarDigester(const JarDigester &&) = delete;
    JarDigester &operator=(const JarDigester &) = delete;
    JarDigester &operator=(const JarDigester &&) = delete;

    [[nodiscard]] bool isValid() const { return _isValid; }

    ~JarDigester() = default;
    void run();

    unsigned int getClassfileCount() const;


private:
    std::shared_ptr<tables::ClassfileDigestTable> _digestTable;
    Results &_results;
    Options _options;
    bool _isValid{true};
    unsigned int _classfileCount{0};

    [[nodiscard]] tables::ClassfileDigestRow* getRow(const JarEntry& jarEntry) const;
    [[nodiscard]] std::optional<tables::MD5> parseEntry(const JarEntry& jarEntry, const tables::ClassfileDigestRow* row) const;
    static bool isUnchanged(const JarEntry& jarEntry, const tables::ClassfileDigestRow* row);
    [[nodiscard]] std::optional<tables::MD5> digestEntry(const JarEntry& jarEntry) const;
    [[nodiscard]] bool isFileUnchanged(uintmax_t size, long long timestamp, const tables::ClassfileDigestRow* row) const;
    std::optional<tables::MD5> getPublicDigest();
    void digest();

};
}
#endif
