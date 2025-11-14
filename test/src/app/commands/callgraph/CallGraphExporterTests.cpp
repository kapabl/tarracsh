#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <string>

#include "app/commands/callgraph/CallGraphExporter.h"
#include "domain/Options.h"
#include "domain/db/CallGraphDb.h"
#include "domain/stats/Results.h"
#include "infrastructure/log/Log.h"

using namespace kapa::tarracsh;
using namespace kapa::tarracsh::app::commands::callgraph;
using namespace kapa::tarracsh::domain;
using namespace kapa::tarracsh::domain::db::table;
using namespace kapa::tarracsh::domain::stats;
using kapa::tarracsh::domain::db::callgraph::CallGraphDb;
using kapa::infrastructure::db::table::column::StringCol;
using kapa::infrastructure::log::Log;

namespace {

std::string uniqueSuffix() {
    static std::atomic<uint64_t> counter{0};
    const auto now = std::chrono::steady_clock::now().time_since_epoch().count();
    return std::to_string(now) + "-" + std::to_string(counter.fetch_add(1));
}

std::string readFile(const std::filesystem::path &path) {
    std::ifstream in(path);
    return std::string(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
}

class CallGraphExporterFixture : public ::testing::Test {
protected:
    void SetUp() override {
        tempDir = std::filesystem::temp_directory_path() / "call-graph-exporter-tests" / uniqueSuffix();
        std::filesystem::create_directories(tempDir);

        log = std::make_shared<Log>();
        log->forceStdout(true);

        CallGraphDb::Config config{tempDir.string(), log.get()};
        db = std::make_shared<CallGraphDb>(config, false);
        db->init();

        options.outputDir = tempDir.string();
        results = std::make_unique<Results>(options);
        results->log = log;
    }

    void TearDown() override {
        if (db) {
            db->stop();
        }
        std::error_code ec;
        std::filesystem::remove_all(tempDir, ec);
    }

    StringCol intern(const std::string &value) {
        return db->getStringPool()->add(value);
    }

    FileRow *addFile(const std::string &filename) {
        auto files = db->getFiles();
        auto *row = reinterpret_cast<FileRow *>(files->allocateRow());
        row->filename = intern(filename);
        files->add(row);
        return row;
    }

    ClassFileRow *addClass(FileRow *fileRow, const std::string &className) {
        auto classFiles = db->getClassFiles();
        auto *row = reinterpret_cast<ClassFileRow *>(classFiles->allocateRow());
        row->file.id = fileRow->id;
        row->classname = intern(className);
        classFiles->add(row);
        return row;
    }

    MethodRow *addMethod(ClassFileRow *owner, const std::string &name, const std::string &descriptor) {
        auto methods = db->getMethods();
        auto *row = reinterpret_cast<MethodRow *>(methods->allocateRow());
        row->ownerClass.id = owner->id;
        row->name = intern(name);
        row->descriptor = intern(descriptor);
        methods->add(row);
        return row;
    }

    FieldRow *addField(ClassFileRow *owner, const std::string &name, const std::string &descriptor) {
        auto fields = db->getFields();
        auto *row = reinterpret_cast<FieldRow *>(fields->allocateRow());
        row->ownerClass.id = owner->id;
        row->name = intern(name);
        row->descriptor = intern(descriptor);
        fields->add(row);
        return row;
    }

    ClassRefRow *addClassRef(ClassFileRow *owner, const std::string &targetName, uint64_t edgeCount) {
        auto refs = db->getClassRefs();
        auto *row = reinterpret_cast<ClassRefRow *>(refs->allocateRow());
        row->ownerClass.id = owner->id;
        row->name = intern(targetName);
        row->edgeCount = edgeCount;
        refs->add(row);
        return row;
    }

    MethodRefRow *addMethodRef(ClassFileRow *owner,
                               const std::string &targetClass,
                               const std::string &methodName,
                               const std::string &descriptor,
                               uint64_t edgeCount) {
        auto refs = db->getMethodRefs();
        auto *row = reinterpret_cast<MethodRefRow *>(refs->allocateRow());
        row->ownerClass.id = owner->id;
        row->classname = intern(targetClass);
        row->name = intern(methodName);
        row->descriptor = intern(descriptor);
        row->edgeCount = edgeCount;
        refs->add(row);
        return row;
    }

    FieldRefRow *addFieldRef(ClassFileRow *owner,
                             const std::string &targetClass,
                             const std::string &fieldName,
                             const std::string &descriptor,
                             uint64_t edgeCount) {
        auto refs = db->getFieldRefs();
        auto *row = reinterpret_cast<FieldRefRow *>(refs->allocateRow());
        row->ownerClass.id = owner->id;
        row->classname = intern(targetClass);
        row->name = intern(fieldName);
        row->descriptor = intern(descriptor);
        row->edgeCount = edgeCount;
        refs->add(row);
        return row;
    }

    void linkClassEdge(ClassRefRow *ref, ClassFileRow *target) {
        auto edges = db->getClassRefEdges();
        auto *row = reinterpret_cast<ClassRefEdgeRow *>(edges->allocateRow());
        row->ownerClass.id = ref->ownerClass.id;
        row->from.id = ref->id;
        row->to.id = target->id;
        edges->add(row);
    }

    void linkMethodEdge(MethodRefRow *ref, MethodRow *target) {
        auto edges = db->getMethodRefEdges();
        auto *row = reinterpret_cast<MethodRefEdgeRow *>(edges->allocateRow());
        row->ownerClass.id = ref->ownerClass.id;
        row->from.id = ref->id;
        row->to.id = target->id;
        edges->add(row);
    }

    void linkFieldEdge(FieldRefRow *ref, FieldRow *target) {
        auto edges = db->getFieldRefEdges();
        auto *row = reinterpret_cast<FieldRefEdgeRow *>(edges->allocateRow());
        row->ownerClass.id = ref->ownerClass.id;
        row->from.id = ref->id;
        row->to.id = target->id;
        edges->add(row);
    }

    std::shared_ptr<CallGraphDb> db;
    std::shared_ptr<Log> log;
    Options options{};
    std::unique_ptr<Results> results;
    std::filesystem::path tempDir;
};

} // namespace

TEST(CallGraphExporterTests, ReturnsFalseWithoutDatabase) {
    Options options;
    options.outputDir = (std::filesystem::temp_directory_path() / "call-graph-exporter-tests" / uniqueSuffix()).string();
    Results results(options);
    results.log = std::make_shared<Log>();
    results.log->forceStdout(true);

    CallGraphExporter exporter(nullptr, results, options.outputDir, true, true);
    EXPECT_FALSE(exporter.run());
}

TEST_F(CallGraphExporterFixture, ReturnsFalseWhenNoClassesPresent) {
    CallGraphExporter exporter(db, *results, tempDir.string(), true, false);
    EXPECT_FALSE(exporter.run());
}

TEST_F(CallGraphExporterFixture, WritesDotAndGmlWithExistingAndMissingReferences) {
    auto *fileA = addFile((tempDir / "a.jar").string());
    auto *fileB = addFile((tempDir / "b.jar").string());

    auto *classA = addClass(fileA, "com/example/A");
    auto *classB = addClass(fileB, "com/example/B");

    // Class edges: one solid edge plus a missing target.
    auto *classRef = addClassRef(classA, "com/example/B", 1);
    linkClassEdge(classRef, classB);
    addClassRef(classA, "com/example/MissingClass", 0);

    // Method edges with descriptors that exercise escaping.
    const std::string descriptor = "\"desc\\value\"";
    auto *method = addMethod(classB, "doWork", descriptor);
    auto *methodRef = addMethodRef(classA, "com/example/B", "doWork", descriptor, 1);
    linkMethodEdge(methodRef, method);

    // Missing refs produce dashed edges to synthetic nodes.
    addMethodRef(classA, "com/example/Unknown", "missingMethod", "()V", 0);

    // Field edges with own missing entry.
    auto *field = addField(classB, "value", "Lpkg/Field;\\path");
    auto *fieldRef = addFieldRef(classA, "com/example/B", "value", "Lpkg/Field;\\path", 1);
    linkFieldEdge(fieldRef, field);
    addFieldRef(classA, "com/example/Unknown", "lostField", "I", 0);

    CallGraphExporter exporter(db, *results, tempDir.string(), true, true);
    ASSERT_TRUE(exporter.run());

    const auto graphDir = tempDir / "callgraph";
    const auto dotPath = graphDir / "callgraph.dot";
    const auto gmlPath = graphDir / "callgraph.gml";
    ASSERT_TRUE(std::filesystem::exists(dotPath));
    ASSERT_TRUE(std::filesystem::exists(gmlPath));

    const auto dot = readFile(dotPath);
    EXPECT_NE(dot.find("class"), std::string::npos);
    EXPECT_NE(dot.find("missingMethod ()V (missing)"), std::string::npos);
    EXPECT_NE(dot.find("\\\"desc\\value\\\""), std::string::npos);

    const auto gml = readFile(gmlPath);
    EXPECT_NE(gml.find("pattern \"dashed\""), std::string::npos);
    EXPECT_NE(gml.find("desc\\\\value"), std::string::npos);
    EXPECT_NE(gml.find("lostField"), std::string::npos);
}
