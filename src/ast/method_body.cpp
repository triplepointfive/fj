#include "ast/method_body.h"

#include "context.h"
#include "eval_term_visitor.h"

namespace fj {

    MethodBody::MethodBody(const MethodName & name,
                           std::shared_ptr< Term > body,
                           ArgumentsDeclaration declaration) :
        name(name),
        body(body),
        args(declaration) {}

    std::shared_ptr< Constructor > MethodBody::invocate(
        std::shared_ptr< Constructor > constructor,
        MethodArguments &arguments,
        Context *context) {
        assert(arguments.size() == args.size());
        /* for (auto elem : args) { */
            /* assert((bool)arguments.count(elem.first)); */
            // TODO: Validate type casting instead
    //        assert(arguments.find(elem.first)->second->getClassName() == elem.second);
        /* } */
        std::map<PropertyName, std::shared_ptr< Constructor >> calculatedArgs;

        std::vector< TermPtr >::size_type i = 0;
        for(auto argDeclaration : args) {
            auto elem = arguments[i];
            i++;
            EvalTermVisitor evaluator(*context);
            elem->accept(evaluator);
            calculatedArgs[argDeclaration.first] = evaluator.getCalculatedValue();
        }

        context->setVariables(calculatedArgs);
        EvalTermVisitor evaluator(*context);
        constructor->accept(evaluator);
        return evaluator.getCalculatedValue();
    }

}
