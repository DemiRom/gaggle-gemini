#ifndef __RESPONSE_OBJECT_H__
#define __RESPONSE_OBJECT_H__

#include <string>
#include <vector>

namespace gg::Net {

enum StatusCode : std::uint32_t {
    //Input status codes
    INPUT_EXPECTED_BASIC  = 10,
    INPUT_SENSITIVE       = 11,

    //Success Status codes
    SUCCESS               = 20,

    //Redirect Status codes
    TEMPORARY_REDIRECTION = 30,
    PERMANENT_REDIRECTION = 31,

    //Error status codes
    UNSPECIFIED_ERROR     = 40,
    SERVER_UNAVAILABLE    = 41,
    CGI_ERROR             = 42,
    PROXY_ERROR           = 43,
    SLOW_DOWN             = 44,
    PERMANENT_FAILIURE    = 50,
    NOT_FOUND             = 51,
    GONE                  = 52,
    PROXY_REQUEST_REFUSED = 53,
    BAD_REQUEST           = 59,

    //Certificate status codes
    CONTENT_REQUIRES_CERT = 60,
    CERTIFICATE_NOT_AUTH  = 61,
    CERTIFICATE_NOT_VALID = 62,

    //Unknown status codes
    UNKNOWN               = 99
};

typedef struct Link {
    std::string base_url;
    std::string relative_url;
    std::string link_url;
    std::string pretty_name;
} Link_t;

typedef struct ResponseObject {
    StatusCode status_code;
    std::string mime_type;
    std::vector<Link_t> links;

    std::string content;
    size_t content_length;
} ResponseObject_t;

}; //Namespace gg::Net

#endif
