#include "ast/class_table.h"

namespace fj {
    std::shared_ptr< ObjectClassBody > ClassTable::getClass(
        const ClassName & className) const {
        return std::move(classes.find(className)->second);
    }

    void ClassTable::addClass(std::shared_ptr< ObjectClassBody > classBody) {
        classes[classBody->getClassName()] = classBody;
    }
}
