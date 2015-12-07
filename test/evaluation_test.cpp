#include <gtest/gtest.h>

#include "eval_term_visitor.h"
#include "context.h"

TEST (Pair, setfst) {
    ObjectClassBody *objectClass = new ObjectClassBody();
    ClassName objectClassName("Object");


    ClassName aClassName("A");
    ClassBody *aClass = new ClassBody(aClassName, Properties(), Methods(), objectClass);


    ClassName bClassName("B");
    ClassBody *bClass = new ClassBody(bClassName, Properties(), Methods(), objectClass);


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

    Constructor *setFstTerm = new Constructor(pairClassName, newPairClassArgs);

    MethodBody *setFstBody = new MethodBody(setFst, setFstTerm, setFstArgs);

    Methods methods;
    methods[setFst] = setFstBody;

    ClassBody *pairClass = new ClassBody(pairClassName, properties, methods, objectClass);

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
