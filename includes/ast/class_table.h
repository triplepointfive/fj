#ifndef FJ_CLASS_TABLE_H
#define FJ_CLASS_TABLE_H

#include <memory>
#include <map>

#include "base_types.h"
#include "class_body.h"

namespace fj {
    class ClassTable {
    public:
        ClassTable();

        std::shared_ptr< ObjectClassBody > getClass(
            const ClassName & className) const;
        void addClass(std::shared_ptr< ObjectClassBody > classBody);

        bool classHasProperty(ClassName className,
            PropertyName propertyName) const;

        bool isASubtype(std::string, std::string) const;

    private:
        std::map< ClassName, std::shared_ptr< ObjectClassBody > > classes;
    };
}

#endif //FJ_CLASS_TABLE_H
