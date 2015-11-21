#include "utils.h"

std::string intercalation(Arguments args, std::string insertion) {
    std::string acc = "";
    bool firstElement = true;

    for (auto &element : args) {
        if (firstElement) {
            firstElement = false;
        } else {
            acc += insertion;
        }
        acc += element->show();
    }
    return acc;
}
