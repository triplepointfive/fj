#include "lexer/lexer.h"
#include "ast/structures.h"

#include "context.h"

namespace fj {
    using std::shared_ptr;

    class ContextBuilder {
    public:
        ContextBuilder();
        void buildAST(const ParsedContext &, Context &);
    private:
        shared_ptr< ObjectClassBody > buildClass(const shared_ptr<ClassDeclaration>&);
        ClassTable classTable;
    };
}
