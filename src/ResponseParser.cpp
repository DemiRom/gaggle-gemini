#include "ResponseParser.h"
#include "ResponseObject.h"
#include "StringUtils.h"
#include <regex>
#include <sstream>
#include <cassert>
#include <exception>
#include <string>
#include <iostream>
#include <sys/socket.h>
#include <vector>

namespace gg::Net {

ResponseParser::ResponseParser(const std::string& host, const std::string& relative_url) : host(host), relative_url(relative_url) { }

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
        {
        	ResponseContent_t emptyResponse;
        	return GenerateSuccessResponse(responseHeader, emptyResponse);
        }
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

	StatusCode statusCode = StatusCode::UNKNOWN;
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
				text += token.data;
			break;
			case Link:
			{
				std::string linkText = token.data;

				auto linkTextSplit = Utils::StringUtils::SplitWhitespace(linkText);

				std::stringstream urlStream;
				std::stringstream linkTextStream;

				assert(!linkTextSplit.empty());

				for(size_t i = 1; i < linkTextSplit.size(); i++) {
					linkTextStream << linkTextSplit[i] << " ";
				}

				if(linkTextSplit[0].find("gemini://") != std::string::npos ||
					linkTextSplit[0].find("http://") != std::string::npos ||
					linkTextSplit[0].find("https://") != std::string::npos
				) {
					urlStream << linkTextSplit[0];

					std::string url = linkTextSplit[0];

					auto urlSplit = Utils::StringUtils::Split(url, "/");

					assert(!urlSplit.empty());

					// Remove the empty elements
					// urlSplit.erase(std::remove_if(urlSplit.begin(), urlSplit.end(), [](std::string& it) {
					// 	return it.empty();
					// }));

					std::string protocol = urlSplit[0] + "//";
					std::string host     = urlSplit[1];

					size_t port = 1965;

					if(linkTextSplit[0].find("http://") != std::string::npos)
						port = 80;
					else if(linkTextSplit[0].find("https://") != std::string::npos)
						port = 443;

					//TODO Check if port is specified in the url if it is overwrite the port number

					std::stringstream relativeUrlStream;

					for(size_t i = 2; i < urlSplit.size(); i++){
						relativeUrlStream << urlSplit[i] << "/";
					}

					std::string relativeUrl = relativeUrlStream.str();

					//Remove the last / from the for loop
					if(*(relativeUrl.end() - 1) == '/') {
						relativeUrl.erase(relativeUrl.end() - 1);
					}

					links.push_back({
						.protocol = protocol,
						.url = urlStream.str(),
						.host = host,
						.relative_url = relativeUrl,
						.port = port,
						.text = linkTextStream.str()
					});
				} else { // Relative URL
					std::string subUrl = this->relative_url;

					std::regex subUrlRegex("([^\\/]\\w+.gmi)");
					subUrl = std::regex_replace(subUrl, subUrlRegex, "");

					urlStream << this->host << subUrl << linkTextSplit[0];
					std::string url  = urlStream.str();
					std::string host = this->host;

					links.push_back({
						.protocol = "gemini://",
						.url = url,
						.host = host,
						.relative_url = linkTextSplit[0],
						.port = 1965,
						.text = linkTextStream.str()
					});
				}
			}
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
				formattedText += token.data;
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

	std::string sc = source;

	auto lines = Utils::StringUtils::Split(sc, "\n");

	for(auto& line : lines) {
		if(line.empty())
			continue;

		auto words = Utils::StringUtils::SplitWhitespace(line);

		while(!words.empty()) {
			if(words.front() == "=>") {
				// Link
				Utils::StringUtils::Shift(words); // Get rid of =>

				tokens.push_back({
					.type = TokenType::Link,
					.data = Utils::StringUtils::GetLine(words)
				});
			} else if (words.front() == "#" || words.front() == "##" || words.front() == "###") {
				// Heading
				Utils::StringUtils::Shift(words); // Get rid of #

				tokens.push_back({
					.type = TokenType::Heading,
					.data = Utils::StringUtils::GetLine(words)
				});
			} else if (words.front() == "*") {
				// List
				Utils::StringUtils::Shift(words); // Get rid of *

				tokens.push_back({
					.type = TokenType::List,
					.data = Utils::StringUtils::GetLine(words)
				});
			} else if (words.front() == ">") {
				// Quote
				Utils::StringUtils::Shift(words); // Get rid of #

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
	}

	return tokens;
}

} //namespace gg::Net
