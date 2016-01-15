#ifndef FJ_ERRORS_H
#define FJ_ERRORS_H

#include "lexemes.h"

namespace fj {
    // TODO: Wipe them out.
    void splitOn(std::string origin, std::string delimiter,
                 std::string &fst, std::string &snd);

    void splitOnSpace(std::string origin, std::string &fst, std::string &snd);

    void splitOnDot(std::string origin, std::string &fst, std::string &snd);

    template< typename Rule >
    struct errors : public pegtl::normal< Rule > {
        static const std::string error_message;

        template< typename Input, typename ... States >
        static void raise(const Input & in, States && ...) {
            throw pegtl::parse_error(error_message, in);
        }
    };

}

#endif //FJ_ERRORS_H
