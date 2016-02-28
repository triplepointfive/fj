#include "utils.h"

namespace fj {

    std::string intercalation(MethodArguments args, std::string insertion) {
        std::string acc = "";
        bool firstElement = true;

        for (auto &pair : args) {
            if (firstElement) {
                firstElement = false;
            } else {
                acc += insertion;
            }
            acc += pair->show();
        }
        return acc;
    }

}
