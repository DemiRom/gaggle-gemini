#include "GeminiException.h"

gg::Net::Exceptions::GenericException::~GenericException() throw() {}

const char* gg::Net::Exceptions::GenericException::what() const throw() {
    return "Generic Exception";
}

gg::Net::Exceptions::ConnectionException::~ConnectionException() throw() {}

const char* gg::Net::Exceptions::ConnectionException::what() const throw() {
    return "Connection Exception";
}

gg::Net::Exceptions::SSLException::SSLException(const std::string& message) : message(message) {}

gg::Net::Exceptions::SSLException::~SSLException() throw() {}

const char* gg::Net::Exceptions::SSLException::what() const throw() {
	return this->message.c_str();
}

gg::Net::Exceptions::SocketException::SocketException(const std::string& message) : message(message) { }

gg::Net::Exceptions::SocketException::SocketException() { this->message = "Socket exception"; }

gg::Net::Exceptions::SocketException::~SocketException() throw() {}

const char* gg::Net::Exceptions::SocketException::what() const throw() {
	return this->message.c_str();
}
