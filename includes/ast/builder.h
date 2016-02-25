#include "lexer/lexer.h"
#include "ast/structures.h"

#include "context.h"

#include "lexer/lexer_term_visitor.h"

namespace fj {
    using std::shared_ptr;

    class LexerTermBuilder : LexerTermVisitor {
    public:
        void visitVariableTerm(const VariableTerm *) override;
        void visitAccessTerm(const AccessTerm *) override;
        void visitMethodInvocation(const MethodInvocation *) override;
        void visitInitiation(const Initiation *) override;
        void visitTypeCastingTerm(const TypeCastingTerm *) override;
        TermPtr getTerm() { return std::move(term); };

    private:
        TermPtr term = { nullptr };
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
