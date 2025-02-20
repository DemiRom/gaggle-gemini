#ifndef __RESPONSE_PARSER_H__
#define __RESPONSE_PARSER_H__

#include "ResponseObject.h"

namespace gg::Net {
    class ResponseParser {
        public:
            ResponseParser(const std::string& base_url);

            ResponseObject_t ParseResponse(std::string& resp);
        protected:

        private:
            ResponseObject_t ParseSuccessResponse(StatusCode sc, std::string& mime_type, std::string& content);

            std::string base_url;
    };
};

#endif
