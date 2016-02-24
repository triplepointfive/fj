#include "lexer/lexer.h"
#include "ast/structures.h"

#include "context.h"

namespace fj {
    using std::shared_ptr;

    class TermBuilder {
    public:
        static void build(TermPtr &, const shared_ptr< MethodTerm > &);
        static void build(TermPtr &, const shared_ptr< VariableTerm > &);
        static void build(TermPtr &, const shared_ptr< AccessTerm > &);
        static void build(TermPtr &, const shared_ptr< TypeCastingTerm > &);
        static void build(TermPtr &, const shared_ptr< Initiation > &);
    };

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
