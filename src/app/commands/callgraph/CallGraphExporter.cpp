#include "CallGraphExporter.h"

#include <fstream>
#include <sstream>
#include <system_error>
#include <utility>

#include <fmt/format.h>

#include "infrastructure/db/StringPool.h"
#include "domain/db/table/ClassFiles.h"
#include "domain/db/table/ClassRefs.h"
#include "domain/db/table/ClassRefEdges.h"
#include "domain/db/table/MethodRefs.h"
#include "domain/db/table/MethodRefEdges.h"
#include "domain/db/table/Methods.h"
#include "domain/db/table/FieldRefs.h"
#include "domain/db/table/FieldRefEdges.h"
#include "domain/db/table/Fields.h"

using namespace kapa::tarracsh::app::commands::callgraph;
using kapa::tarracsh::domain::db::callgraph::CallGraphDb;
using kapa::tarracsh::domain::db::table::ClassFileRow;
using kapa::tarracsh::domain::db::table::ClassRefRow;
using kapa::tarracsh::domain::db::table::ClassRefEdgeRow;
using kapa::tarracsh::domain::db::table::MethodRefRow;
using kapa::tarracsh::domain::db::table::MethodRefEdgeRow;
using kapa::tarracsh::domain::db::table::MethodRow;
using kapa::tarracsh::domain::db::table::FieldRefRow;
using kapa::tarracsh::domain::db::table::FieldRefEdgeRow;
using kapa::tarracsh::domain::db::table::FieldRow;
using kapa::infrastructure::db::table::AutoIncrementedRow;

namespace {
constexpr const char *kClassEdgeColor = "#4F81BD";
constexpr const char *kMethodEdgeColor = "#70AD47";
constexpr const char *kFieldEdgeColor = "#ED7D31";
constexpr const char *kMissingColor = "#C00000";
}

CallGraphExporter::CallGraphExporter(std::shared_ptr<CallGraphDb> db,
                                     kapa::tarracsh::domain::stats::Results &results,
                                     std::string outputDir)
    : _db(std::move(db)),
      _results(results),
      _outputDir(std::move(outputDir)) {
}

auto CallGraphExporter::exportAll() -> bool {
    if (_db == nullptr) {
        return false;
    }

    buildClassNodes();
    if (_nodes.empty()) {
        return false;
    }

    std::vector<EdgeRecord> edges;
    collectEdges(edges);

    const auto graphDir = ensureOutputDir();
    const auto dotOk = writeDot(graphDir / "callgraph.dot", edges);
    const auto gmlOk = writeGml(graphDir / "callgraph.gml", edges);

    return dotOk && gmlOk;
}

void CallGraphExporter::buildClassNodes() {
    _nodes.clear();
    _classNodeIds.clear();
    _missingNodeIds.clear();

    const auto classFiles = _db->getClassFiles();
    classFiles->forEach([this, classFiles](AutoIncrementedRow *pRow) -> void {
        auto *row = reinterpret_cast<ClassFileRow *>(pRow);
        const auto label = classFiles->getStrongClassname(*row);
        const int nodeId = static_cast<int>(_nodes.size());
        _nodes.push_back({nodeId, label, false});
        _classNodeIds[row->id] = nodeId;
    });
}

auto CallGraphExporter::getClassNodeId(uint64_t classId) -> int {
    const auto it = _classNodeIds.find(classId);
    if (it == _classNodeIds.end()) {
        return -1;
    }
    return it->second;
}

auto CallGraphExporter::getMissingNodeId(const std::string &label) -> int {
    const auto it = _missingNodeIds.find(label);
    if (it != _missingNodeIds.end()) {
        return it->second;
    }

    const int nodeId = static_cast<int>(_nodes.size());
    _nodes.push_back({nodeId, label, true});
    _missingNodeIds[label] = nodeId;
    return nodeId;
}

void CallGraphExporter::collectEdges(std::vector<EdgeRecord> &edges) {
    edges.clear();
    collectClassEdges(edges);
    collectMethodEdges(edges);
    collectFieldEdges(edges);
}

