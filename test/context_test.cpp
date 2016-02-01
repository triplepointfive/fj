#include <gtest/gtest.h>

#include "context.h"

using namespace fj;

TEST (Context, classHasProperty) {
    PropertyName fst("fst");
    PropertyName snd("snd");

    ClassName objectClassName("Object");
    ClassName pairClassName("Pair");

    Properties properties;
    properties[fst] = objectClassName;
    properties[snd] = objectClassName;
    std::shared_ptr< ObjectClassBody > objectClass =
        std::make_shared< ObjectClassBody >();

    auto pairClass = std::make_shared< ClassBody >(
        pairClassName, properties, Methods(), std::move(objectClass)
    );

    Context ctx;
    ctx.addClass(std::move(pairClass));

    EXPECT_TRUE(ctx.classHasProperty(pairClassName, fst));
    EXPECT_TRUE(ctx.classHasProperty(pairClassName, snd));
}