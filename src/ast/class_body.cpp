#include "ast/class_body.h"

namespace fj {
    ClassBody::ClassBody(const ClassName &className, Properties properties,
                         Methods methods, std::shared_ptr< ObjectClassBody > parentClass) :
        className(className),
        parentClass(parentClass) {
        Properties parentProperties = parentClass->getProperties();
        this->properties = properties;
        this->properties.insert(parentProperties.begin(), parentProperties.end());

        this->methods = methods;
    }

    Properties ClassBody::getProperties() const {
        return properties;
    }


    TermPtr ClassBody::getProperty(std::shared_ptr< Constructor > object,
        PropertyName propertyName) const {
        assert(className == object->getClassName());
        assert(properties.size() == object->getArgs().size());
        assert(properties.count(propertyName) != 0);

        auto i = 0;
        for (auto &elem : properties) {
            if (propertyName == elem.first) {
                return object->getArgs()[i];
            } else {
                i++;
            }
        }
    }

    std::shared_ptr< MethodBody > ClassBody::getMethod(
        MethodName methodName) const {
        auto i = methods.find(methodName);
        if (methods.end() == i) {
            return parentClass->getMethod(methodName);
        } else {
            return std::move(i->second);
        }
    }

    ClassName ClassBody::getClassName() const {
        return className;
    }
}
