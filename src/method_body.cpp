#include "method_body.h"

#include "context.h"

MethodBody::MethodBody(Term *body, ArgumentsDeclaration declaration) {
    this->body = body;
    this->args = declaration;
}

MethodBody::~MethodBody() {
    delete body;
}

Constructor *MethodBody::invocate(Constructor *constructor,
                                  MethodArguments arguments,
                                  Context *context) {

    return nullptr;
}
