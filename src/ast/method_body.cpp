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

    Constructor *MethodBody::invocate(Constructor *constructor,
                                      MethodArguments &arguments,
                                      Context *context) {
        assert(arguments.size() == args.size());
        for (auto elem : args) {
            assert((bool)arguments.count(elem.first));
            // TODO: Validate type casting instead
    //        assert(arguments.find(elem.first)->second->getClassName() == elem.second);
        }
        map<PropertyName, Constructor *> calculatedArgs;
        for (auto elem : arguments) {
            EvalTermVisitor evaluator(*context);
            elem.second->accept(evaluator);
            calculatedArgs[elem.first] = evaluator.getCalculatedValue();
        }
        context->setVariables(calculatedArgs);
        EvalTermVisitor evaluator(*context);
        constructor->accept(evaluator);
        return evaluator.getCalculatedValue();
    }

}
