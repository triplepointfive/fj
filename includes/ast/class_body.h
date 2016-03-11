#ifndef FJ_CLASS_BODY_H
#define FJ_CLASS_BODY_H

#include <map>
#include <memory>

#include "base_types.h"
#include "method_body.h"

namespace  fj {
    using Properties = std::map<PropertyName, ClassName>;
    using Methods = std::map<MethodName, std::shared_ptr< MethodBody >>;

    class ObjectClassBody {
    public:
        virtual Properties getProperties() const { return Properties(); }
        virtual std::shared_ptr< MethodBody > getMethod(MethodName) const {
            return nullptr;
        }
        virtual TermPtr getProperty(std::shared_ptr< Constructor >, PropertyName) const {
            return nullptr;
        }
        virtual std::shared_ptr< ObjectClassBody > getParentClass() const {
            return std::make_shared< ObjectClassBody >();
        }
        virtual ClassName getClassName() const { return ClassName("Object"); }
        virtual ~ObjectClassBody() {}
    };

    class ClassBody : public ObjectClassBody {
    public:
        ClassBody(const ClassName&, Properties, Methods,
                  std::shared_ptr< ObjectClassBody >);

        Properties getProperties() const override;
        std::shared_ptr< MethodBody > getMethod(MethodName) const override;
        ClassName getClassName() const override;
        TermPtr getProperty(std::shared_ptr< Constructor >, PropertyName) const override;
        std::shared_ptr< ObjectClassBody > getParentClass() const override {
            return std::move(parentClass);
        }

    protected:
        ClassName className;
        Properties properties;
        Methods methods;
        std::shared_ptr< ObjectClassBody > parentClass;
    };
}

#endif //FJ_CLASS_BODY_H
