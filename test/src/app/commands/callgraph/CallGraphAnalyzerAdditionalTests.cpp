#include "CallGraphAnalyzerTestBase.h"

#include <fstream>

#include "domain/Utils.h"
#include "domain/stats/Report.h"
#include "infrastructure/filesystem/Utils.h"

using testhooks::CallGraphAnalyzerAccessor;

TEST_F(CallGraphAnalyzerTests, CreateClassnameIndexHandlesRepeatedCalls) {
    auto &classA = addClass("A.class", "com/foo/A");
    auto &classB = addClass("B.class", "com/foo/B");
    (void)classB;

    CallGraphAnalyzerAccessor::createClassnameIndex(*analyzer);
    // Re-invoking should hit the branch that appends to existing buckets.
    CallGraphAnalyzerAccessor::createClassnameIndex(*analyzer);

    auto &classRef = addClassRef(classA, "com/foo/B");
    CallGraphAnalyzerAccessor::linkClassRefs(*analyzer);
    auto classRefs = db->getClassRefs();
    auto updatedRef = reinterpret_cast<ClassRefRow *>(classRefs->getRow(classRef.id));
    ASSERT_NE(updatedRef, nullptr);
    EXPECT_EQ(updatedRef->edgeCount, 1u);
}

TEST_F(CallGraphAnalyzerTests, DoClassFileParsesRealClassfileAndStoresRows) {
    const auto classFile = std::filesystem::path("test-subjects/org/kapa/reti/ast/AstNode.class");
    ASSERT_TRUE(std::filesystem::exists(classFile));

    const auto initialParsed = context->results.standaloneClassfiles.parsedCount.load();
    CallGraphAnalyzerAccessor::doClassFile(*analyzer, classFile.string());

    EXPECT_EQ(context->results.standaloneClassfiles.parsedCount.load(), initialParsed + 1);
    EXPECT_GE(db->getFiles()->size(), 1u);
    EXPECT_GE(db->getClassFiles()->size(), 1u);
}

TEST_F(CallGraphAnalyzerTests, DoClassFileSkipsProcessingWhenFileIsUnchanged) {
    const auto classFile = std::filesystem::path("test-subjects/org/kapa/reti/ast/AstNode.class");
    ASSERT_TRUE(std::filesystem::exists(classFile));

    const auto size = std::filesystem::file_size(classFile);
    const auto timestamp = kapa::infrastructure::filesystem::utils::getLastWriteTimestamp(classFile.string());

    auto files = db->getFiles();
    auto &fileRow = reinterpret_cast<FileRow &>(*files->allocateRow());
    new(&fileRow) FileRow();
    fileRow.filename = db->getPoolString(classFile.string());
    fileRow.fileSize = size;
    fileRow.lastWriteTime = timestamp;
    fileRow.classfileCount = 1;
    files->add(&fileRow);

    auto classFiles = db->getClassFiles();
    auto &classRow = reinterpret_cast<ClassFileRow &>(*classFiles->allocateRow());
    new(&classRow) ClassFileRow(fileRow);
    classRow.classname = db->getPoolString("org/kapa/reti/ast/AstNode");
    classFiles->add(&classRow);

    CallGraphAnalyzerAccessor::options(*analyzer).useFileTimestamp = true;

    const auto parsedBefore = context->results.standaloneClassfiles.parsedCount.load();
    CallGraphAnalyzerAccessor::doClassFile(*analyzer, classFile.string());
    EXPECT_EQ(context->results.standaloneClassfiles.parsedCount.load(), parsedBefore);
    EXPECT_EQ(db->getFiles()->size(), 1u);
    EXPECT_EQ(db->getClassFiles()->size(), 1u);
}

TEST_F(CallGraphAnalyzerTests, DoClassFileRecordsFailureWhenParserFails) {
    const auto badFile = tempDir / "invalid.class";
    {
        std::ofstream out(badFile, std::ios::binary);
        out << "not-a-classfile";
    }

    const auto errorsBefore = context->results.standaloneClassfiles.errors.load();
    CallGraphAnalyzerAccessor::doClassFile(*analyzer, badFile.string());

    EXPECT_EQ(context->results.standaloneClassfiles.errors.load(), errorsBefore + 1);
}

