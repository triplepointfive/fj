#include <gtest/gtest.h>
#include "ast_struct.h"

TEST (Ast, hello) {
    std::string name;
    std::string input = "Hello, Ilya!";
    pegtl::parse< hello::grammar, hello::action >(input.c_str(), input.size(), "input variable", name);
    std::cout << "Good bye, " << name << "!" << std::endl;
}
