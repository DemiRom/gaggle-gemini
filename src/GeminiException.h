#ifndef __GEMINI_EXCEPTION_H__
#define __GEMINI_EXCEPTION_H__

#include <exception>
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
     		~SSLException() _NOEXCEPT;

       		const char* what() const _NOEXCEPT;
    };

};

#endif
