#ifndef FJ_CONTEXT_BUILDER_H
#define FJ_CONTEXT_BUILDER_H

#include "lexer/lexer.h"

#include "context.h"

namespace fj {
    using std::shared_ptr;

    class ContextBuilder {
    public:
        ContextBuilder();
        void buildAST(const ParsedContext &, Context &);
    private:
        shared_ptr< ObjectClassBody > buildClass(const shared_ptr <ClassDeclaration >&);
        shared_ptr< MethodBody > buildMethod(const shared_ptr< MethodDeclaration >&);
        shared_ptr< Term > buildMethodBody(const std::shared_ptr< MethodTerm >&);
        ClassTable classTable;
    };
}

#endif //FJ_CONTEXT_BUILDER_H
