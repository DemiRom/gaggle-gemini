#include <iostream>

#include "GeminiRequest.h"
#include "ResponseParser.h"
#include "ResponseObject.h"


#define HOST "geminiprotocol.net"
#define PORT 1965
#define MAX_CRAWL_COUNT 100
using namespace gg::Net;

size_t crawl_count = 0;

void crawl(Link_t& link) {
    crawl_count++;

    GeminiRequest request(link.host, link.port);
    ResponseParser parser("gemini://" + link.host);
    ResponseObject_t ro = parser.ParseResponse(request.DoRequest(link.link_url));

    if(ro.status_code == StatusCode::SUCCESS && crawl_count < MAX_CRAWL_COUNT) {
        for(Link_t link : ro.links) {
            std::cout << "Link: " << link.link_url << std::endl;
            crawl(link);
        }
    }
}

int main(int argc, char** argv) {
    GeminiRequest request(HOST, PORT);

    std::string response = request.DoRequest("gemini://geminiprotocol.net/");

    ResponseParser parser("gemini://geminiprotocol.net/");
    ResponseObject_t ro = parser.ParseResponse(response);

    // std::cout << "Status: " << ro.status_code << std::endl;
    // std::cout << "MimeType: " << ro.mime_type << std::endl;
    // std::cout << "Content: " << ro.content << std::endl;

    if(ro.status_code == StatusCode::SUCCESS) {
        for(Link_t link : ro.links) {
            std::cout << "Link: " << link.link_url << std::endl;
            crawl(link);
        }
    } else {
        std::cout << "Failed to request " << request.GetRequestString() << std::endl;
    }

    return 0;
}
