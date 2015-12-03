#include "method_body.h"

#include "context.h"
#include "eval_term_visitor.h"

MethodBody::MethodBody(Term *body, ArgumentsDeclaration declaration) {
    this->body = body;
    this->args = declaration;
}

MethodBody::~MethodBody() {
    delete body;
}

Constructor *MethodBody::invocate(Constructor *constructor,
                                  MethodArguments &arguments,
                                  Context *context) {
    assert(arguments.size() == args.size());
    for (auto elem : args) {
        assert((bool)arguments.count(elem.first));
        assert(arguments.find(elem.first)->second->getClassName()
               == elem.second);
    }



    return nullptr;
}
