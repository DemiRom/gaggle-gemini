#include "StringUtils.h"

std::vector<std::string> gg::Utils::StringUtils::Split(std::string &s, const std::string &delim){
    std::vector<std::string> tokens;

    size_t pos = 0;

    std::string token;

    while((pos = s.find(delim)) != std::string::npos) {
        token = s.substr(0, pos);
        tokens.push_back(token);
        s.erase(0, pos + delim.length());
    }

    tokens.push_back(s);

    return tokens;
}
