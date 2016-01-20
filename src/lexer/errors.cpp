#include "lexer/errors.h"

namespace fj {
    template<> const std::string errors< classes >::error_message = "Empty file";

    // Identifiers.
    template<> const std::string errors< declared_class_name_padded >::error_message = "Missed class name";
    template<> const std::string errors< inherited_class_name_padded >::error_message = "Missed class name";

    // Keywords.
    template<> const std::string errors< class_keyword >::error_message = "Expected \"class\" keyword";
    template<> const std::string errors< extends_keyword >::error_message = "Expected \"extends\" keyword";

    // Chars.
    template<> const std::string errors< close_brace >::error_message = "Missing close brace '}'";
    template<> const std::string errors< open_brace >::error_message = "Missing open brace '{'";
    template<> const std::string errors< semicolon >::error_message = "Missing semicolon";

    // Stubs.
    template<> const std::string errors< constructor_body >::error_message = "stub1";
    template<> const std::string errors< class_body >::error_message = "stub2";
    template<> const std::string errors< method_ret_and_name >::error_message = "invalid method defenition";
    template<> const std::string errors< sur_with_brackets<fj::method_arguments> >::error_message = "invalid method argument defenition";

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
