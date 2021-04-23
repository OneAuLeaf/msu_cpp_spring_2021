#include <string>
#include <sstream>

#include "Serializer.hpp"

Serializer::Serializer(std::ostream& out) : out_{out} {}

Error Serializer::process(bool obj)
{
    if (obj) {
        out_ << "true";
    } else {
        out_ << "false";
    }
    out_ << sep;
    return Error::NoError;
}

Error Serializer::process(uint64_t obj)
{
    out_ << obj << sep;
    return Error::NoError;
}

Deserializer::Deserializer(std::istream& in) : in_{in} {}

Error Deserializer::process(uint64_t& obj)
{
    std::string text;
    in_ >> text;

    std::stringstream ss;
    uint64_t tmp;
    ss << text;
    ss >> tmp;
    if (!ss.eof() || text.empty()) {
        return Error::CorruptedArchive;
    }

    obj = tmp;
    return Error::NoError;
}

Error Deserializer::process(bool& obj)
{
    std::string text;
    in_ >> text;

    if (text == "true") {
        obj = true;
    }
    else if (text == "false") {
        obj = false;
    }
    else {
        return Error::CorruptedArchive;
    }

    return Error::NoError;
}