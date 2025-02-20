#include <iostream>

#include "GeminiRequest.h"
#include "ResponseParser.h"
#include "ResponseObject.h"


#define HOST "geminiprotocol.net"
#define PORT 1965

using namespace gg::Net;

int main(int argc, char** argv) {
    GeminiRequest request(HOST, PORT);

    std::string response = request.DoRequest("gemini://geminiprotocol.net/\r\n");

    ResponseParser parser("gemini://geminiprotocol.net/");
    ResponseObject_t ro = parser.ParseResponse(response);

    std::cout << "Status: " << ro.status_code << std::endl;
    std::cout << "MimeType: " << ro.mime_type << std::endl;
    std::cout << "Content: " << ro.content << std::endl;

    for(Link_t link : ro.links) {
        std::cout << "LINK: " << link.base_url << "" << link.link_url << " PRETTY NAME: " << link.pretty_name << std::endl;
    }

    return 0;
}
