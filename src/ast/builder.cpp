#include "ast/builder.h"

namespace fj {

    void TermBuilder::build(TermPtr &, const shared_ptr< MethodTerm > &) {
        throw "Build is called for base type!";
    }

    void TermBuilder::build(TermPtr &term,
                            const shared_ptr< VariableTerm > &parsedTerm) {
        term = std::make_shared< Variable >(
            PropertyName(parsedTerm->getName())
        );
    }

    void TermBuilder::build(TermPtr  &term,
                            const shared_ptr< AccessTerm > &parsedTerm) {
        TermPtr accessor;
        build(accessor, parsedTerm->getTerm());

        term = std::make_shared< Access >(
            accessor,
            PropertyName(parsedTerm->getName())
        );
    }

    void TermBuilder::build(TermPtr &term,
                            const shared_ptr< TypeCastingTerm > &parsedTerm) {

    }

    void TermBuilder::build(TermPtr &term,
                            const shared_ptr< Initiation > &parsedTerm) {

    }

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
}
