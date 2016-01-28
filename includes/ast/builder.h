#include "lexer/lexer.h"
#include "ast/structures.h"

#include "context.h"

namespace fj {
    using std::shared_ptr;

    class ContextBuilder {
    public:
        void buildAST(ParsedContext & parsedContext, Context & context);
    private:
        shared_ptr< ObjectClassBody > buildClass(shared_ptr<ClassDeclaration>);
    };
}
