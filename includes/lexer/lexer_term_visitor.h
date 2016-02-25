#ifndef FJ_TERM_VISITOR_H
#define FJ_TERM_VISITOR_H

namespace fj {
    class VariableTerm;
    class AccessTerm;
    class TypeCastingTerm;
    class MethodInvocation;
    class Initiation;

    class LexerTermVisitor {
    public:
        virtual ~LexerTermVisitor() { }

        virtual void visitVariableTerm(const VariableTerm *) = 0;
        virtual void visitAccessTerm(const AccessTerm *) = 0;
        virtual void visitMethodInvocation(const MethodInvocation *) = 0;
        virtual void visitInitiation(const Initiation *) = 0;
        virtual void visitTypeCastingTerm(const TypeCastingTerm *) = 0;
    };

}

#endif //FJ_TERM_VISITOR_H
