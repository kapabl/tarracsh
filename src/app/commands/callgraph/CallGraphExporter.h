#ifndef TARRACSH_CALL_GRAPH_EXPORTER_H
#define TARRACSH_CALL_GRAPH_EXPORTER_H

#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "domain/db/CallGraphDb.h"
#include "domain/stats/Results.h"

namespace kapa::tarracsh::app::commands::callgraph {

class CallGraphExporter {
public:
    CallGraphExporter(std::shared_ptr<domain::db::callgraph::CallGraphDb> db,
                      domain::stats::Results &results,
                      std::string outputDir,
                      bool exportDot,
                      bool exportGml);

    auto run() -> bool;

private:
    struct NodeRecord {
        int id;
        std::string label;
        bool isMissing;
    };

    struct EdgeRecord {
        int from;
        int to;
        std::string label;
        std::string color;
        std::string style;
    };

    std::shared_ptr<domain::db::callgraph::CallGraphDb> _db;
    domain::stats::Results &_results;
    std::filesystem::path _outputDir;
    bool _exportDot{false};
    bool _exportGml{false};

    std::vector<NodeRecord> _nodes;
    std::unordered_map<uint64_t, int> _classNodeIds;
    std::unordered_map<std::string, int> _missingNodeIds;

    void buildClassNodes();
    auto getClassNodeId(uint64_t classId) -> int;
    auto getMissingNodeId(const std::string &label) -> int;
    void collectEdges(std::vector<EdgeRecord> &edges);

    void collectClassEdges(std::vector<EdgeRecord> &edges);
    void collectMethodEdges(std::vector<EdgeRecord> &edges);
    void collectFieldEdges(std::vector<EdgeRecord> &edges);
    static auto escapeForDot(const std::string &value) -> std::string;
    static auto escapeForGml(const std::string &value) -> std::string;

    auto ensureOutputDir() -> std::filesystem::path;
    auto writeDot(const std::filesystem::path &path, const std::vector<EdgeRecord> &edges) -> bool;
    auto writeGml(const std::filesystem::path &path, const std::vector<EdgeRecord> &edges) -> bool;
};

}
#endif
