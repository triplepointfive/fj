#ifndef FJ_CONTEXT_BUILDER_H
#define FJ_CONTEXT_BUILDER_H

#include "lexer/structures.h"

#include "context.h"

namespace fj {
    using std::shared_ptr;

    class ClassTable;
    class MethodBody;

    class ContextBuilder {
    public:
        ContextBuilder();
        void buildAST(const ParsedContext &);
        std::shared_ptr< ClassTable > getClassTable();

    private:
        shared_ptr< ObjectClassBody > buildClass(const shared_ptr <ClassDeclaration >&);
        shared_ptr< MethodBody > buildMethod(const shared_ptr< MethodDeclaration >&);
        shared_ptr< Term > buildMethodBody(const std::shared_ptr< MethodTerm >&);
        std::shared_ptr< ClassTable > classTable{ nullptr };
    };
}

#endif //FJ_CONTEXT_BUILDER_H
