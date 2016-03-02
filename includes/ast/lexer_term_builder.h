#ifndef FJ_LEXER_TERM_BUILDER_H
#define FJ_LEXER_TERM_BUILDER_H

#include "lexer/lexer_term_visitor.h"
#include "lexer/lexer.h"
#include "ast/structures.h"

namespace fj {
    using std::shared_ptr;

    class LexerTermBuilder : public LexerTermVisitor {
    public:
        void visitVariableTerm(const VariableTerm *) override;
        void visitAccessTerm(const AccessTerm *) override;
        void visitMethodInvocation(const MethodInvocation *) override;
        void visitInitiation(const Initiation *) override;
        void visitTypeCastingTerm(const TypeCastingTerm *) override;
        TermPtr getTerm() { return std::move(term); };

    private:
        MethodArguments buildArgsList(
            const std::vector<std::shared_ptr< MethodTerm > > &);
        TermPtr term = { nullptr };
    };

}

#endif //FJ_LEXER_TERM_BUILDER_H
