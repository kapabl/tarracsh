#include <gtest/gtest.h>

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

#include "app/classfile/ParserPrinter.h"
#include "app/classfile/constantpool/printer/nav/HtmlGen.h"
#include "domain/Options.h"
#include "domain/classfile/ClassFileParser.h"
#include "domain/classfile/reader/MemoryReader.h"
#include "infrastructure/log/Log.h"

using namespace kapa::tarracsh;
using namespace kapa::tarracsh::app::classfile;
using namespace kapa::tarracsh::app::classfile::constantpool::printer::nav;
using namespace kapa::tarracsh::domain;
using namespace kapa::tarracsh::domain::classfile;
using namespace kapa::tarracsh::domain::classfile::reader;

namespace {

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

std::filesystem::path makeTempDir(const std::string &prefix) {
    const auto base = std::filesystem::temp_directory_path();
    const auto stamp = std::chrono::steady_clock::now().time_since_epoch().count();
    auto dir = base / (prefix + std::to_string(stamp));
    std::filesystem::create_directories(dir);
    return dir;
}

std::string readTextFile(const std::filesystem::path &path) {
    std::ifstream file(path);
    return std::string(std::istreambuf_iterator<char>(file), {});
}

void writeTextFile(const std::filesystem::path &path, const std::string &content) {
    std::ofstream file(path);
    file << content;
}

struct ParsedClass {
    std::vector<char> bytes;
    std::unique_ptr<MemoryReader> reader;
    std::shared_ptr<kapa::infrastructure::log::Log> log;
    std::unique_ptr<ClassFileParser> parser;
};

ParsedClass parseSampleClass() {
    ParsedClass result;
    const auto classPath = workspacePath("test-subjects/org/kapa/reti/App.class");
    result.bytes = loadBinary(classPath);
    result.reader = std::make_unique<MemoryReader>(
        result.bytes.data(),
        static_cast<std::streamsize>(result.bytes.size()));
    result.log = std::make_shared<kapa::infrastructure::log::Log>();
    auto parser = std::make_unique<ClassFileParser>(*result.reader, classPath.string(), result.log);
    const bool parsedOk = parser->parse();
    EXPECT_TRUE(parsedOk);
    if (!parsedOk) {
        return {};
    }
    result.parser = std::move(parser);
    return result;
}

std::string capturePrinterOutput(ParserPrinter &printer) {
    std::ostringstream capture;
    auto *original = std::cout.rdbuf(capture.rdbuf());
    printer.print();
    std::cout.rdbuf(original);
    return capture.str();
}

void ensureHtmlTemplatesLoaded() {
    static bool initialized = false;
    static std::filesystem::path templateDir;
    if (initialized) {
        return;
    }
    templateDir = makeTempDir("tarracsh-html-templates-");
    const std::string classTemplate = R"(<!doctype html>
<html><body><h1>Constant Pool for: {{classname}}</h1>{{cpoolEntries}}{{classRefs}}</body></html>)";
    const std::string implTemplate = R"(<!doctype html>
<html><body><h1>Implementations - {{classname}}</h1>
{% for implementation in implementations %}<div>{{implementation}}</div>{% endfor %}
</body></html>)";
    writeTextFile(templateDir / "cpool-tmpl.html", classTemplate);
    writeTextFile(templateDir / "impl-index-tmpl.html", implTemplate);
    const auto previousCwd = std::filesystem::current_path();
    std::filesystem::current_path(templateDir);
    HtmlGen::init();
    std::filesystem::current_path(previousCwd);
    initialized = true;
}

} // namespace

TEST(ParserPrinterTests, EmitsReadableStructureForSampleClass) {
    auto parsed = parseSampleClass();
    ASSERT_TRUE(parsed.parser);
    ParserPrinter printer(*parsed.parser);

    const auto output = capturePrinterOutput(printer);

    EXPECT_NE(output.find("//Class"), std::string::npos);
    EXPECT_NE(output.find("org.kapa.reti.App"), std::string::npos);
    EXPECT_NE(output.find("//Methods:"), std::string::npos);
    EXPECT_NE(output.find("//Fields:"), std::string::npos);
}

TEST(HtmlGenTests, GeneratesNavigationAndConstantPoolHtml) {
    auto parsed = parseSampleClass();
    ASSERT_TRUE(parsed.parser);
    Options options;
    const auto tempDir = makeTempDir("tarracsh-htmlgen-");
    options.outputDir = tempDir.string();

    ensureHtmlTemplatesLoaded();

    HtmlGen htmlGen(*parsed.parser, options);
    htmlGen.print();

    const auto classRelDir = std::filesystem::path("org/kapa/reti/App");
    const auto navDir = tempDir / "nav" / classRelDir;
    const auto indexHtml = navDir / "index.html";
    const auto cpoolHtml = navDir / "App.class" / "cpool.html";

    ASSERT_TRUE(std::filesystem::exists(indexHtml));
    ASSERT_TRUE(std::filesystem::exists(cpoolHtml));

    const auto indexContent = readTextFile(indexHtml);
    EXPECT_NE(indexContent.find("Implementations -"), std::string::npos);
    EXPECT_FALSE(indexContent.empty());

    const auto cpoolContent = readTextFile(cpoolHtml);
    EXPECT_NE(cpoolContent.find("Constant Pool for:"), std::string::npos);
    EXPECT_FALSE(cpoolContent.empty());

    std::filesystem::remove_all(tempDir);
}
