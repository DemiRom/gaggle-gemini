cmake_minimum_required(VERSION 3.3)
project(gemeni-gaggle)

set(CMAKE_CXX_STANDARD 26)

set(SRCS
    src/main.cpp
    src/ResponseObject.h
    src/ResponseParser.cpp
    src/ResponseParser.h
    src/StringUtils.cpp
    src/StringUtils.h
    src/GeminiException.cpp
    src/GeminiException.h
    src/GeminiRequest.cpp
    src/GeminiRequest.h

)

find_package(OpenSSL REQUIRED)

add_executable(gemeni-gaggle ${SRCS})
target_link_libraries(gemeni-gaggle OpenSSL::SSL)
