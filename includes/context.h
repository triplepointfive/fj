#ifndef FJ_CONTEXT_H
#define FJ_CONTEXT_H

#include <string>
#include <map>

#include "term.h"
#include "class_body.h"

class Context {
public:
    void addClass(ObjectClassBody*);

    void setVariables(map<PropertyName, Constructor *>);

    bool classHasProperty(ClassName className, PropertyName propertyName) const;

    bool isASubtype(std::string, std::string) const;

    Constructor *invocateMethod(Constructor *, MethodName, MethodArguments);

    Constructor *assignedValue(PropertyName) const;

    virtual ~Context();

private:
    map<ClassName, ObjectClassBody*> classes;
    map<PropertyName, Constructor *> assigned_variables;
};

#endif //FJ_CONTEXT_H
