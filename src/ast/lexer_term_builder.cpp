#include "ast/lexer_term_builder.h"

namespace fj {

    MethodArguments LexerTermBuilder::buildArgsList(
        const std::vector<shared_ptr<MethodTerm>> &list) {
        std::vector<TermPtr> args;
        args.reserve(list.size());

        for ( auto argTerm : list ) {
            argTerm->accept(this);
            // TODO: Verify order.
            args.push_back(this->term);
        }

        return args;
    }

    void LexerTermBuilder::visitTypeCastingTerm(const TypeCastingTerm *term) {
        term->getTerm()->accept(this);

        this->term = std::make_shared< Coercion >(
            ClassName(term->getName()),
            this->term
        );
    }

    void LexerTermBuilder::visitInitiation(const Initiation *initiation) {
        auto terms = buildArgsList(initiation->getArgs());
        this->term = std::make_shared< Constructor >(
            ClassName(initiation->getName()),
            terms
        );
    }

    void LexerTermBuilder::visitMethodInvocation(
        const MethodInvocation *invocation) {
        MethodArguments args = buildArgsList(invocation->getArgs());
        invocation->getTerm()->accept(this);

        this->term = std::make_shared< Invocation >(
            this->term,
            MethodName(invocation->getName()),
            args
        );
    }

    void LexerTermBuilder::visitAccessTerm(const AccessTerm *term) {
        term->getTerm()->accept(this);

        this->term = std::make_shared< Access >(
            this->term,
            PropertyName(term->getName())
        );
    }

    void LexerTermBuilder::visitVariableTerm(const VariableTerm *term) {
        this->term = std::make_shared< Variable >(
            PropertyName(term->getName())
        );
    }

}
