#ifndef FJ_CONTEXT_H
#define FJ_CONTEXT_H

#include <string>

class Context {
public:
    bool classHasProperty(std::string className, std::string propertyName)
            const;

    bool isASubtype(std::string, std::string) const;

};

#endif //FJ_CONTEXT_H
