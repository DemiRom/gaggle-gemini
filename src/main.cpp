#include <cstring>
#include <iostream>

#include "GeminiRequest.h"
#include "ResponseObject.h"
#include "ResponseParser.h"

#define HOST "geminiprotocol.net"
#define PORT 1965
#define MAX_CRAWL_COUNT 100

using namespace gg::Net;

size_t crawl_count = 0;

void crawl(Link_t &link) {
    crawl_count++;

    GeminiRequest *request = new GeminiRequest(link.host, link.port);
    std::string response = request->DoRequest(link.url);

    ResponseParser *parser = new ResponseParser(link.host, link.relative_url);
    ResponseObject_t ro = parser->ParseResponse(response);

    std::cout << "BEGIN [" << crawl_count << "] -------------------------- [" << crawl_count << "] BEGIN" << std::endl
    		  << "\tRequest: " << link.url << std::endl
    		  << "\tResponse: " << response.substr(0, 20) << "..." << std::endl
    		  << "\tStatus: " << ro.header.status_code << std::endl
    		  << "\tMimeType: " << ro.header.mime_type << std::endl
    		  << "\tLang: " << ro.header.lang << std::endl
              << "\tLinks Size: " << ro.content.links.size() << std::endl;

    if (ro.header.status_code == StatusCode::SUCCESS && crawl_count < MAX_CRAWL_COUNT) {
        for (Link_t link : ro.content.links) {
        	if(link.url.find("http://") != std::string::npos || link.url.find("https://") != std::string::npos)
         		continue;

        	std::cout << "\tLink: " << link.url << std::endl
					  << "\tHost: " << link.host << std::endl
					  << "\tText: " << link.text << std::endl
		              << std::endl;
        	crawl(link);
        }
    }

    std::cout << "END [" << crawl_count << "] -------------------------- [" << crawl_count << "] END" << std::endl << std::endl;

    delete request;
    delete parser;
}

int main() {
    GeminiRequest *request = new GeminiRequest(HOST, PORT);

    std::string response = request->DoRequest("gemini://geminiprotocol.net/");

    ResponseParser *parser = new ResponseParser("gemini://geminiprotocol.net/", "");
    ResponseObject_t ro = parser->ParseResponse(response);

    if (ro.header.status_code == StatusCode::SUCCESS) {
        for (Link_t link : ro.content.links) {
	      //   std::cout << "Link: " << link.url << std::endl
					  // << "Host: " << link.host << std::endl
					  // << "Text: " << link.text << std::endl
	      //             << std::endl;
	        crawl(link);
        }
        // for (Heading_t heading : ro.content.headings) {
        // 	std::cout << "Heading: " << heading.heading_text << std::endl;
        // }

    } else {
        std::cout << "Failed to request " << request->GetRequestString()
                << std::endl;
    }

    delete request;
    delete parser;

    return 0;
}
