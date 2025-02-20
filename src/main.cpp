#include <iostream>

#include "GeminiRequest.h"
#include "ResponseParser.h"
#include "ResponseObject.h"


#define HOST "geminiprotocol.net"
#define PORT 1965
#define MAX_CRAWL_DEPTH 10
using namespace gg::Net;

int main(int argc, char** argv) {
    GeminiRequest request(HOST, PORT);

    std::string response = request.DoRequest("gemini://geminiprotocol.net/\r\n");

    ResponseParser parser("gemini://geminiprotocol.net/");
    ResponseObject_t ro = parser.ParseResponse(response);

    // std::cout << "Status: " << ro.status_code << std::endl;
    // std::cout << "MimeType: " << ro.mime_type << std::endl;
    // std::cout << "Content: " << ro.content << std::endl;

    if(ro.status_code == StatusCode::SUCCESS) {
        for(Link_t link : ro.links) {
            GeminiRequest request(HOST, PORT);

            std::cout << "Link: " << link.link_url << std::endl;
            ResponseObject_t re = parser.ParseResponse(request.DoRequest(link.link_url + "\r\n"));
            std::cout << re.status_code << std::endl;

            if(re.status_code == StatusCode::SUCCESS){
                for(Link_t lin : re.links) {
                    std::cout << "Inner Links: " << lin.link_url << std::endl;
                }
            }
        }

    } else {
        std::cout << "Failed to request " << request.GetRequestString() << std::endl;
    }

    return 0;
}
