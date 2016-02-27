#include "ast/builder.h"
#include <type_traits>

using std::is_same;

namespace fj {
    ContextBuilder::ContextBuilder() {
        classTable.addClass(std::make_shared< ObjectClassBody >());
    }

    void ContextBuilder::buildAST(const ParsedContext &parsedContext,
        Context &context) {
        for (auto elem : parsedContext.getClasses()) {
            context.addClass(buildClass(elem));
        }
    }

    shared_ptr<ObjectClassBody> ContextBuilder::buildClass(
        const shared_ptr<ClassDeclaration> &classDeclaration) {
        ClassName className(classDeclaration->getName());

        std::map<PropertyName, ClassName> properties;
        for (auto elem : classDeclaration->getProperties()) {
            properties[PropertyName(elem.first)] = ClassName(elem.second);
        }

        std::map<MethodName, std::shared_ptr< MethodBody >> methods;
        for (auto elem : classDeclaration->getMethods()) {
            methods[MethodName(elem->getName())] = buildMethod(elem);
        }

        auto parentClass = classTable.getClass(
            ClassName(classDeclaration->getParentName())
        );

        return std::make_shared< ClassBody >(
            className,
            properties,
            methods,
            parentClass
        );
    }

    shared_ptr<MethodBody> ContextBuilder::buildMethod(
        const shared_ptr<MethodDeclaration> &method) {
        std::map< PropertyName, ClassName > args;
        for (auto elem : method->getArgs()) {
            args[PropertyName(elem.first)] = ClassName(elem.second);
        }
        MethodName methodName(method->getName());

        auto term = buildMethodBody(method->getBodyTerm());

        return std::make_shared< MethodBody >(
            methodName,
            term,
            args
        );
    }

    shared_ptr<Term> ContextBuilder::buildMethodBody(
        const shared_ptr<MethodTerm> &ptr) {

        return std::shared_ptr<Term>();
    }

    void LexerTermBuilder::visitTypeCastingTerm(const TypeCastingTerm *term) {
        term->getTerm()->accept(this);

        this->term = std::make_shared< Coercion >(
            ClassName(term->getName()),
            this->term
        );
    }

    void LexerTermBuilder::visitInitiation(const Initiation *initiation) {

    }

    void LexerTermBuilder::visitMethodInvocation(
        const MethodInvocation *invocation) {
        invocation->getTerm()->accept(this);

        MethodArguments args;

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

    std::vector<TermPtr> LexerTermBuilder::buildArgsList(
        const std::vector<shared_ptr<MethodTerm>> &list) {
        std::vector<TermPtr> args;
        args.reserve(list.size());

        for ( auto argTerm : list ) {
            argTerm->accept(this);
            // TODO: Verify order.
            args.push_back(this->term);
        }

        return std::move(args);
    }
}
