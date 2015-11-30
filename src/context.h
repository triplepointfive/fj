#ifndef FJ_CONTEXT_H
#define FJ_CONTEXT_H

#include <string>
#include <map>

#include "term.h"
#include "class_body.h"

class Context {
public:
    void addClass(ClassName, ObjectClassBody*);

    bool classHasProperty(ClassName className, PropertyName propertyName) const;

    bool isASubtype(std::string, std::string) const;

    Constructor *invocateMethod(Constructor *, std::string, Arguments) const;

private:
    map<ClassName, ObjectClassBody*> classes;
};

#endif //FJ_CONTEXT_H
