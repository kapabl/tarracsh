#include "CallGraphAnalyzerTestBase.h"

using testhooks::CallGraphAnalyzerAccessor;

TEST_F(CallGraphAnalyzerTests, LinkClassRefsCreatesEdges) {
    auto &classA = addClass("A.class", "com/foo/A");
    auto &classB = addClass("B.class", "com/foo/B");

    auto &classRef = addClassRef(classA, "com/foo/B");
    auto classRefs = db->getClassRefs();

    CallGraphAnalyzerAccessor::createNamedIndexes(*analyzer);
    CallGraphAnalyzerAccessor::linkClassRefs(*analyzer);

    auto classRefEdges = db->getClassRefEdges();
    EXPECT_EQ(classRefEdges->size(), 1u);

    auto updatedRef = reinterpret_cast<ClassRefRow *>(classRefs->getRow(classRef.id));
    ASSERT_NE(updatedRef, nullptr);
    EXPECT_EQ(updatedRef->edgeCount, 1u);

    auto classFiles = db->getClassFiles();
    auto updatedTarget = classFiles->getClassFileRow(classB.id);
    ASSERT_NE(updatedTarget, nullptr);
    EXPECT_EQ(updatedTarget->refCount, 1u);
}

TEST_F(CallGraphAnalyzerTests, LinkMethodRefsMatchesTargetsAndIncrementsCounts) {
    auto &classA = addClass("A.class", "com/foo/A");
    auto &classB = addClass("B.class", "com/foo/B");
    auto &method = addMethod(classB, "doIt", "()V");

    auto &methodRef = addMethodRef(classA, "com/foo/B", "doIt", "()V");
    auto methodRefs = db->getMethodRefs();

    CallGraphAnalyzerAccessor::createNamedIndexes(*analyzer);
    CallGraphAnalyzerAccessor::linkMethodRefs(*analyzer);

    auto methodRefEdges = db->getMethodRefEdges();
    EXPECT_EQ(methodRefEdges->size(), 1u);

    auto updatedRef = reinterpret_cast<MethodRefRow *>(methodRefs->getRow(methodRef.id));
    ASSERT_NE(updatedRef, nullptr);
    EXPECT_EQ(updatedRef->edgeCount, 1u);

    auto methods = db->getMethods();
    auto updatedMethod = reinterpret_cast<MethodRow *>(methods->getRow(method.id));
    ASSERT_NE(updatedMethod, nullptr);
    EXPECT_EQ(updatedMethod->refCount, 1u);
}

TEST_F(CallGraphAnalyzerTests, LinkFieldRefsMatchesTargetsAndIncrementsCounts) {
    auto &classA = addClass("A.class", "com/foo/A");
    auto &classB = addClass("B.class", "com/foo/B");
    auto &field = addField(classB, "value", "I");

    auto &fieldRef = addFieldRef(classA, "com/foo/B", "value", "I");
    auto fieldRefs = db->getFieldRefs();

    CallGraphAnalyzerAccessor::createNamedIndexes(*analyzer);
    CallGraphAnalyzerAccessor::linkFieldRefs(*analyzer);

    auto fieldRefEdges = db->getFieldRefEdges();
    EXPECT_EQ(fieldRefEdges->size(), 1u);

    auto updatedRef = reinterpret_cast<FieldRefRow *>(fieldRefs->getRow(fieldRef.id));
    ASSERT_NE(updatedRef, nullptr);
    EXPECT_EQ(updatedRef->edgeCount, 1u);

    auto fields = db->getFields();
    auto updatedField = reinterpret_cast<FieldRow *>(fields->getRow(field.id));
    ASSERT_NE(updatedField, nullptr);
    EXPECT_EQ(updatedField->refCount, 1u);
}

TEST_F(CallGraphAnalyzerTests, LinkRefNodesProcessesAllReferenceTypes) {
    auto &classA = addClass("A.class", "com/foo/A");
    auto &classB = addClass("B.class", "com/foo/B");
    auto &method = addMethod(classB, "doIt", "()V");
    auto &field = addField(classB, "value", "I");

    auto &classRef = addClassRef(classA, "com/foo/B");
    auto &methodRef = addMethodRef(classA, "com/foo/B", "doIt", "()V");
    auto &fieldRef = addFieldRef(classA, "com/foo/B", "value", "I");

    CallGraphAnalyzerAccessor::createNamedIndexes(*analyzer);
    CallGraphAnalyzerAccessor::linkRefNodes(*analyzer);

    EXPECT_EQ(db->getClassRefEdges()->size(), 1u);
    EXPECT_EQ(db->getMethodRefEdges()->size(), 1u);
    EXPECT_EQ(db->getFieldRefEdges()->size(), 1u);

    auto classRefs = db->getClassRefs();
    auto updatedClassRef = reinterpret_cast<ClassRefRow *>(classRefs->getRow(classRef.id));
    ASSERT_NE(updatedClassRef, nullptr);
    EXPECT_EQ(updatedClassRef->edgeCount, 1u);

    auto methodRefs = db->getMethodRefs();
    auto updatedMethodRef = reinterpret_cast<MethodRefRow *>(methodRefs->getRow(methodRef.id));
    ASSERT_NE(updatedMethodRef, nullptr);
    EXPECT_EQ(updatedMethodRef->edgeCount, 1u);

    auto fieldRefs = db->getFieldRefs();
    auto updatedFieldRef = reinterpret_cast<FieldRefRow *>(fieldRefs->getRow(fieldRef.id));
    ASSERT_NE(updatedFieldRef, nullptr);
    EXPECT_EQ(updatedFieldRef->edgeCount, 1u);

    auto methods = db->getMethods();
    auto updatedMethod = reinterpret_cast<MethodRow *>(methods->getRow(method.id));
    ASSERT_NE(updatedMethod, nullptr);
    EXPECT_EQ(updatedMethod->refCount, 1u);

    auto fields = db->getFields();
    auto updatedField = reinterpret_cast<FieldRow *>(fields->getRow(field.id));
    ASSERT_NE(updatedField, nullptr);
    EXPECT_EQ(updatedField->refCount, 1u);
}

TEST_F(CallGraphAnalyzerTests, EndAnalysisExportsDotAndGmlWhenRequested) {
    auto &classA = addClass("A.class", "com/foo/A");
    auto &classB = addClass("B.class", "com/foo/B");
    addMethod(classB, "doIt", "()V");
    addField(classB, "value", "I");

    addClassRef(classA, "com/foo/B");
    addMethodRef(classA, "com/foo/B", "doIt", "()V");
    addFieldRef(classA, "com/foo/B", "value", "I");

    auto &options = CallGraphAnalyzerAccessor::options(*analyzer);
    options.outputDir = context->options.outputDir;
    options.callGraph.exportDot = true;
    options.callGraph.exportGml = true;
    options.callGraph.dryRun = true;

    CallGraphAnalyzerAccessor::endAnalysis(*analyzer);

    const auto graphDir = std::filesystem::path(context->options.outputDir) / "callgraph";
    EXPECT_TRUE(std::filesystem::exists(graphDir / "callgraph.dot"));
    EXPECT_TRUE(std::filesystem::exists(graphDir / "callgraph.gml"));
}
