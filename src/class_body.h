#ifndef FJ_CLASS_BODY_H
#define FJ_CLASS_BODY_H

#include <map>

#include "base_types.h"
#include "method_body.h"

typedef std::map<PropertyName, ClassName> Properties;
typedef std::map<MethodName, MethodBody*> Methods;

class ObjectClassBody {
public:
    virtual Properties getProperties() const { return Properties(); }
    virtual Methods getMethods() const { return Methods(); }
};

class ClassBody : public ObjectClassBody {
public:
    ClassBody(ClassName, Properties, Methods, ObjectClassBody*);

    Properties getProperties() const override;
    Methods getMethods() const override;

protected:
    ClassName className;
    Properties properties;
    Methods methods;
    ObjectClassBody *parentClass;
    // TODO: Cleanup methods bodies
};

#endif //FJ_CLASS_BODY_H
