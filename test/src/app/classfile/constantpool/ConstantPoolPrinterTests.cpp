#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <iterator>
#include <memory>
#include <cstring>
#include <sstream>
#include <string>
#include <vector>

#include "app/Context.h"
#include "app/classfile/constantpool/printer/ConstantPoolPrinter.h"
#include "domain/classfile/ClassFileConstants.h"
#include "domain/Options.h"
#include "domain/classfile/reader/MemoryReader.h"
#include "domain/stats/Results.h"
#include "infrastructure/log/Log.h"

using namespace kapa::tarracsh;
using namespace app;
using namespace app::classfile::constantpool::printer;
using namespace domain;
using namespace domain::classfile::constantpool;
using namespace domain::stats;
using kapa::infrastructure::log::Log;

namespace {

class StubContext : public Context {
public:
    StubContext()
        : options(), results(options) {
        log = std::make_shared<Log>();
        results.log = log;
    }

    Results &getResults() override { return results; }
    Options &getOptions() override { return options; }
    Log &getLog() override { return *log; }

    Options options;
    Results results;
    std::shared_ptr<Log> log;
};

struct ConstantPoolPrinterAccessor : ConstantPoolPrinter {
    using ConstantPoolPrinter::tagToString;
    using ConstantPoolPrinter::refKindToString;

    static void clearFilter() {
        _filter.clear();
    }
};

struct ParsedClassFile {
    std::vector<char> bytes;
    std::unique_ptr<domain::classfile::reader::MemoryReader> reader;
    std::shared_ptr<Log> log;
    std::unique_ptr<domain::classfile::ClassFileParser> parser;
};

std::filesystem::path workspacePath(const std::string &relative) {
    if (const char *srcDir = std::getenv("TEST_SRCDIR")) {
        if (const char *workspace = std::getenv("TEST_WORKSPACE")) {
            auto candidate = std::filesystem::path(srcDir) / workspace / relative;
            if (std::filesystem::exists(candidate)) {
                return candidate;
            }
        }
    }
    return std::filesystem::path(PROJECT_SOURCE_DIR) / relative;
}

std::vector<char> loadBinary(const std::filesystem::path &path) {
    std::ifstream file(path, std::ios::binary);
    return std::vector<char>(std::istreambuf_iterator<char>(file), {});
}

ParsedClassFile parseSampleClass() {
    ParsedClassFile parsed;
    const auto classPath = workspacePath("test-subjects/org/kapa/reti/App.class");
    parsed.bytes = loadBinary(classPath);
    parsed.reader = std::make_unique<domain::classfile::reader::MemoryReader>(
        parsed.bytes.data(),
        static_cast<std::streamsize>(parsed.bytes.size()));
    parsed.log = std::make_shared<Log>();
    auto parser = std::make_unique<domain::classfile::ClassFileParser>(*parsed.reader, classPath.string(), parsed.log);
    EXPECT_TRUE(parser->parse());
    parsed.parser = std::move(parser);
    return parsed;
}

std::string capturePrinterOutput(ConstantPoolPrinter &printer) {
    std::ostringstream capture;
    auto *originalBuf = std::cout.rdbuf(capture.rdbuf());
    printer.print();
    std::cout.rdbuf(originalBuf);
    return capture.str();
}

u2 addUtf8(ConstantPool &pool, const std::string &value) {
    const auto allocSize = sizeof(ConstantPoolRecord::utf8Info.tag) +
                           sizeof(ConstantPoolRecord::utf8Info.length) +
                           value.size() + 1;
    auto *record = static_cast<ConstantPoolRecord *>(std::malloc(allocSize));
    record->utf8Info.tag = JVM_CONSTANT_Utf8;
    record->utf8Info.length = static_cast<u2>(value.size());
    std::memcpy(record->utf8Info.bytes, value.data(), value.size());
    record->utf8Info.bytes[value.size()] = 0;
    pool.addRecord(*record, allocSize);
    std::free(record);
    return static_cast<u2>(pool.getPoolSize() - 1);
}

u2 addNameAndType(ConstantPool &pool, u2 nameIndex, u2 descriptorIndex) {
    ConstantPoolRecord record{};
    record.nameAndTypeInfo.tag = JVM_CONSTANT_NameAndType;
    record.nameAndTypeInfo.nameIndex = nameIndex;
    record.nameAndTypeInfo.descriptorIndex = descriptorIndex;
    pool.addRecord(record);
    return static_cast<u2>(pool.getPoolSize() - 1);
}

void addModuleEntry(ConstantPool &pool, u2 nameIndex) {
    ConstantPoolRecord record{};
    record.moduleInfo.tag = JVM_CONSTANT_Module;
    record.moduleInfo.nameIndex = nameIndex;
    pool.addRecord(record);
}

void addPackageEntry(ConstantPool &pool, u2 nameIndex) {
    ConstantPoolRecord record{};
    record.packageInfo.tag = JVM_CONSTANT_Package;
    record.packageInfo.nameIndex = nameIndex;
    pool.addRecord(record);
}

void addDynamicEntry(ConstantPool &pool, u2 bootstrapIndex, u2 nameAndTypeIndex) {
    ConstantPoolRecord record{};
    record.dynamicInfo.tag = JVM_CONSTANT_Dynamic;
    record.dynamicInfo.bootstrapMethodAttrIndex = bootstrapIndex;
    record.dynamicInfo.nameAndTypeIndex = nameAndTypeIndex;
    pool.addRecord(record);
}

void addUnicodeEntry(ConstantPool &pool) {
    ConstantPoolRecord record{};
    record.base.tag = JVM_CONSTANT_Unicode;
    pool.addRecord(record);
}

void addEmptyEntry(ConstantPool &pool) {
    ConstantPoolRecord record{};
    record.base.tag = JVM_CONSTANT_Empty;
    pool.addRecord(record);
}

} // namespace

