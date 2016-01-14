#ifndef FJ_ERRORS_H
#define FJ_ERRORS_H

#include "lexemes.h"

namespace fj {
    template< typename Rule >
    struct errors : public pegtl::normal< Rule >
    {
        static const std::string error_message;

        template< typename Input, typename ... States >
        static void raise(const Input & in, States && ...) {
            throw pegtl::parse_error(error_message, in);
        }
    };

    template<> const std::string errors< classes >::error_message = "Empty file";
    template<> const std::string errors< method_ret_and_name >::error_message = "invalid method defenition";
    template<> const std::string errors< sur_with_brackets<fj::method_arguments> >::error_message = "invalid method argument defenition";
    template<> const std::string errors< semicolon >::error_message = "missing semicolon";

    template<> const std::string errors< pegtl::eof >::error_message = "unexpected character";
}

#endif //FJ_ERRORS_H
