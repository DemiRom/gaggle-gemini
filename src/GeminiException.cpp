#include "GeminiException.h"

namespace gg::Net::Exceptions {

GenericException::~GenericException() throw() {}

const char* GenericException::what() const throw() {
    return "Generic Exception";
}

ConnectionException::~ConnectionException() throw() {}

const char* ConnectionException::what() const throw() {
    return "Connection Exception";
}

SSLException::SSLException(const std::string& message) : message(message) {}

SSLException::~SSLException() throw() {}

const char* SSLException::what() const throw() {
	return this->message.c_str();
}

SocketException::SocketException(const std::string& message) : message(message) { }

SocketException::SocketException() { this->message = "Socket exception"; }

SocketException::~SocketException() throw() {}

const char* SocketException::what() const throw() {
	return this->message.c_str();
}

} // namespace gg::Net::Exceptions