void CallGraphExporter::collectClassEdges(std::vector<EdgeRecord> &edges) {
    const auto classRefEdges = _db->getClassRefEdges();
    const auto classRefs = _db->getClassRefs();
    const auto stringPool = classRefs->getStringPool();

    classRefEdges->forEach([this, &edges](AutoIncrementedRow *pRow) -> void {
        auto &edgeRow = reinterpret_cast<ClassRefEdgeRow &>(*pRow);
        const auto fromId = getClassNodeId(edgeRow.ownerClass.id);
        const auto toId = getClassNodeId(edgeRow.to.id);
        if (fromId >= 0 && toId >= 0) {
            edges.push_back({fromId, toId, "class", kClassEdgeColor, "solid"});
        }
    });

    classRefs->forEach([this, &edges, &stringPool](AutoIncrementedRow *pRow) -> void {
        auto &refRow = reinterpret_cast<ClassRefRow &>(*pRow);
        if (refRow.edgeCount == 0) {
            const auto fromId = getClassNodeId(refRow.ownerClass.id);
            if (fromId < 0) return;
            const auto targetName = stringPool->getCString(refRow.name);
            const auto missingId = getMissingNodeId(fmt::format("{} (missing)", targetName));
            edges.push_back({fromId, missingId, targetName, kMissingColor, "dashed"});
        }
    });
}

void CallGraphExporter::collectMethodEdges(std::vector<EdgeRecord> &edges) {
    const auto methodRefEdges = _db->getMethodRefEdges();
    const auto methodRefs = _db->getMethodRefs();
    const auto methods = _db->getMethods();

    const auto refPool = methodRefs->getStringPool();
    const auto methodPool = methods->getStringPool();

    methodRefEdges->forEach([this, methods, &edges, &methodPool](AutoIncrementedRow *pRow) -> void {
        auto &edgeRow = reinterpret_cast<MethodRefEdgeRow &>(*pRow);
        const auto fromId = getClassNodeId(edgeRow.ownerClass.id);
        auto *methodRow = reinterpret_cast<MethodRow *>(methods->getRow(edgeRow.to.id));
        if (fromId < 0 || methodRow == nullptr) return;
        const auto toId = getClassNodeId(methodRow->ownerClass.id);
        if (toId < 0) return;

        const auto descriptor = methodPool->getCString(methodRow->descriptor);
        const auto label = fmt::format("{} {}", methods->getStrongMemberName(*methodRow), descriptor);
        edges.push_back({fromId, toId, label, kMethodEdgeColor, "solid"});
    });

    methodRefs->forEach([this, &edges, &refPool](AutoIncrementedRow *pRow) -> void {
        auto &refRow = reinterpret_cast<MethodRefRow &>(*pRow);
        if (refRow.edgeCount == 0) {
            const auto fromId = getClassNodeId(refRow.ownerClass.id);
            if (fromId < 0) return;
            const auto owner = refPool->getCString(refRow.classname);
            const auto methodName = refPool->getCString(refRow.name);
            const auto descriptor = refPool->getCString(refRow.descriptor);
            const auto missingKey = fmt::format("{}#{} {}", owner, methodName, descriptor);
            const auto label = fmt::format("{} (missing)", missingKey);
            const auto missingId = getMissingNodeId(missingKey);
            edges.push_back({fromId, missingId, label, kMissingColor, "dashed"});
        }
    });
}

void CallGraphExporter::collectFieldEdges(std::vector<EdgeRecord> &edges) {
    const auto fieldRefEdges = _db->getFieldRefEdges();
    const auto fieldRefs = _db->getFieldRefs();
    const auto fields = _db->getFields();

    const auto refPool = fieldRefs->getStringPool();
    const auto fieldPool = fields->getStringPool();

    fieldRefEdges->forEach([this, fields, &edges, &fieldPool](AutoIncrementedRow *pRow) -> void {
        auto &edgeRow = reinterpret_cast<FieldRefEdgeRow &>(*pRow);
        const auto fromId = getClassNodeId(edgeRow.ownerClass.id);
        auto *fieldRow = reinterpret_cast<FieldRow *>(fields->getRow(edgeRow.to.id));
        if (fromId < 0 || fieldRow == nullptr) return;
        const auto toId = getClassNodeId(fieldRow->ownerClass.id);
        if (toId < 0) return;

        const auto descriptor = fieldPool->getCString(fieldRow->descriptor);
        const auto label = fmt::format("{} {}", fields->getStrongMemberName(*fieldRow), descriptor);
        edges.push_back({fromId, toId, label, kFieldEdgeColor, "solid"});
    });

    fieldRefs->forEach([this, &edges, &refPool](AutoIncrementedRow *pRow) -> void {
        auto &refRow = reinterpret_cast<FieldRefRow &>(*pRow);
        if (refRow.edgeCount == 0) {
            const auto fromId = getClassNodeId(refRow.ownerClass.id);
            if (fromId < 0) return;
            const auto owner = refPool->getCString(refRow.classname);
            const auto fieldName = refPool->getCString(refRow.name);
            const auto descriptor = refPool->getCString(refRow.descriptor);
            const auto missingKey = fmt::format("{}#{} {}", owner, fieldName, descriptor);
            const auto label = fmt::format("{} (missing)", missingKey);
            const auto missingId = getMissingNodeId(missingKey);
            edges.push_back({fromId, missingId, label, kMissingColor, "dashed"});
        }
    });
}

