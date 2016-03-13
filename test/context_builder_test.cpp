#include <gtest/gtest.h>
#include "ast/context_builder.h"

using namespace fj;

TEST (context_builder, build_class_with_two_properties) {
    auto constructor = std::make_shared< ConstructorBody >();
    constructor->addProperty("fst");
    constructor->addProperty("snd");

    auto newClass = std::make_shared< ClassDeclaration >();
    newClass->setName("Pair");
    newClass->setParentName("Object");
    newClass->setConstructorBody(constructor);
    newClass->addProperty("Object", "fst");
    newClass->addProperty("Object", "snd");

    ParsedContext parsedContext;
    parsedContext.addClass(newClass);

    Context context;
    ContextBuilder builder;
    builder.buildAST(parsedContext, context);

    ASSERT_EQ(2, context.getClasses().size());

    auto builtClass = context.getClasses()[ClassName("Pair")];
    Properties properties = builtClass->getProperties();

    EXPECT_EQ("Object", properties[PropertyName("fst")].t);
    EXPECT_EQ("Object", properties[PropertyName("snd")].t);
}

TEST (context_builder, build_class_with_a_method) {
    auto constructor = std::make_shared< ConstructorBody >();

    auto variableTerm = std::make_shared< VariableTerm >();
    variableTerm->setName("queue");
    auto methodTerm = std::make_shared< AccessTerm >();
    methodTerm->setName("last");
    methodTerm->setTerm(variableTerm);

    auto method = std::make_shared< MethodDeclaration >();
    method->setName("getFst");
    method->setReturnClassName("Object");
    method->setBodyTerm(methodTerm);

    auto newClass = std::make_shared< ClassDeclaration >();
    newClass->setName("Pair");
    newClass->setParentName("Object");
    newClass->setConstructorBody(constructor);
    newClass->addMethod(method);

    ParsedContext parsedContext;
    parsedContext.addClass(newClass);

    Context context;
    ContextBuilder builder;
    builder.buildAST(parsedContext, context);

    ASSERT_EQ(2, context.getClasses().size());
    auto builtClass = context.getClasses()[ClassName("Pair")];

    auto methodBody = builtClass->getMethod(MethodName("getFst"));
    ASSERT_NE(nullptr, methodBody);
}

TEST (context_builder, build_class_with_parent_class) {
    auto newClass1 = std::make_shared< ClassDeclaration >();
    newClass1->setName("A");
    newClass1->setParentName("Object");
    newClass1->setConstructorBody(std::make_shared< ConstructorBody >());

    auto newClass2 = std::make_shared< ClassDeclaration >();
    newClass2->setName("B");
    newClass2->setParentName("A");
    newClass2->setConstructorBody(std::make_shared< ConstructorBody >());

    ParsedContext parsedContext;
    parsedContext.addClass(newClass1);
    parsedContext.addClass(newClass2);

    Context context;
    ContextBuilder builder;
    builder.buildAST(parsedContext, context);

    ASSERT_EQ(3, context.getClasses().size());
    auto builtClass = context.getClasses()[ClassName("B")];

    EXPECT_EQ("B", builtClass->getClassName().t);
    EXPECT_EQ("A", builtClass->getParentClass()->getClassName().t);
}
