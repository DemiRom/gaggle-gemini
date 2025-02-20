#ifndef __GEMINI_REQUEST_H__
#define __GEMINI_REQUEST_H__

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include <string>

namespace gg::Net {
    class GeminiRequest {
        public:
            GeminiRequest(const std::string& url, size_t port);
            ~GeminiRequest();

            std::string& DoRequest(const std::string& request);

        private:
            struct addrinfo hints = {};
            struct addrinfo *ptr_addrs = nullptr;

            SSL_CTX *ptr_ssl_ctx = nullptr;
            SSL *ptr_ssl = nullptr;

            int socket_descriptor = 0;

            std::string response_string;
    };
};

#endif
