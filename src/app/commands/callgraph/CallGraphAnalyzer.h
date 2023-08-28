#ifndef TARRACSH_CALL_GRAPH_ANALYZER_H
#define TARRACSH_CALL_GRAPH_ANALYZER_H

#include <BS_thread_pool.hpp>

#include "app/Analyzer.h"
#include "domain/db/CallGraphDb.h"

using kapa::tarracsh::domain::classfile::ClassFileParser;
using kapa::tarracsh::domain::classfile::StandaloneClassFileInfo;


namespace kapa::tarracsh::app::commands::callgraph {
class CallGraphAnalyzer: public Analyzer {
public:

    explicit CallGraphAnalyzer(Context& config);

    explicit CallGraphAnalyzer(Context& config, const std::shared_ptr<infrastructure::db::Database>& db);
    
    CallGraphAnalyzer(const CallGraphAnalyzer &) = delete;
    CallGraphAnalyzer(const CallGraphAnalyzer &&) = delete;
    CallGraphAnalyzer &operator=(const CallGraphAnalyzer &) = delete;
    CallGraphAnalyzer &operator=(const CallGraphAnalyzer &&) = delete;

    ~CallGraphAnalyzer() override = default;


private:
    void endAnalysis() override;
    void doClassfile(const std::string &filename) override;
    void processJar(const std::string &filename) override;
    void processStandaloneClassfile(const std::string &filename) override;
    domain::db::callgraph::CallGraphDb &getCallGraphDb() const;


};
}
#endif
