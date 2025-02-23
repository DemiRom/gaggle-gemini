#include "StringUtils.h"
#include <sstream>

namespace gg::Utils {
	std::vector<std::string> StringUtils::Split(std::string &source, const std::string &delim){
	    std::vector<std::string> tokens;
	    std::string token;
	    size_t pos = 0;

	    while((pos = source.find(delim)) != std::string::npos) {
	        token = source.substr(0, pos);

			if(!token.empty()) {
	        	tokens.push_back(token);
			}

	        source.erase(0, pos + delim.length());
	    }

		if(!source.empty()) {
	    	tokens.push_back(source);
		}

	    return tokens;
	}

	std::vector<std::string> StringUtils::SplitWhitespace(std::string &source) {
		std::vector<std::string> tokens;
		std::string token;

		for(char ch : source) {
			if(!isspace(ch)) {
				token += ch;
			} else if (!token.empty()) {
				tokens.push_back(token);
				token.clear();
			}
		}

		if(!token.empty()) {
			tokens.push_back(token);
		}

		return tokens;
	}

	std::string StringUtils::Shift(std::vector<std::string> &source) {
		if(source.empty())
			return "";

		std::string current = source.front();
		source.erase(source.begin());
		return current;
	}

	std::string StringUtils::GetLine(std::vector<std::string> &words) {
		if(words.empty())
			return "";

		std::stringstream lineStream;

		while(!words.empty()) {
			lineStream << Shift(words) << " ";
		}

		return lineStream.str();
	}

	bool StringUtils::IsSpaceNotNewLine(char ch) {
		return ch == ' ' || ch == '\t' || ch == '\v' || ch == '\f' || ch == '\r';
	}
}
