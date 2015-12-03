#include <gtest/gtest.h>

#include "eval_term_visitor.h"
#include "context.h"

TEST (Pair, setfst) {
    ObjectClassBody objectClass;
    ClassName objectClassName("Object");


    ClassName aClassName("A");
    ClassBody aClass(aClassName, Properties(), Methods(), &objectClass);


    ClassName bClassName("B");
    ClassBody bClass(bClassName, Properties(), Methods(), &objectClass);


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
    Variable newFstVar(newFstName);
    Variable thisVar(PropertyName("this"));
    Access sndAccess(&thisVar, snd);
    newPairClassArgs[fst] = &newFstVar;
    newPairClassArgs[snd] = &sndAccess;

    Constructor setFstTerm(pairClassName, newPairClassArgs);

    MethodBody setFstBody(&setFstTerm, setFstArgs);

    Methods methods;
    methods[setFst] = &setFstBody;

    ClassBody pairClass(pairClassName, properties, methods, &objectClass);

    Context ctx;

    ctx.addClass(aClassName, &aClass);
    ctx.addClass(bClassName, &bClass);
    ctx.addClass(pairClassName, &pairClass);

    EXPECT_TRUE(ctx.classHasProperty(pairClassName, fst));
    EXPECT_TRUE(ctx.classHasProperty(pairClassName, snd));
}
