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
using kapa::tarracsh::domain::db::table::MethodRefRow;
using kapa::tarracsh::domain::db::table::FieldRefRow;
using kapa::infrastructure::db::table::column::StringCol;
using kapa::infrastructure::db::table::column::RefCol;
using kapa::infrastructure::db::table::AutoIncrementedRow;


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

    typedef StringCol ClassName, MemberName, Descriptor;

    std::unordered_map<ClassName, std::unordered_set<ClassFileRow*>> _classesByName;

    struct MemberInfo {
        RefCol member{RefCol::InvalidRef};
        bool operator==(const MemberInfo& other) const {
            return member.id == other.member.id;
        }
    };

    struct MemberInfoHasher {
        size_t operator()(const MemberInfo& memberInfo) const {
            std::hash<uint64_t> hasher;
            return hasher(memberInfo.member.id);
        }
    };

    typedef std::unordered_map<Descriptor, std::unordered_set<MemberInfo, MemberInfoHasher>> Descriptor2Member;
    typedef std::unordered_map<MemberName, Descriptor2Member> MemberName2Descriptors;
    typedef std::unordered_map<ClassName, MemberName2Descriptors> MembersIndex;

    MembersIndex _methodsIndex;
    MembersIndex _fieldsIndex;

    void createClassnameIndex();

    template< class TTable, class TRow >
    void createMemberIndex(MembersIndex &membersIndex, TTable memberTable) {
        auto classes = _callGraphDb->getClassFiles();

        memberTable->forEach([classes, &membersIndex](AutoIncrementedRow *pRow) -> void {
            auto* memberRow = reinterpret_cast<TRow *>(pRow);
            auto* pOwnerClassRow = classes->getClassFileRow(memberRow->ownerClass.id);
            auto classname = pOwnerClassRow->classname;

            if (!membersIndex.contains(classname)) {
                membersIndex[classname] = MemberName2Descriptors();
            }

            auto memberName2Descriptors = membersIndex[classname];
            if ( !memberName2Descriptors.contains(memberRow->name)) {
                memberName2Descriptors[memberRow->name] = Descriptor2Member();
            }
            auto descriptor2Member = memberName2Descriptors[memberRow->name];

            if ( !descriptor2Member.contains(memberRow->descriptor)) {
                descriptor2Member[memberRow->descriptor] = std::unordered_set<MemberInfo, MemberInfoHasher>();
            }

            MemberInfo memberInfo;
            memberInfo.member.id = memberRow->id;
            descriptor2Member[memberRow->descriptor].insert(memberInfo);
        });
    }
    
    template< class TMemberRefRow >
    auto getMemberMatches(MembersIndex& memberIndex, const TMemberRefRow &memberRefRow) -> std::vector<RefCol> {
        std::vector<RefCol> result;
        auto classMemberIt = memberIndex.find(memberRefRow.classname);
        if (classMemberIt != memberIndex.end()) {
            auto memberIt = classMemberIt->second.find(memberRefRow.name);
            if (memberIt != classMemberIt->second.end()) {
                auto descriptorIt = memberIt->second.find(memberRefRow.descriptor);
                if (descriptorIt != memberIt->second.end()) {
                    for (auto &memberInfo: descriptorIt->second) {
                        result.push_back(memberInfo.member);
                    }
                }
            }
        }
        return result;
    }
    
    template< class TRefTable, class TRefEdgesTable, class TRefRow, class TEdgeRow >
    void linkMemberRefs(TRefTable refTable, TRefEdgesTable refEdgesTable, MembersIndex& membersIndex) {
        refTable->forEach([this, refEdgesTable, refTable, &membersIndex](AutoIncrementedRow *pRow) -> void {
            auto &refRow = reinterpret_cast<TRefRow &>(*pRow);
            auto matches = getMemberMatches(membersIndex, refRow);

            auto matchCount = matches.size();

            if ( matchCount > 0 ) {
                for( auto& memberInfo: matches) {
                    auto refEdgeRow = reinterpret_cast<TEdgeRow &>(*refEdgesTable->allocateRow());
                    refEdgeRow.ownerClass.id = refRow.ownerClass.id;
                    refEdgeRow.from.id = refRow.id;
                    refEdgeRow.to.id = memberInfo.id;
                    refEdgesTable->add(&refEdgeRow);
                }

                refRow.refCount += matchCount;
                refTable->update(&refRow);
            }
        });
    }
    

};
}
#endif