TEST(ConstantPoolPrinterTests, DescriptiveFlagControlsTagStrings) {
    StubContext context;
    context.options.parse.descriptiveCPoolEntries = false;
    ConstantPoolPrinterAccessor::clearFilter();
    ConstantPoolPrinter::init(context);
    EXPECT_EQ(ConstantPoolPrinterAccessor::tagToString(JVM_CONSTANT_String), "string");

    context.options.parse.descriptiveCPoolEntries = true;
    ConstantPoolPrinterAccessor::clearFilter();
    ConstantPoolPrinter::init(context);
    EXPECT_EQ(ConstantPoolPrinterAccessor::tagToString(JVM_CONSTANT_String), "String");
}

TEST(ConstantPoolPrinterTests, RefKindStringsAndInvalidTagsHandled) {
    StubContext context;
    ConstantPoolPrinterAccessor::clearFilter();
    ConstantPoolPrinter::init(context);

    EXPECT_EQ(ConstantPoolPrinterAccessor::refKindToString(JVM_REF_getField), "REF_getField");
    EXPECT_EQ(ConstantPoolPrinterAccessor::refKindToString(JVM_REF_invokeInterface), "REF_invokeInterface");

    const auto invalidRef = ConstantPoolPrinterAccessor::refKindToString(static_cast<MethodHandleSubtypes>(JVM_REF_LIMIT));
    EXPECT_NE(invalidRef.find("Invalid MethodHandleSubtypes"), std::string::npos);

    const auto invalidTag = ConstantPoolPrinterAccessor::tagToString(static_cast<ConstantPoolTag>(JVM_CONSTANT_ExternalMax + 5));
    EXPECT_NE(invalidTag.find("Invalid pool tag"), std::string::npos);
}

TEST(ConstantPoolPrinterTests, PrintHonorsTagFilter) {
    auto parsed = parseSampleClass();
    ASSERT_TRUE(parsed.parser);

    StubContext context;
    context.options.parse.descriptiveCPoolEntries = true;
    context.options.parse.cpoolFilter = {"utf8"};
    ConstantPoolPrinterAccessor::clearFilter();
    ConstantPoolPrinter::init(context);

    ConstantPoolPrinter printer(*parsed.parser);
    const auto output = capturePrinterOutput(printer);

    EXPECT_NE(output.find("UTF8 String"), std::string::npos);
    EXPECT_EQ(output.find("Class Ref"), std::string::npos);
}

TEST(ConstantPoolPrinterTests, PrintOutputsMultipleEntryTypesWhenUnfiltered) {
    auto parsed = parseSampleClass();
    ASSERT_TRUE(parsed.parser);

    StubContext context;
    context.options.parse.descriptiveCPoolEntries = true;
    context.options.parse.cpoolFilter.clear();
    ConstantPoolPrinterAccessor::clearFilter();
    ConstantPoolPrinter::init(context);

    ConstantPoolPrinter printer(*parsed.parser);
    const auto output = capturePrinterOutput(printer);

    EXPECT_NE(output.find("Constant Pool for:"), std::string::npos);
    EXPECT_NE(output.find("Class Ref"), std::string::npos);
    EXPECT_NE(output.find("Method Ref"), std::string::npos);
}

TEST(ConstantPoolPrinterTests, PrintHandlesModulePackageDynamicAndSpecialEntries) {
    auto parsed = parseSampleClass();
    ASSERT_TRUE(parsed.parser);

    auto &pool = parsed.parser->getConstantPool();
    const auto moduleNameIndex = addUtf8(pool, "test.module");
    addModuleEntry(pool, moduleNameIndex);

    const auto packageNameIndex = addUtf8(pool, "org/example/pkg");
    addPackageEntry(pool, packageNameIndex);

    const auto dynNameIndex = addUtf8(pool, "dynamicName");
    const auto dynDescIndex = addUtf8(pool, "()V");
    const auto nameAndTypeIndex = addNameAndType(pool, dynNameIndex, dynDescIndex);
    addDynamicEntry(pool, 1, nameAndTypeIndex);

    addUnicodeEntry(pool);
    addEmptyEntry(pool);

    StubContext context;
    context.options.parse.descriptiveCPoolEntries = true;
    context.options.parse.cpoolFilter = {"module", "package", "dynamic", "unicode", "empty"};
    ConstantPoolPrinterAccessor::clearFilter();
    ConstantPoolPrinter::init(context);

    ConstantPoolPrinter printer(*parsed.parser);
    const auto output = capturePrinterOutput(printer);

    EXPECT_NE(output.find("test.module"), std::string::npos);
    EXPECT_NE(output.find("Package"), std::string::npos);
    EXPECT_NE(output.find("Dynamic"), std::string::npos);
    EXPECT_NE(output.find("Unused Cool Entry type"), std::string::npos);
    EXPECT_NE(output.find("empty entry"), std::string::npos);
}
