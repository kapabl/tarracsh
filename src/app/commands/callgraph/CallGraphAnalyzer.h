#ifndef TARRACSH_CALL_GRAPH_ANALYZER_H
#define TARRACSH_CALL_GRAPH_ANALYZER_H

#include <BS_thread_pool.hpp>
#include <unordered_map>
#include <unordered_set>
#include "app/Analyzer.h"
#include "domain/db/CallGraphDb.h"

using kapa::tarracsh::domain::classfile::ClassFileParser;
using kapa::tarracsh::domain::classfile::StandaloneClassFileInfo;
using kapa::tarracsh::domain::db::callgraph::CallGraphDb;
using kapa::tarracsh::domain::db::table::ClassFileRow;
using kapa::tarracsh::domain::db::table::MethodRow;
using kapa::infrastructure::db::table::column::StringCol;
using kapa::infrastructure::db::table::column::RefCol;


namespace kapa::tarracsh::app::commands::callgraph {
class CallGraphAnalyzer: public Analyzer {
public:

    explicit CallGraphAnalyzer(Context& config);

    explicit CallGraphAnalyzer(Context& config, const std::shared_ptr<CallGraphDb>& db);

    CallGraphAnalyzer(const CallGraphAnalyzer &) = delete;
    CallGraphAnalyzer(const CallGraphAnalyzer &&) = delete;
    CallGraphAnalyzer &operator=(const CallGraphAnalyzer &) = delete;
    CallGraphAnalyzer &operator=(const CallGraphAnalyzer &&) = delete;

    ~CallGraphAnalyzer() override = default;


private:
    void endAnalysis() override;
    void doClassFile(const std::string &filename) override;
    void processJar(const std::string &filename) override;
    void processStandaloneClassFile(const std::string &filename) override;

    void linkRefNodes();
    void linkClassRefs();
    void linkMethodRefs();
    void linkFieldRefs();
    void createNamedIndexes();

    std::shared_ptr<CallGraphDb> _callGraphDb;

    typedef StringCol ClassName, MethodName, FieldName;

    std::unordered_map<ClassName, std::unordered_set<ClassFileRow*>> _classesByName;

    struct MemberInfo {
        RefCol method{RefCol::InvalidRef};
        bool operator==(const MemberInfo& other) const {
            return method.id == other.method.id;
        }
    };

    struct MemberInfoHasher {
        size_t operator()(const MemberInfo& methodInfo) const {
            std::hash<uint64_t> hasher;
            return hasher(methodInfo.method.id);
        }
    };

    typedef std::unordered_set<MemberInfo, MemberInfoHasher> MemberSet;
    typedef std::unordered_map<ClassName, MemberSet> MemberIndexEntry;
    typedef std::unordered_map<StringCol, MemberIndexEntry> MembersIndex;

    MembersIndex _methodsIndex;
    MembersIndex _fieldsIndex;

    void createClassnameIndex();
    void createMemberNameIndex(MembersIndex& membersIndex);
};
}
#endif
