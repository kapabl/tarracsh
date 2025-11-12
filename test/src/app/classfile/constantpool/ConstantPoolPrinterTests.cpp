#include <gtest/gtest.h>

#include <memory>
#include <string>

#include "app/Context.h"
#include "app/classfile/constantpool/printer/ConstantPoolPrinter.h"
#include "domain/classfile/ClassFileConstants.h"
#include "domain/Options.h"
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
};

} // namespace

TEST(ConstantPoolPrinterTests, DescriptiveFlagControlsTagStrings) {
    StubContext context;
    context.options.parse.descriptiveCPoolEntries = false;
    ConstantPoolPrinter::init(context);
    EXPECT_EQ(ConstantPoolPrinterAccessor::tagToString(JVM_CONSTANT_String), "string");

    context.options.parse.descriptiveCPoolEntries = true;
    ConstantPoolPrinter::init(context);
    EXPECT_EQ(ConstantPoolPrinterAccessor::tagToString(JVM_CONSTANT_String), "String");
}

TEST(ConstantPoolPrinterTests, RefKindStringsAndInvalidTagsHandled) {
    StubContext context;
    ConstantPoolPrinter::init(context);

    EXPECT_EQ(ConstantPoolPrinterAccessor::refKindToString(JVM_REF_getField), "REF_getField");
    EXPECT_EQ(ConstantPoolPrinterAccessor::refKindToString(JVM_REF_invokeInterface), "REF_invokeInterface");

    const auto invalidRef = ConstantPoolPrinterAccessor::refKindToString(static_cast<MethodHandleSubtypes>(JVM_REF_LIMIT));
    EXPECT_NE(invalidRef.find("Invalid MethodHandleSubtypes"), std::string::npos);

    const auto invalidTag = ConstantPoolPrinterAccessor::tagToString(static_cast<ConstantPoolTag>(JVM_CONSTANT_ExternalMax + 5));
    EXPECT_NE(invalidTag.find("Invalid pool tag"), std::string::npos);
}
