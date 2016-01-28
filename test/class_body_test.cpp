#include <gtest/gtest.h>

#include "eval_term_visitor.h"
#include "context.h"

TEST (ClassBody, getMethod) {
    std::shared_ptr< ObjectClassBody > objectClass =
        std::make_shared< ObjectClassBody >();

    ClassName pairClassName("Pair");

    MethodName setFst("setfst");

    Constructor *setFstTerm = new Constructor(pairClassName, MethodArguments());
    MethodBody *setFstBody = new MethodBody(setFst, setFstTerm, ArgumentsDeclaration());

    ClassBody *pairClass = new ClassBody(pairClassName, Properties(), {{ setFst , setFstBody }}, objectClass);

    MethodBody *foundMethod = pairClass->getMethod(setFst);

    EXPECT_EQ(setFst, foundMethod->getName());
}
