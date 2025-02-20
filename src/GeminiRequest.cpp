#include "GeminiRequest.h"
#include "GeminiException.h"

#include <iostream>
#include <sstream>

gg::Net::GeminiRequest::GeminiRequest(const std::string& host, size_t port) {
    this->hints.ai_family = AF_INET;
    this->hints.ai_socktype = SOCK_STREAM;
    this->hints.ai_protocol = IPPROTO_TCP;

    if (getaddrinfo(host.c_str(), std::to_string(port).c_str(), &hints, &this->ptr_addrs) < 0) {
        std::cerr << "Could not get address of " << host << std::endl << std::flush;
        exit(1);
    }

    for (struct addrinfo *addr = this->ptr_addrs; addr != NULL; addr = addr->ai_next) {
        this->socket_descriptor = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);

        if (this->socket_descriptor == -1) break;
        if (connect(this->socket_descriptor, addr->ai_addr, addr->ai_addrlen) == 0) break;

        close(this->socket_descriptor);
        this->socket_descriptor = -1;
    }

    freeaddrinfo(this->ptr_addrs);

    if (this->socket_descriptor == -1) {
        throw gg::Net::Exceptions::ConnectionException();
    }
}

gg::Net::GeminiRequest::~GeminiRequest() {
    close(this->socket_descriptor);
}

std::string& gg::Net::GeminiRequest::DoRequest(const std::string& request) {
    std::string full_request = request + "\r\n"; //Add CRLF to the request url

    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    ptr_ssl_ctx = SSL_CTX_new(TLS_client_method());

    if (ptr_ssl_ctx == nullptr) {
        throw gg::Net::Exceptions::SSLException();
    }

    ptr_ssl = SSL_new(ptr_ssl_ctx);
    SSL_set_fd(ptr_ssl, this->socket_descriptor);

    if (SSL_connect(ptr_ssl) < 0) {
        throw gg::Net::Exceptions::SSLException();
    }

    SSL_write(ptr_ssl, full_request.c_str(), full_request.length());

    char buffer[1024];

    std::stringstream ss;

    ssize_t n = SSL_read(ptr_ssl, buffer, sizeof(buffer));
    while (n > 0) {
        // fwrite(buffer, 1, n, stdout);

        for(size_t i = 0; i < n; i++) {
            ss << buffer[i];
        }

        n = SSL_read(ptr_ssl, buffer, sizeof(buffer));
    }

    this->response_string = ss.str();

    SSL_set_shutdown(ptr_ssl, SSL_RECEIVED_SHUTDOWN | SSL_SENT_SHUTDOWN);
    SSL_shutdown(ptr_ssl);
    SSL_free(ptr_ssl);
    SSL_CTX_free(ptr_ssl_ctx);

    return this->response_string;
}

const std::string& gg::Net::GeminiRequest::GetRequestString() const {
    return this->request_string;
}
