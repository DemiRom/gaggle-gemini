#ifndef __RESPONSE_OBJECT_H__
#define __RESPONSE_OBJECT_H__

#include <string>
#include <vector>

namespace gg::Net {

enum StatusCode : std::uint32_t {
	// Input status codes
	INPUT_EXPECTED_BASIC = 10,
	INPUT_SENSITIVE = 11,

	// Success Status codes
	SUCCESS = 20,

	// Redirect Status codes
	TEMPORARY_REDIRECTION = 30,
	PERMANENT_REDIRECTION = 31,

	// Error status codes
	UNSPECIFIED_ERROR = 40,
	SERVER_UNAVAILABLE = 41,
	CGI_ERROR = 42,
	PROXY_ERROR = 43,
	SLOW_DOWN = 44,
	PERMANENT_FAILIURE = 50,
	NOT_FOUND = 51,
	GONE = 52,
	PROXY_REQUEST_REFUSED = 53,
	BAD_REQUEST = 59,

	// Certificate status codes
	CONTENT_REQUIRES_CERT = 60,
	CERTIFICATE_NOT_AUTH = 61,
	CERTIFICATE_NOT_VALID = 62,

	// Unknown status codes
	UNKNOWN = 99
};

enum MimeType {
	TEXT_GEMINI,
	TEXT_HTML
};

enum Lang {
	EN,
	FR,
	DE_CH,
	SR_CYR,
	ZH_HANS_CN
};

typedef struct Link {
	std::string protocol;
	std::string host;
	std::string relative_url;
	std::string url;
	size_t port;

	std::string text;
} Link_t;

typedef struct Heading {
	std::string heading_text;
} Heading_t;

typedef struct ListItem {
	std::string list_item_text;
} ListItem_t;

typedef struct QuoteItem {
	std::string quote_item_text;
} QuoteItem_t;

typedef struct ResponseHeader {
	StatusCode status_code;
	MimeType mime_type;
	Lang lang;
} ResponseHeader_t;

typedef struct ResponseContent {
	std::vector<Link_t> links;
	std::vector<Heading_t> headings;
	std::vector<ListItem_t> list_items;
	std::vector<QuoteItem_t> quote_items;
	std::string formatted_text;
	std::string text;
} ResponseContent_t;

typedef struct ResponseObject {
	ResponseHeader_t header;
	ResponseContent_t content;
} ResponseObject_t;

}; // Namespace gg::Net

#endif
