#ifndef FJ_INTERACTIVE_COMMAND_H
#define FJ_INTERACTIVE_COMMAND_H

#include <string>
#include <memory>

namespace fj {
    class InteractiveMode;

    class InteractiveCommand {
    public:
        InteractiveCommand(std::shared_ptr< InteractiveMode > mode,
                std::string name, std::string doc)
            : mode(mode)
            , name(name)
            , doc(doc)
            {}

        virtual ~InteractiveCommand() = default;

        virtual int execute(std::string) = 0;
        const std::string &getName() const { return name; };
        const std::string &getDoc()  const { return doc;  };
    protected:
        std::shared_ptr< InteractiveMode > mode{ nullptr };
        std::string name;
        std::string doc;
    };
}

#endif //FJ_INTERACTIVE_COMMAND_H
