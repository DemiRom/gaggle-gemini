#ifndef __STRING_UTILS_H__
#define __STRING_UTILS_H__

#include <vector>
#include <string>

namespace gg::Utils {
    class StringUtils {
        public:
            static std::vector<std::string> Split(std::string& source, const std::string& delim);
            static std::vector<std::string> SplitWhitespace(std::string& source);
            static std::string Shift(std::vector<std::string>& source);
            static std::string GetLine(std::vector<std::string>& words);

        private:
        	static bool IsSpaceNotNewLine(char ch);
    };
};

#endif