auto CallGraphExporter::escapeForDot(const std::string &value) -> std::string {
    std::string result;
    result.reserve(value.size());
    for (const auto ch : value) {
        if (ch == '"') {
            result += "\\\"";
        } else {
            result += ch;
        }
    }
    return result;
}

auto CallGraphExporter::escapeForGml(const std::string &value) -> std::string {
    std::string result;
    result.reserve(value.size());
    for (const auto ch : value) {
        if (ch == '"') {
            result += "\\\"";
        } else if (ch == '\\') {
            result += "\\\\";
        } else {
            result += ch;
        }
    }
    return result;
}

auto CallGraphExporter::ensureOutputDir() -> std::filesystem::path {
    auto base = _outputDir.empty()
                ? std::filesystem::current_path()
                : _outputDir;
    auto dir = base / "callgraph";
    std::error_code ec;
    std::filesystem::create_directories(dir, ec);
    if (ec) {
        _results.log->writeln(fmt::format("CallGraphExporter: cannot create {}", dir.string()), true);
    }
    return dir;
}

auto CallGraphExporter::writeDot(const std::filesystem::path &path, const std::vector<EdgeRecord> &edges) -> bool {
    std::ofstream out(path);
    if (!out.good()) {
        _results.log->writeln(fmt::format("CallGraphExporter: failed to write {}", path.string()), true);
        return false;
    }

    out << "digraph CallGraph {\n";
    out << "  rankdir=LR;\n";
    out << "  node [shape=box, fontname=\"Helvetica\"];\n";

    for (const auto &node : _nodes) {
        const auto color = node.isMissing ? kMissingColor : "#333333";
        out << fmt::format("  n{} [label=\"{}\", color=\"{}\"];\n",
                           node.id,
                           escapeForDot(node.label),
                           color);
    }

    for (const auto &edge : edges) {
        out << fmt::format("  n{} -> n{} [label=\"{}\", color=\"{}\", style=\"{}\"];\n",
                           edge.from,
                           edge.to,
                           escapeForDot(edge.label),
                           edge.color,
                           edge.style);
    }

    out << "}\n";
    out.flush();
    _results.log->writeln(fmt::format("Call graph DOT written to {}", path.string()), true);
    return true;
}

auto CallGraphExporter::writeGml(const std::filesystem::path &path, const std::vector<EdgeRecord> &edges) -> bool {
    std::ofstream out(path);
    if (!out.good()) {
        _results.log->writeln(fmt::format("CallGraphExporter: failed to write {}", path.string()), true);
        return false;
    }

    out << "graph [\n";
    out << "  directed 1\n";

    for (const auto &node : _nodes) {
        const auto fill = node.isMissing ? kMissingColor : "#FFFFFF";
        out << "  node [\n";
        out << fmt::format("    id {}\n", node.id);
        out << fmt::format("    label \"{}\"\n", escapeForGml(node.label));
        out << "    graphics [\n";
        out << fmt::format("      fill \"{}\"\n", fill);
        out << "    ]\n";
        out << "  ]\n";
    }

    for (const auto &edge : edges) {
        out << "  edge [\n";
        out << fmt::format("    source {}\n", edge.from);
        out << fmt::format("    target {}\n", edge.to);
        out << fmt::format("    label \"{}\"\n", escapeForGml(edge.label));
        out << "    graphics [\n";
        out << fmt::format("      fill \"{}\"\n", edge.color);
        if (edge.style == "dashed") {
            out << "      pattern \"dashed\"\n";
        }
        out << "    ]\n";
        out << "  ]\n";
    }

    out << "]\n";
    out.flush();
    _results.log->writeln(fmt::format("Call graph GML written to {}", path.string()), true);
    return true;
}
