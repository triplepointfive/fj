#include <type_traits>
#include <iostream>

#include "ast/structures.h"
#include "ast/context_builder.h"
#include "ast/lexer_term_builder.h"

using std::is_same;

namespace fj {
    ContextBuilder::ContextBuilder() {
        classTable.addClass(std::make_shared< ObjectClassBody >());
    }

    void ContextBuilder::buildAST(const ParsedContext &parsedContext,
        Context &context) {
        for (auto elem : parsedContext.getClasses()) {
            auto newClass = buildClass(elem);
            classTable.addClass(newClass);
            context.addClass(newClass);
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
        const shared_ptr<MethodTerm> &term) {

        LexerTermBuilder builder;
        term->accept(&builder);

        return builder.getTerm();
    }
}
