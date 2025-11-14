#pragma once

#include "app/commands/callgraph/CallGraphAnalyzer.h"

#include <gtest/gtest.h>

#include <chrono>
#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include "app/Context.h"
#include "domain/Options.h"
#include "domain/db/CallGraphDb.h"
#include "domain/stats/Results.h"
#include "infrastructure/log/Log.h"

using namespace kapa::tarracsh;
using namespace kapa::tarracsh::app;
using namespace kapa::tarracsh::app::commands::callgraph;
using namespace kapa::tarracsh::domain;
using namespace kapa::tarracsh::domain::db;
using namespace kapa::tarracsh::domain::db::callgraph;
using namespace kapa::tarracsh::domain::db::table;
using namespace kapa::tarracsh::domain::stats;
using kapa::infrastructure::log::Log;

namespace kapa::tarracsh::app::commands::callgraph::testhooks {
class CallGraphAnalyzerAccessor {
public:
    static void createNamedIndexes(CallGraphAnalyzer &analyzer) { analyzer.createNamedIndexes(); }
    static void createClassnameIndex(CallGraphAnalyzer &analyzer) { analyzer.createClassnameIndex(); }
    static void linkClassRefs(CallGraphAnalyzer &analyzer) { analyzer.linkClassRefs(); }
    static void linkMethodRefs(CallGraphAnalyzer &analyzer) { analyzer.linkMethodRefs(); }
    static void linkFieldRefs(CallGraphAnalyzer &analyzer) { analyzer.linkFieldRefs(); }
    static void linkRefNodes(CallGraphAnalyzer &analyzer) { analyzer.linkRefNodes(); }
    static void endAnalysis(CallGraphAnalyzer &analyzer) { analyzer.endAnalysis(); }
    static Options &options(CallGraphAnalyzer &analyzer) { return analyzer._options; }
    static void doClassFile(CallGraphAnalyzer &analyzer, const std::string &filename) {
        analyzer.doClassFile(filename);
    }
    static void processStandaloneClassFile(CallGraphAnalyzer &analyzer, const std::string &filename) {
        analyzer.processStandaloneClassFile(filename);
    }
    static void processJar(CallGraphAnalyzer &analyzer, const std::string &filename) {
        analyzer.processJar(filename);
    }
    static void waitForWorkerPool(CallGraphAnalyzer &analyzer) { analyzer._fileThreadPool.wait(); }
};
}

namespace callgraph_analyzer_test_support {

struct StubContext : Context {
    StubContext() : options(), results(options) {
        log = std::make_shared<Log>();
        log->forceStdout(true);
        results.log = log;
    }

    Results &getResults() override { return results; }
    Options &getOptions() override { return options; }
    Log &getLog() override { return *log; }

    Options options;
    Results results;
    std::shared_ptr<Log> log;
};

inline std::filesystem::path makeTempDir(const std::string &prefix) {
    const auto base = std::filesystem::temp_directory_path();
    const auto stamp = std::chrono::steady_clock::now().time_since_epoch().count();
    auto dir = base / (prefix + std::to_string(stamp));
    std::filesystem::create_directories(dir);
    return dir;
}

} // namespace callgraph_analyzer_test_support

using callgraph_analyzer_test_support::makeTempDir;
using callgraph_analyzer_test_support::StubContext;

class CallGraphAnalyzerTests : public ::testing::Test {
protected:
    void SetUp() override {
        tempDir = makeTempDir("callgraph-analyzer-tests-");
        context = std::make_unique<StubContext>();
        context->options.outputDir = tempDir.string();
        context->options.isCallGraph = true;
        context->options.callGraph.isDiff = true;
        CallGraphDb::Config config{tempDir.string(), context->log.get()};
        db = CallGraphDb::create(config, true, false);
        ASSERT_NE(db, nullptr);
        analyzer = std::make_unique<CallGraphAnalyzer>(*context, db);
    }

    void TearDown() override {
        if (db) {
            db->stop();
        }
        std::error_code ec;
        std::filesystem::remove_all(tempDir, ec);
    }

    FileRow &addFile(const std::string &filename) {
        auto files = db->getFiles();
        auto &fileRow = reinterpret_cast<FileRow &>(*files->allocateRow());
        new(&fileRow) FileRow();
        fileRow.filename = db->getPoolString(filename);
        fileRow.fileSize = 1;
        fileRow.lastWriteTime = 1;
        files->add(&fileRow);
        return fileRow;
    }

    ClassFileRow &addClass(const std::string &filename, const std::string &classname) {
        auto &fileRow = addFile(filename);
        auto classFiles = db->getClassFiles();
        auto &classRow = reinterpret_cast<ClassFileRow &>(*classFiles->allocateRow());
        new(&classRow) ClassFileRow(fileRow);
        classRow.classname = db->getPoolString(classname);
        classFiles->add(&classRow);
        return classRow;
    }

    MethodRow &addMethod(ClassFileRow &owner,
                         const std::string &name,
                         const std::string &descriptor) {
        auto methods = db->getMethods();
        auto &methodRow = reinterpret_cast<MethodRow &>(*methods->allocateRow());
        new(&methodRow) MethodRow(owner);
        methodRow.name = db->getPoolString(name);
        methodRow.descriptor = db->getPoolString(descriptor);
        methods->add(&methodRow);
        return methodRow;
    }

    FieldRow &addField(ClassFileRow &owner,
                       const std::string &name,
                       const std::string &descriptor) {
        auto fields = db->getFields();
        auto &fieldRow = reinterpret_cast<FieldRow &>(*fields->allocateRow());
        new(&fieldRow) FieldRow(owner);
        fieldRow.name = db->getPoolString(name);
        fieldRow.descriptor = db->getPoolString(descriptor);
        fields->add(&fieldRow);
        return fieldRow;
    }

    ClassRefRow &addClassRef(ClassFileRow &owner, const std::string &targetClass) {
        auto classRefs = db->getClassRefs();
        auto &classRef = reinterpret_cast<ClassRefRow &>(*classRefs->allocateRow());
        new(&classRef) ClassRefRow(owner);
        classRef.name = db->getPoolString(targetClass);
        classRefs->add(&classRef);
        return classRef;
    }

    MethodRefRow &addMethodRef(ClassFileRow &owner,
                               const std::string &targetClass,
                               const std::string &name,
                               const std::string &descriptor) {
        auto methodRefs = db->getMethodRefs();
        auto &methodRef = reinterpret_cast<MethodRefRow &>(*methodRefs->allocateRow());
        new(&methodRef) MethodRefRow(owner);
        methodRef.classname = db->getPoolString(targetClass);
        methodRef.name = db->getPoolString(name);
        methodRef.descriptor = db->getPoolString(descriptor);
        methodRefs->add(&methodRef);
        return methodRef;
    }

    FieldRefRow &addFieldRef(ClassFileRow &owner,
                             const std::string &targetClass,
                             const std::string &name,
                             const std::string &descriptor) {
        auto fieldRefs = db->getFieldRefs();
        auto &fieldRef = reinterpret_cast<FieldRefRow &>(*fieldRefs->allocateRow());
        new(&fieldRef) FieldRefRow(owner);
        fieldRef.classname = db->getPoolString(targetClass);
        fieldRef.name = db->getPoolString(name);
        fieldRef.descriptor = db->getPoolString(descriptor);
        fieldRefs->add(&fieldRef);
        return fieldRef;
    }

    std::filesystem::path tempDir;
    std::unique_ptr<StubContext> context;
    std::shared_ptr<CallGraphDb> db;
    std::unique_ptr<CallGraphAnalyzer> analyzer;
};
