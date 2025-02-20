#include "GeminiException.h"

gg::Net::Exceptions::GenericException::~GenericException() throw() {

}

const char* gg::Net::Exceptions::GenericException::what() const throw() {
    return "Generic Exception";
}

gg::Net::Exceptions::ConnectionException::~ConnectionException() throw() {

}

const char* gg::Net::Exceptions::ConnectionException::what() const throw() {
    return "Connection Exception";
}
