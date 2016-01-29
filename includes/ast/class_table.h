#include <memory>
#include <map>

#include "base_types.h"
#include "class_body.h"

namespace fj {
    class ClassTable {
    public:
        std::shared_ptr< ObjectClassBody > getClass(
            const ClassName & className) const {
            return std::move(classes.find(className)->second);
        }

        void addClass(std::shared_ptr< ObjectClassBody > classBody) {
            classes[classBody->getClassName()] = classBody;
        }
    private:
        std::map< ClassName, std::shared_ptr< ObjectClassBody > > classes;
    };
}

