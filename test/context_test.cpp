#include <gtest/gtest.h>

#include "context.h"

using namespace fj;

TEST (Context, includes_object_class) {
    Context context;

    ASSERT_EQ(1, context.getClasses().size());

    auto objectClass = context.getClasses()[ClassName("Object")];
    ASSERT_NE(nullptr, objectClass);
}

TEST (Context, class_has_property) {
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

TEST (Context, setfst) {
    std::shared_ptr< ObjectClassBody > objectClass =
        std::make_shared< ObjectClassBody >();
    ClassName objectClassName("Object");

    ClassName aClassName("A");
    std::shared_ptr< ClassBody > aClass = std::make_shared< ClassBody >(
        aClassName, Properties(), Methods(), objectClass
    );

    ClassName bClassName("B");
    std::shared_ptr< ClassBody > bClass = std::make_shared< ClassBody >(
        bClassName, Properties(), Methods(), objectClass
    );

    ClassName pairClassName("Pair");
    PropertyName fst("fst");
    PropertyName snd("snd");

    Properties properties;
    properties[fst] = objectClassName;
    properties[snd] = objectClassName;

    MethodName setFst("setfst");

    ArgumentsDeclaration setFstArgs;
    PropertyName newFstName("newfst");
    setFstArgs[newFstName] = objectClassName;

    MethodArguments newPairClassArgs;
    auto newFstVar = std::make_shared< Variable >(newFstName);
    auto thisVar = std::make_shared< Variable >(PropertyName("this"));
    auto sndAccess = std::make_shared< Access >(thisVar, snd);
    newPairClassArgs.push_back(newFstVar);
    newPairClassArgs.push_back(sndAccess);

    auto setFstTerm = std::make_shared< Constructor >(
        pairClassName, newPairClassArgs);

    auto setFstBody = std::make_shared< MethodBody >(
        setFst, setFstTerm, setFstArgs);

    Methods methods;
    methods[setFst] = setFstBody;

    std::shared_ptr< ClassBody > pairClass = std::make_shared< ClassBody >(
        pairClassName, properties, methods, objectClass
    );

    Context ctx;

    ctx.addClass(objectClass);
    ctx.addClass(aClass);
    ctx.addClass(bClass);
    ctx.addClass(pairClass);

    MethodArguments args;
    args.push_back(std::make_shared< Constructor >(aClassName, MethodArguments()));
    args.push_back(std::make_shared< Constructor >(bClassName, MethodArguments()));

    auto basePair = std::make_shared< Constructor >(pairClassName, args);
    MethodArguments newArgs;
    newArgs.push_back(std::make_shared< Constructor >(bClassName, MethodArguments()));
    auto newPair = ctx.invocateMethod(basePair, setFst, newArgs);

    EXPECT_EQ(newPair->getClassName(), pairClassName);

    auto attribute = ctx.getAttribute(newPair, fst);
    EXPECT_TRUE(attribute->isValue());
}
