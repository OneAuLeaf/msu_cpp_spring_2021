#ifndef FORMAT_HPP
#define FORMAT_HPP

#include <string>
#include <vector>

#include "Exceptions.hpp"

template <class... ArgsT>
std::string format(const std::string&, ArgsT&&...);

template <class First, class... RestT>
void parse(std::vector<std::string>&, First&&, RestT&&...);
void parse(std::vector<std::string>&) {}

#include "Format.tpp"

#endif