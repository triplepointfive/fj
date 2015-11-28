#ifndef FJ_CLASS_BODY_H
#define FJ_CLASS_BODY_H

#include <map>

#include "base_types.h"

typedef std::map<PropertyName, ClassName> Properties;

class ClassBody {
public:
    ClassBody(ClassName, Properties);

    Properties getProperties() const { return properties; }

protected:
    ClassName className;
    Properties properties;
};

#endif //FJ_CLASS_BODY_H
