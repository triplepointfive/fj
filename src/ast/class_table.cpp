#include "ast/class_table.h"

namespace fj {
    std::shared_ptr< ObjectClassBody > ClassTable::getClass(
        const ClassName & className) const {
        auto classBody = classes.find(className)->second;
        assert(nullptr != classBody);
        return std::move(classBody);
    }

    ClassTable::ClassTable() {
        classes[ClassName("Object")] = std::make_shared< ObjectClassBody >();
    }

    void ClassTable::addClass(std::shared_ptr< ObjectClassBody > classBody) {
        classes[classBody->getClassName()] = classBody;
    }

    bool ClassTable::classHasProperty(ClassName className,
                                   PropertyName propertyName) const {
        assert((bool)classes.count(className));
        auto classBody = classes.find(className)->second;
        return (bool)classBody->getProperties().count(propertyName);
    }

    bool ClassTable::isASubtype(std::string, std::string) const {
        // TODO
        return true;
    }
}
