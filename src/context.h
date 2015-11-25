#ifndef FJ_CONTEXT_H
#define FJ_CONTEXT_H

#include <string>

#include "term.h"

class Context {
public:
    bool classHasProperty(std::string className, std::string propertyName)
            const;

    bool isASubtype(std::string, std::string) const;

    Constructor *invocateMethod(Constructor *, std::string, Arguments) const;
};

#endif //FJ_CONTEXT_H
