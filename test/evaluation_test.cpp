#include <gtest/gtest.h>

#include "eval_term_visitor.h"
#include "context.h"

using namespace fj;

TEST (Pair, setfst) {
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
    Variable *newFstVar = new Variable(newFstName);
    Variable *thisVar = new Variable(PropertyName("this"));
    Access *sndAccess = new Access(thisVar, snd);
    newPairClassArgs[fst] = newFstVar;
    newPairClassArgs[snd] = sndAccess;

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
    args[fst] = new Constructor(aClassName, MethodArguments());
    args[snd] = new Constructor(bClassName, MethodArguments());

    Constructor *basePair = new Constructor(pairClassName, args);
    MethodArguments newArgs;
    newArgs[newFstName] = new Constructor(bClassName, MethodArguments());
    Constructor *newPair = ctx.invocateMethod(basePair, setFst, newArgs);

    EXPECT_EQ(newPair->getClassName(), pairClassName);
    EXPECT_TRUE(newPair->getAttribute(fst)->isValue());
}
