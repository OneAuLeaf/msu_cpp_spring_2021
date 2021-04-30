#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <stdexcept>
#include <string>

class BadFormat : public std::invalid_argument
{
public:
    BadFormat(const std::string& what_str, const std::string& src, size_t err_idx) : 
            std::invalid_argument(src + what_str + std::to_string(err_idx)) {}
};

class OutOfRange : public std::out_of_range
{
public:
    OutOfRange(const std::string& what_str, const std::string& src, size_t err_idx) : 
            std::out_of_range(src + what_str + std::to_string(err_idx)) {}
};

#endif