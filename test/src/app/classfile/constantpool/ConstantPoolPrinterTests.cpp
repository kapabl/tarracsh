#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <iterator>
#include <memory>
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
