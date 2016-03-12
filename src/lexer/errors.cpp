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
    template<> const string errors< close_bracket >::error_message = "Missing close brace ')'";
    template<> const string errors< open_bracket >::error_message = "Missing open brace '('";
    template<> const string errors< close_brace >::error_message = "Missing close brace '}'";
    template<> const string errors< open_brace >::error_message = "Missing open brace '{'";
    template<> const string errors< semicolon >::error_message = "Missing semicolon";
    template<> const string errors< space >::error_message = "Terms must be separated";

    // Terms.
    template<> const string errors< super_invocation >::error_message = "Call of super is a must";
    template<> const string errors< returned_statement >::error_message = "Expected a term";
    template<> const string errors< return_stat >::error_message = "Method body is missing";
    template<> const string errors< class_name >::error_message = "Must be valid class name";
    template<> const string errors< object_name >::error_message = "Must be valid variable name";

    // Stubs.
    template<> const string errors< constructor_body >::error_message = "stub1";
    template<> const string errors< class_body >::error_message = "stub2";
    template<> const string errors< pegtl::not_at<fj::keyword>  >::error_message = "stub3";
    template<> const string errors< pegtl::plus<pegtl::ascii::identifier> >::error_message = "stub4";
    template<> const string errors< method_arguments >::error_message = "stub5";
    template<> const string errors< super_args >::error_message = "stub6";
    template<> const string errors< method_list_of_args >::error_message = "stub7";
    template<> const string errors< type_casting_class >::error_message = "stub8";
    template<> const string errors< seps >::error_message = "stub8";

    template<> const string errors< method_ret_and_name >::error_message = "invalid method defenition";
    template<> const string errors< method_head >::error_message = "invalid method_head";
    template<> const string errors< sur_with_braces< method_body > >::error_message = "invalid method_body";
    template<> const string errors< sur_with_brackets<fj::method_arguments> >::error_message = "invalid method argument defenition";
    template<> const string errors< properties_list >::error_message = "invalid properties list";

    template<> const string errors< method_body >::error_message = "invalid method body";
    template<> const string errors< pegtl::eof >::error_message = "unexpected character";
}
