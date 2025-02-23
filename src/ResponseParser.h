#ifndef __RESPONSE_PARSER_H__
#define __RESPONSE_PARSER_H__

#include "ResponseObject.h"

namespace gg::Net {
	enum TokenType {
		Text,
		Link,
		Heading,
		List,
		Quote,
		PreformatToggle
	};

	typedef struct Token {
		TokenType type;
		std::string data;
	} Token_t;

    class ResponseParser {
        public:
            ResponseParser(const std::string& base_url);

            ResponseObject_t ParseResponse(std::string& resp);
        protected:

        private:
        	ResponseHeader_t ParseResponseHeader(std::string& resp);
         	ResponseContent_t ParseResponseContent(std::string& resp);

            ResponseObject_t GenerateSuccessResponse(ResponseHeader_t& header, ResponseContent_t& content);
            std::vector<Token_t> Tokenize(std::string& content);


            std::string base_url;
    };
};

#endif
