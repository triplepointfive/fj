#include "lexer/errors.h"

namespace fj {
    template<> const std::string errors< classes >::error_message = "Empty file";
    template<> const std::string errors< method_ret_and_name >::error_message = "invalid method defenition";
    template<> const std::string errors< sur_with_brackets<fj::method_arguments> >::error_message = "invalid method argument defenition";
    template<> const std::string errors< semicolon >::error_message = "missing semicolon";

    template<> const std::string errors< pegtl::eof >::error_message = "unexpected character";

    void splitOn(std::string origin, std::string delimiter,
                 std::string &fst, std::string &snd) {
        unsigned long space = origin.find(delimiter);
        fst = origin.substr(0, space);
        snd = origin.substr(space + 1, origin.size());
    }

    void splitOnSpace(std::string origin, std::string &fst, std::string &snd) {
        // TODO: Assumes delimiter is a space, should be more abstract.
        splitOn(origin, " ", fst, snd);
    }

    void splitOnDot(std::string origin, std::string &fst, std::string &snd) {
        splitOn(origin, ".", fst, snd);
    }
}
