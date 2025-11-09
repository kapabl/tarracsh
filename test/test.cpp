#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include <format>
#include "domain/classfile/signature/parser/Parser.h"


using namespace kapa::tarracsh::domain::classfile::signature::parser;


std::vector<const char *> testJvmMethodSignatures{
    "([Ljava/lang/String;)V",
    "()I",
    "()V",
    "(DD)D",
    "()Ljava/util/List;",
    "<T:Ljava/lang/Object;>(Ljava/util/List<TT;>;)V",
    "<T:Ljava/lang/Comparable<TT;>;>(TT;TT;)TT;",
    "<K:Ljava/lang/Object;V:Ljava/lang/Object;>()Ljava/util/Map<TK;TV;>;",
    "<T:Ljava/lang/Object;>([TT;)[TT;",
    "<T:Ljava/lang/Object;>(Ljava/lang/Class<TT;>;)TT;",
    "(Ljava/util/List<+Ljava/lang/Number;>;)Ljava/util/List<+Ljava/lang/Number;>;",
    "(Ljava/util/function/Consumer<-TT;>;)V",
    "(Ljava/util/List<TT;>;Ljava/util/function/Predicate<-TT;>;)Ljava/util/List<TT;>;",
    "(Ljava/util/List<*TT;>;Ljava/util/function/Predicate<*TT;>;)Ljava/util/List<*TT;>;"
};

TEST_CASE("JVM Method Signature", "Method Sig") {

    const auto parser = Parser::make();

    for (const auto &testSig : testJvmMethodSignatures) {
        std::cout << std::format("testing method sig:{}", testSig) << std::endl;
        parser->reset();
        try {
            const auto tree = parser->parseMethodSig(testSig);
            REQUIRE(tree);
        } catch (std::exception &error) {
            std::cout << error.what() << std::endl;
            REQUIRE(false);
        }

    }

}

std::vector<const char *> testJvmClassSignatures{
};

TEST_CASE("JVM Class Signature", "Class Sig") {

    const auto parser = Parser::make();

    for (const auto &testSig : testJvmClassSignatures) {
        std::cout << std::format("testing Class sig:{}", testSig) << std::endl;
        parser->reset();
        try {
            const auto tree = parser->parseClassSig(testSig);
            REQUIRE(tree);
        } catch (std::exception &error) {
            std::cout << error.what() << std::endl;
            REQUIRE(false);
        }
    }

}

std::vector<const char *> testJvmFieldSignatures{
    "[Ljava/lang/String;",
    "I",
    "Ljava/util/List<Ljava/lang/String;>;"
};

TEST_CASE("JVM Types Signature", "Type Sig") {

    const auto parser = Parser::make();

    for (const auto &testSig : testJvmFieldSignatures) {
        std::cout << std::format("testing field sig:{}", testSig) << std::endl;
        parser->reset();
        try {
            const auto tree = parser->parseTypeSig(testSig);
            REQUIRE(tree);
        } catch (std::exception &error) {
            std::cout << error.what() << std::endl;
            REQUIRE(false);
        }

    }

}


int main(int argc, char *argv[]) {
    // your setup ...

    int result = Catch::Session().run(argc, argv);

    // your clean-up...

    return result;
}
