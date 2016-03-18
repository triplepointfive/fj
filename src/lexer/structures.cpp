#include "lexer/structures.h"
#include "lexer/lexemes.h"
#include "lexer/lexer_term_visitor.h"

namespace fj {
    std::string MethodTerm::getName() const {
        return name;
    }

    void ConstructorBody::addProperty(const std::string &propertyName) {
        properties.push_back(propertyName);
    }

    void ConstructorBody::addSuperArg(const std::string &variableName){
        superArgs.push_back(variableName);
    }

    void MethodDeclaration::setReturnClassName(const std::string &return_class_name) {
        this->return_class_name = return_class_name;
    }

    void VariableTerm::accept(LexerTermVisitor* visitor) const {
        visitor->visitVariableTerm(this);
    }

    void AccessTerm::accept(LexerTermVisitor* visitor) const {
        visitor->visitAccessTerm(this);
    }

    void TypeCastingTerm::accept(LexerTermVisitor* visitor) const {
        visitor->visitTypeCastingTerm(this);
    }

    void MethodInvocation::accept(LexerTermVisitor* visitor) const {
        visitor->visitMethodInvocation(this);
    }

    void Initiation::accept(LexerTermVisitor* visitor) const {
        visitor->visitInitiation(this);
    }

    void AccessTerm::setTerm(std::shared_ptr<MethodTerm> methodTerm) {
        // Move it deeper in the tree if self has term.
        if (nullptr != this->methodTerm) {
            this->methodTerm->setTerm(methodTerm);
        } else {
            this->methodTerm = methodTerm;
        }
    }
}
