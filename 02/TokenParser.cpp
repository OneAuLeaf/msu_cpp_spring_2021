#include <cstdlib>
#include <cerrno>
#include <sstream>

#include "TokenParser.hpp"

namespace parse
{

void TokenParser::Parse(const std::string& text)
{
    if (start_callback_) {
        start_callback_();
    }

    std::istringstream ss(text);
    std::string token;
    while (ss >> token) {
        errno = 0;
        char *eptr;
        auto res = strtoull(token.c_str(), &eptr, 10);
        if (errno || eptr == token.c_str() || *eptr 
                || res != (uint64_t) res || token[0] == '+' || token[0] == '-') {
            if (string_callback_) {
                string_callback_(token);
            }
        } else {
            uint64_t digit = (uint64_t) res;
            if (digit_callback_) {
                digit_callback_(digit);
            }
        }
    }

    if (end_callback_) {
        end_callback_();
    }
}

}