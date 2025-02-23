#include "ResponseParser.h"
#include "ResponseObject.h"
#include "StringUtils.h"
#include <sstream>
#include <cassert>
#include <exception>
#include <string>
#include <iostream>
#include <sys/socket.h>
#include <vector>

namespace gg::Net {

ResponseParser::ResponseParser(const std::string& base_url) : base_url(base_url) { }

ResponseObject_t ResponseParser::ParseResponse(std::string &resp) {
	auto responseHeader = ParseResponseHeader(resp);
    switch(responseHeader.status_code) {
        case INPUT_EXPECTED_BASIC:
            break;
        case INPUT_SENSITIVE:
            break;
        case SUCCESS:
       	{
        	auto responseContent = ParseResponseContent(resp);
            return GenerateSuccessResponse(responseHeader, responseContent);
       	}
        break;

        case TEMPORARY_REDIRECTION:
            break;
        case PERMANENT_REDIRECTION:
            break;
        case UNSPECIFIED_ERROR:
            break;
        case SERVER_UNAVAILABLE:
            break;
        case CGI_ERROR:
            break;
        case PROXY_ERROR:
            break;
        case SLOW_DOWN:
            break;
        case PERMANENT_FAILIURE:
            break;
        case NOT_FOUND:
            break;
        case GONE:
            break;
        case PROXY_REQUEST_REFUSED:
            break;
        case BAD_REQUEST:
            break;
        case CONTENT_REQUIRES_CERT:
            break;
        case CERTIFICATE_NOT_AUTH:
            break;
        case CERTIFICATE_NOT_VALID:
            break;
        case UNKNOWN:
            break;
    }

    return {};
}

ResponseHeader_t ResponseParser::ParseResponseHeader(std::string& resp) {
	std::string responseCopy = resp;

	auto responseLines = Utils::StringUtils::Split(responseCopy, "\r\n");

	assert(responseLines.size() > 0);

	auto responseHeaderText = responseLines[0];
	auto responseHeaderWords = Utils::StringUtils::SplitWhitespace(responseLines[0]);

	assert(responseHeaderWords.size() > 1);

	StatusCode statusCode = StatusCode::UNSPECIFIED_ERROR;
	MimeType mimeType = MimeType::TEXT_GEMINI;
	Lang lang = Lang::EN;

	try {
		statusCode = static_cast<StatusCode>(std::stoi(responseHeaderWords[0]));
	} catch (std::exception& e) {
		std::cerr << "Could not parse status code: " << e.what() << " " << __FILE__ << ":" << __LINE__ << std::endl;
	}

	return {
		.status_code = statusCode,
		.mime_type = mimeType,
		.lang = lang
	};
}

ResponseContent_t ResponseParser::ParseResponseContent(std::string& resp) {
	std::vector<Heading_t> headings;
	std::vector<Link_t> links;
	std::vector<ListItem_t> listItems;
	std::vector<QuoteItem_t> quoteItems;
	std::string formattedText;
	std::string text;

	auto responseCopy = resp;


	auto responseSplit = Utils::StringUtils::Split(responseCopy, "\r\n");
	assert(responseSplit.size() > 0);
	responseSplit.erase(responseSplit.begin());

	responseCopy.clear();

	for(const auto& line : responseSplit)
		responseCopy += line;


    auto tokens = Tokenize(responseCopy);

    for(Token_t token : tokens) {
    	switch(token.type) {
		   	case Text:
				text = token.data;
			break;
			case Link:
				links.push_back({
					.link_url = token.data
				});
			break;
			case Heading:
				headings.push_back({
					.heading_text = token.data
				});
			break;
			case List:
				listItems.push_back({
					.list_item_text = token.data
				});
			break;
			case Quote:
				quoteItems.push_back({
					.quote_item_text = token.data
				});
			break;
			case PreformatToggle:
				formattedText = token.data;
			break;
		}
    }

	return {
		.headings = headings,
		.links = links,
		.list_items = listItems,
		.quote_items = quoteItems,
		.formatted_text = formattedText,
		.text = text
	};
}

ResponseObject_t ResponseParser::GenerateSuccessResponse(ResponseHeader_t &header, ResponseContent_t &content) {
    return {
    	.header = header,
     	.content = content
    };
}

std::vector<Token_t> ResponseParser::Tokenize(std::string& source) {
	std::vector<Token_t> tokens;

	std::vector<std::string> words = Utils::StringUtils::SplitWhitespace(source);

	for(auto word : words) {
		std::cout << "DEBUG: " << word << std::endl;
	}

	while(!words.empty()) {
		if(words.front() == "=>") {
			// Link
			tokens.push_back({
				.type = TokenType::Link,
				.data = Utils::StringUtils::GetLine(words)
			});
		} else if (words.front() == "#") {
			// Heading
			tokens.push_back({
				.type = TokenType::Heading,
				.data = Utils::StringUtils::GetLine(words)
			});
		} else if (words.front() == "*") {
			// List
			tokens.push_back({
				.type = TokenType::List,
				.data = Utils::StringUtils::GetLine(words)
			});
		} else if (words.front() == ">") {
			// Quote
			tokens.push_back({
				.type = TokenType::Quote,
				.data = Utils::StringUtils::GetLine(words)
			});
		} else if (words.front() == "```") {
			// Preformatted text

			// Remove the first ```
		  	Utils::StringUtils::Shift(words);

			std::stringstream formattedText;

			while(words.front() != "```") {
				formattedText << Utils::StringUtils::Shift(words);
			}

			// Remove the last ```
			Utils::StringUtils::Shift(words);

			tokens.push_back({
				.type = TokenType::PreformatToggle,
				.data = formattedText.str()
			});

		} else {
			// Text
			tokens.push_back({
				.type = TokenType::Text,
				.data = Utils::StringUtils::GetLine(words)
			});
		}

	}

	return tokens;
}

} //namespace gg::Net
