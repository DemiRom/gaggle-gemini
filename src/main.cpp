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
    ResponseParser *parser = new ResponseParser("gemini://" + link.host);
    ResponseObject_t *ro = (ResponseObject_t *)(malloc(sizeof(ResponseObject_t)));

    // parser->ParseResponse(request->DoRequest(link.link_url));
    auto response = request->DoRequest(link.link_url);

    std::cout << "Response: " << response << std::endl;

    ResponseObject_t rp =
        parser->ParseResponse(response);

    memcpy(ro, &rp, sizeof(ResponseObject_t));

    if (ro->header.status_code == StatusCode::SUCCESS && crawl_count < MAX_CRAWL_COUNT) {
        for (Link_t link : ro->content.links) {
        // std::cout << "Link: " << link.link_url << std::endl;
        	crawl(link);
        }
    }

    delete request;
    delete parser;
    free(ro);
}

int main() {
    GeminiRequest *request = new GeminiRequest(HOST, PORT);

    std::string response = request->DoRequest("gemini://geminiprotocol.net/");

    ResponseParser *parser = new ResponseParser("gemini://geminiprotocol.net/");
    ResponseObject_t ro = parser->ParseResponse(response);

    std::cout << "Status: " << ro.header.status_code << std::endl;
    std::cout << "MimeType: " << ro.header.mime_type << std::endl;
    std::cout << "Content: " << ro.content.text << std::endl;
    std::cout << "Links Size: " << ro.content.links.size() << std::endl;

    if (ro.header.status_code == StatusCode::SUCCESS) {
        for (Link_t link : ro.content.links) {
	        std::cout << "Link: " << link.link_url << " Host: " << link.host
	                    << std::endl;
	        // crawl(link);
        }
    } else {
        std::cout << "Failed to request " << request->GetRequestString()
                << std::endl;
    }

    delete request;
    delete parser;

    return 0;
}