TEST_F(CallGraphAnalyzerTests, DoClassFileReplacesExistingClassRowsWhenModified) {
    const auto classFile = std::filesystem::path("test-subjects/org/kapa/reti/ast/AstNode.class");
    ASSERT_TRUE(std::filesystem::exists(classFile));

    auto files = db->getFiles();
    auto &existingFile = reinterpret_cast<FileRow &>(*files->allocateRow());
    new(&existingFile) FileRow();
    existingFile.filename = db->getPoolString(classFile.string());
    existingFile.fileSize = 0;
    existingFile.lastWriteTime = 0;
    existingFile.classfileCount = 1;
    files->add(&existingFile);

    auto classFiles = db->getClassFiles();
    auto &existingClass = reinterpret_cast<ClassFileRow &>(*classFiles->allocateRow());
    new(&existingClass) ClassFileRow(existingFile);
    existingClass.classname = db->getPoolString("org/kapa/reti/ast/AstNode");
    classFiles->add(&existingClass);

    const auto strongName = classFiles->getStrongClassname(existingClass);
    CallGraphAnalyzerAccessor::options(*analyzer).useFileTimestamp = false;
    CallGraphAnalyzerAccessor::doClassFile(*analyzer, classFile.string());

    auto replacement = reinterpret_cast<ClassFileRow *>(classFiles->findByKey(strongName));
    ASSERT_NE(replacement, nullptr);
    auto *updatedFile = reinterpret_cast<FileRow *>(files->getRow(replacement->file.id));
    ASSERT_NE(updatedFile, nullptr);
    EXPECT_EQ(updatedFile->fileSize, std::filesystem::file_size(classFile));
}

TEST_F(CallGraphAnalyzerTests, DoClassFileMarksExistingClassAsModified) {
    const auto classFile = std::filesystem::path("test-subjects/org/kapa/reti/ast/AstNode.class");
    ASSERT_TRUE(std::filesystem::exists(classFile));

    auto &options = CallGraphAnalyzerAccessor::options(*analyzer);
    options.useFileTimestamp = false;

    CallGraphAnalyzerAccessor::doClassFile(*analyzer, classFile.string());
    const auto &initialResults = context->results.report->getClassResults();
    ASSERT_FALSE(initialResults.empty());
    const auto strongName = initialResults.back().strongClassname;

    CallGraphAnalyzerAccessor::doClassFile(*analyzer, classFile.string());
    EXPECT_EQ(context->results.standaloneClassfiles.taskResult.differentDigest.load(), 1u);
}

TEST_F(CallGraphAnalyzerTests, ProcessStandaloneClassFileRunsAsyncAndStoresRows) {
    const auto original = std::filesystem::path("test-subjects/org/kapa/reti/ast/AstNode.class");
    ASSERT_TRUE(std::filesystem::exists(original));
    const auto copyPath = tempDir / "StandaloneCopy.class";
    std::filesystem::copy_file(original, copyPath, std::filesystem::copy_options::overwrite_existing);

    const auto filesBefore = db->getFiles()->size();
    CallGraphAnalyzerAccessor::processStandaloneClassFile(*analyzer, copyPath.string());
    CallGraphAnalyzerAccessor::waitForWorkerPool(*analyzer);

    EXPECT_GT(db->getFiles()->size(), filesBefore);
    EXPECT_GT(context->results.standaloneClassfiles.count.load(), 0u);
}

TEST_F(CallGraphAnalyzerTests, ProcessJarParsesEntriesAndUpdatesCounts) {
    const auto sourceJar = std::filesystem::path("test-subjects/jars/AstNode.jar");
    ASSERT_TRUE(std::filesystem::exists(sourceJar));
    const auto tempJar = tempDir / "AstNodeCopy.jar";
    std::filesystem::copy_file(sourceJar, tempJar, std::filesystem::copy_options::overwrite_existing);

    const auto jarsBefore = context->results.jarfiles.count.load();
    CallGraphAnalyzerAccessor::processJar(*analyzer, tempJar.string());
    CallGraphAnalyzerAccessor::waitForWorkerPool(*analyzer);

    EXPECT_EQ(context->results.jarfiles.count.load(), jarsBefore + 1);
    EXPECT_GT(db->getClassFiles()->size(), 0u);
}
