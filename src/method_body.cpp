#include "method_body.h"

#include "term.h"

MethodBody::MethodBody(Term *body, ArgumentsDeclaration declaration) {
    this->body = body;
    this->args = declaration;
}

MethodBody::~MethodBody() {
    delete body;
}

Constructor *MethodBody::invocate(Constructor *constructor,
                                  Arguments arguments) {
    return nullptr;
}
