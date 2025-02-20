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
    ResponseObject_t rp =
        parser->ParseResponse(request->DoRequest(link.link_url));


    memcpy(ro, &rp, sizeof(ResponseObject_t));

    std::cout << "HERE? " << std::endl;

    if (ro->status_code == StatusCode::SUCCESS && crawl_count < MAX_CRAWL_COUNT) {
        for (Link_t link : ro->links) {
        // std::cout << "Link: " << link.link_url << std::endl;
        // crawl(link);
        }
    }

    delete request;
    delete parser;
    free(ro);
}

int main(int argc, char **argv) {
    GeminiRequest request(HOST, PORT);

    std::string response = request.DoRequest("gemini://geminiprotocol.net/");

    ResponseParser parser("gemini://geminiprotocol.net/");
    ResponseObject_t ro = parser.ParseResponse(response);

    // std::cout << "Status: " << ro.status_code << std::endl;
    // std::cout << "MimeType: " << ro.mime_type << std::endl;
    // std::cout << "Content: " << ro.content << std::endl;

    if (ro.status_code == StatusCode::SUCCESS) {
        for (Link_t link : ro.links) {
        std::cout << "Link: " << link.link_url << " Host: " << link.host
                    << std::endl;
        crawl(link);
        }
    } else {
        std::cout << "Failed to request " << request.GetRequestString()
                << std::endl;
    }

    return 0;
}
