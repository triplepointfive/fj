#include "tokenizer.h"

#include <fstream>
#include <boost/algorithm/string.hpp>

Tokenizer::Tokenizer(std::string fileName) {
    std::ifstream t(fileName);
    this->content = std::string(std::istreambuf_iterator<char>(t),
                                std::istreambuf_iterator<char>());
}

std::string Tokenizer::getNextToken() {
    return "";
}

std::vector<std::string> Tokenizer::tokenize() {
    std::vector<std::string> strs;
    boost::split(strs, content, boost::is_any_of("\t \n"));
    return strs;
}
