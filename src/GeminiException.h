#ifndef __GEMINI_EXCEPTION_H__
#define __GEMINI_EXCEPTION_H__

#include <exception>
#include <string>

namespace gg::Net::Exceptions {
    class GenericException : std::exception {
        public:
            ~GenericException() _NOEXCEPT;

            const char* what() const _NOEXCEPT;
    };

    class ConnectionException : std::exception {
        public:
            ~ConnectionException() _NOEXCEPT;

            const char* what() const _NOEXCEPT;
    };

    class SSLException : std::exception {
    	public:
     		SSLException(const std::string& message);
     		~SSLException() _NOEXCEPT;

       		const char* what() const _NOEXCEPT;

        private:
        	std::string message;
    };

    class SocketException : std::exception {
    	public:
     		SocketException(const std::string& message);
       		SocketException();

         	~SocketException() _NOEXCEPT;

          	const char* what() const _NOEXCEPT;

        private:
        	std::string message;
    };
};

#endif
