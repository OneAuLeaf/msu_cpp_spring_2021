#include <sstream>

template <class... ArgsT>
std::string format(const std::string& fmt, ArgsT&&... args)
{
    std::vector<std::string> contents{};
    parse(contents, std::forward<ArgsT>(args)...);
    std::string res;
    bool in_brct = false;
    size_t idx = 0;
    for (size_t i = 0; i < fmt.size(); ++i) {
        switch (fmt[i])
        {
        case '{':
            if (!in_brct) {
                in_brct = true;
                res += fmt.substr(idx, i - idx);
                idx = i;
            } else  {
                throw BadFormat(" bad format at index: ", fmt, i);
            }
            break;
        case '}':
            if (in_brct) {
                in_brct = false;
                std::stringstream ss;
                size_t content_idx;
                ss << fmt.substr(idx + 1, i - idx - 1);
                ss >> content_idx;
                if (!ss.eof() || ss.str().empty()) {
                    throw BadFormat(" bad format at index: ", fmt, idx + 1);
                }

                if (content_idx >= contents.size()) {
                    throw OutOfRange(" out of range: ", fmt, content_idx);
                }
                res += contents[content_idx];

                idx = i + 1;
            } else  {
                throw BadFormat(" bad format at index: ", fmt, i);
            }
            break;
        default:
            break;
        }
    }
    if (in_brct) {
        throw BadFormat(" bad format at index: ", fmt, idx);
    }
    if (idx < fmt.size()) {
        res += fmt.substr(idx, fmt.size() - idx);
    }
    return res;
}

template <class First, class... RestT>
void parse(std::vector<std::string>& vec, First&& arg, RestT&&... rest)
{
    std::stringstream ss;
    ss << arg;
    vec.push_back(std::move(ss.str()));
    parse(vec, std::forward<RestT>(rest)...);
}