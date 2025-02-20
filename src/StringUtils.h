#ifndef __STRING_UTILS_H__
#define __STRING_UTILS_H__

#include <vector>
#include <string>

namespace gg::Utils {
    class StringUtils {
        public:
            static std::vector<std::string> Split(std::string& s, const std::string& delim);
    };
};

#endif
