#include "ast/builder.h"

namespace fj {
    void ContextBuilder::buildAST(ParsedContext &parsedContext,
        Context &context) {
    }

    shared_ptr<ObjectClassBody> ContextBuilder::buildClass(
        shared_ptr<ClassDeclaration> classDeclaration) {
        ClassName className(classDeclaration->getName());
        auto properties;
        for (auto elem : classDeclaration->getProperties()) {
            properties[PropertyName(elem.first)] = ClassName(elem.second);
        }

        return std::make_shared< ObjectClassBody >(
        );
    }
}
