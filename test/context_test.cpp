#include <gtest/gtest.h>

#include "context.h"

TEST (Context, classHasProperty) {
    PropertyName fst("fst");
    PropertyName snd("snd");

    ClassName objectClassName("Object");
    ClassName pairClassName("Pair");

    Properties properties;
    properties[fst] = objectClassName;
    properties[snd] = objectClassName;
    ObjectClassBody objectClass;

    ClassBody pairClass(pairClassName, properties, Methods(), &objectClass);

    Context ctx;
    ctx.addClass(&pairClass);

    EXPECT_TRUE(ctx.classHasProperty(pairClassName, fst));
    EXPECT_TRUE(ctx.classHasProperty(pairClassName, snd));
}