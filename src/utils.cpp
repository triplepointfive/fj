#include "utils.h"

std::string intercalation(Arguments args, std::string insertion) {
    std::string acc = "";
    bool firstElement = true;

    for (auto &pair : args) {
        if (firstElement) {
            firstElement = false;
        } else {
            acc += insertion;
        }
        acc += pair.second->show();
    }
    return acc;
}
