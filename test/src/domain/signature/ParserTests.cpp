#include <gtest/gtest.h>

#include <memory>

#include "domain/classfile/signature/parser/Parser.h"
#include "domain/classfile/types/SignatureNodes.h"

using namespace kapa::tarracsh::domain::classfile::signature::parser;

TEST(SignatureParserNodes, ParsesGenericClassSignature) {
    const std::string signature = "<T:Ljava/lang/Object;>Ljava/util/AbstractList<TT;>;Ljava/util/List<TT;>;";
    auto parser = Parser::make();
    auto classSig = parser->parseClassSig(signature);

    const auto formals = classSig->getFormalTypeParameters();
    ASSERT_EQ(formals.size(), 1);
    const auto formal = formals.front();
    EXPECT_EQ(formal->getName(), "T");
    ASSERT_EQ(formal->getBounds().size(), 1);

    const auto superclass = classSig->getSuperclass();
    ASSERT_NE(superclass, nullptr);
    const auto &superPath = superclass->getPath();
    ASSERT_FALSE(superPath.empty());
    EXPECT_EQ(superPath.front()->getName(), "java.util.AbstractList");
    ASSERT_EQ(superPath.front()->getTypeArguments().size(), 1);

    const auto &interfaces = classSig->getSuperInterfaces();
    ASSERT_EQ(interfaces.size(), 1);
    const auto &ifacePath = interfaces.front()->getPath();
    ASSERT_FALSE(ifacePath.empty());
    EXPECT_EQ(ifacePath.front()->getName(), "java.util.List");
}

TEST(SignatureParserNodes, ParsesMethodSignatureWithBoundsAndExceptions) {
    const std::string signature = "<T:Ljava/lang/Object;>(TT;[I)Ljava/util/List<TT;>;^Ljava/lang/Exception;";
    auto parser = Parser::make();
    auto methodSig = parser->parseMethodSig(signature);

    ASSERT_EQ(methodSig->getFormalTypeParameters().size(), 1);
    ASSERT_EQ(methodSig->getParameterTypes().size(), 2);
    ASSERT_EQ(methodSig->getExceptionTypes().size(), 1);
    const auto arrayParam = std::dynamic_pointer_cast<ArrayTypeSignature>(methodSig->getParameterTypes()[1]);
    ASSERT_NE(arrayParam, nullptr);
    const auto returnType = methodSig->getReturnType();
    ASSERT_NE(returnType, nullptr);
}

TEST(SignatureParserNodes, ParsesNestedTypeSignature) {
    const std::string signature = "Ljava/util/Map$Entry<Ljava/lang/String;Ljava/lang/Integer;>;";
    auto parser = Parser::make();
    auto typeSig = parser->parseTypeSig(signature);

    const auto classType = std::dynamic_pointer_cast<ClassTypeSignature>(typeSig);
    ASSERT_NE(classType, nullptr);
    const auto &path = classType->getPath();
    ASSERT_EQ(path.size(), 2);
    EXPECT_EQ(path.front()->getName(), "java.util.Map");
    EXPECT_EQ(path.back()->getName(), "Entry");
}
