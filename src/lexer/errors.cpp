#include "lexer/errors.h"

namespace fj {
    using std::string;
    template<> const string errors< classes >::error_message = "Empty file";

    // Identifiers.
    template<> const string errors< declared_class_name_padded >::error_message = "Missed class name";
    template<> const string errors< inherited_class_name_padded >::error_message = "Missed class name";

    // Keywords.
    template<> const string errors< class_keyword >::error_message = "Expected \"class\" keyword";
    template<> const string errors< extends_keyword >::error_message = "Expected \"extends\" keyword";

    // Chars.
    template<> const string errors< close_brace >::error_message = "Missing close brace '}'";
    template<> const string errors< open_brace >::error_message = "Missing open brace '{'";
    template<> const string errors< semicolon >::error_message = "Missing semicolon";

    // Terms.
    template<> const string errors< super_invocation >::error_message = "Call of super is a must";

    // Stubs.
    template<> const string errors< constructor_body >::error_message = "stub1";
    template<> const string errors< class_body >::error_message = "stub2";
    template<> const string errors< method_ret_and_name >::error_message = "invalid method defenition";
    template<> const string errors< sur_with_brackets<fj::method_arguments> >::error_message = "invalid method argument defenition";
    template<> const string errors< properties_list >::error_message = "invalid properties list";

    template<> const string errors< pegtl::eof >::error_message = "unexpected character";
}
