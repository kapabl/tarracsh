
#include <gtest/gtest.h>
#include <format>
#include "../app/App.h"
#include "../domain/classfile/signature/grammar/JvmSignatureLexer.h"
#include "../domain/classfile/signature/grammar/JvmSignatureParser.h"


using namespace kapa::tarracsh::domain::classfile::signature::parser;

#define TEST_SIG "[C[Ljava/lang/Character;Ljava/util/Map<Ljava/lang/Character;Lkotlin/UInt;>;Ljava/util/List<+Lorg/kapa/reti/lexers/skippers/Skipper;>;Ljava/util/List<Lorg/kapa/reti/lexers/Keyword;>;Ljava/util/List<Lorg/kapa/reti/lexers/lexemes/Lexeme;>;)V"

class MyErrorListener : public antlr4::ANTLRErrorListener {
public:
    virtual void syntaxError(antlr4::Recognizer* recognizer, antlr4::Token* offendingSymbol, size_t line,
        size_t charPositionInLine, const std::string& msg, std::exception_ptr e) override {
        std::cerr << "Syntax error at line " << line << ", column " << charPositionInLine << ": " << msg << std::endl;
    }

    virtual void reportAmbiguity(antlr4::Parser* recognizer, const antlr4::dfa::DFA& dfa, size_t startIndex,
        size_t stopIndex, bool exact, const antlrcpp::BitSet& ambigAlts,
        antlr4::atn::ATNConfigSet* configs) override {
        // handle ambiguity errors if needed
    }

    virtual void reportAttemptingFullContext(antlr4::Parser* recognizer, const antlr4::dfa::DFA& dfa, size_t startIndex,
        size_t stopIndex, const antlrcpp::BitSet& conflictingAlts,
        antlr4::atn::ATNConfigSet* configs) override {
        // handle full-context errors if needed
    }

    virtual void reportContextSensitivity(antlr4::Parser* recognizer, const antlr4::dfa::DFA& dfa, size_t startIndex,
        size_t stopIndex, size_t prediction,
        antlr4::atn::ATNConfigSet* configs) override {
        // handle context-sensitivity errors if needed
    }
};


TEST(TestCaseName, TestName) {
    std::string result;
    antlr4::ANTLRInputStream input(TEST_SIG);

    JvmSignatureLexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);
    JvmSignatureParser parser(&tokens);

    MyErrorListener errorListener;
    parser.removeErrorListeners();
    parser.addErrorListener(&errorListener);

    auto parseTree = parser.methodSignature();
    auto parsed = parser.getNumberOfSyntaxErrors() == 0;

    if (parsed) {
        // parser::JavaSignatureListenerImpl listener;
        // antlr4::tree::ParseTreeWalker::DEFAULT.walk(&listener, parseTree);
        // result = listener.getResult();
    }
    else {
        std::cout << std::format("Invalid java signature: {}", TEST_SIG) << std::endl;
    }
}