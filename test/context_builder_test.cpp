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

    ASSERT_EQ(1, context.getClasses().size());
}
