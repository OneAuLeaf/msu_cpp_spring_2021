#ifndef TOKEN_PARSER_HPP
#define TOKEN_PARSER_HPP

#include <cstdint>
#include <string>
#include <functional>

namespace parse
{

using BaseCallback = std::function<void ()>;
using DigitCallback = std::function<void (uint64_t)>;
using StringCallback = std::function<void (const std::string&)>;

class TokenParser
{
public:
    TokenParser() = default;

    void SetStartCallback(const BaseCallback& callback) { start_callback_ = callback; }
    void SetEndCallback(const BaseCallback& callback) { end_callback_ = callback; }
    void SetDigitTokenCallback(const DigitCallback& callback) { digit_callback_ = callback; }
    void SetStringTokenCallback(const StringCallback& callback) { string_callback_ = callback; }

    void Parse(const std::string& text);

    ~TokenParser() = default;
private:
    BaseCallback start_callback_;
    BaseCallback end_callback_;
    DigitCallback digit_callback_;
    StringCallback string_callback_;
};

}

#endif
