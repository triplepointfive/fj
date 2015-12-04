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
    virtual MethodBody *getMethod(MethodName) const { return nullptr; }
    virtual ClassName getClassName() const { return ClassName("Object"); }
    virtual ~ObjectClassBody() {}
};

class ClassBody : public ObjectClassBody {
public:
    ClassBody(ClassName, Properties, Methods, ObjectClassBody*);

    Properties getProperties() const override;
    MethodBody *getMethod(MethodName) const override;
    ClassName getClassName() const override;

    virtual ~ClassBody();

protected:
    ClassName className;
    Properties properties;
    Methods methods;
    ObjectClassBody *parentClass;
};

#endif //FJ_CLASS_BODY_H
