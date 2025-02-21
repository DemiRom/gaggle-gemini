#include "ResponseParser.h"
#include "ResponseObject.h"
#include "StringUtils.h"
#include <exception>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>

gg::Net::ResponseParser::ResponseParser(const std::string& base_url) : base_url(base_url) {

}

gg::Net::ResponseObject_t gg::Net::ResponseParser::ParseResponse(std::string &resp) {
    std::stringstream ss;

    auto lines = gg::Utils::StringUtils::Split(resp, "\r\n");

    auto response_header_words = gg::Utils::StringUtils::Split(lines[0], " ");

    //Status code should be the very first word in the response
    try {
        StatusCode sc = static_cast<StatusCode>(std::stoi(response_header_words[0]));
        std::string mime_type = lines[0];
        std::string content = lines[1];

        switch(sc) {
            case INPUT_EXPECTED_BASIC:
                break;
            case INPUT_SENSITIVE:
                break;
            case SUCCESS:
                return ParseSuccessResponse(sc, mime_type, content);
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
    } catch (std::exception &e) {
        std::cerr << "Could not parse status code: " << e.what() << std::endl;
    }

    return {};
}

gg::Net::ResponseObject_t gg::Net::ResponseParser::ParseSuccessResponse(StatusCode sc, std::string& mime_type, std::string& content) {
    std::string cp = content; //Copy the string because split modify it.
    auto content_lines = gg::Utils::StringUtils::Split(cp, "\n");

    std::vector<gg::Net::Link_t> links;

    for(auto line : content_lines) {
        if(line.find("=>") != std::string::npos) {
            if(line.find("http://") != std::string::npos || line.find("https://") != std::string::npos) { //TODO This could be much better
                continue;
            }

            line.erase(0, 3); //Remove the => tag

            std::string cp = line;
            std::vector<std::string> split;
            if(cp.find("\t") != std::string::npos)
                split = gg::Utils::StringUtils::Split(cp, "\t"); //TODO This will break on other pages because it may not necessarily be a tab
            else
                split = gg::Utils::StringUtils::Split(cp, " ");

            std::string host = this->base_url;

            host.erase(0, 9); // Remove the protocol

            if(line.find("gemini://") != std::string::npos) {
                links.push_back({
                    .host = host,
                    .link_url = split[0],
                    .relative_url = "", //TODO Split of the relative url
                    .pretty_name = split[1],
                    .port = 1965 //TODO Split off the port
                });
            } else {
                links.push_back({
                    .host = host,
                    .link_url = this->base_url + split[0],
                    .relative_url = split[0],
                    .pretty_name = split[1],
                    .port = 1965 //TODO Split off the port
                });
            }
        }
    }

    return {
        .status_code = sc,
        .mime_type = mime_type,
        .content = content,
        .content_length = content.length(),
        .links = std::move(links)
    };
}
