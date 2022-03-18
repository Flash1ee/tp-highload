cmake_minimum_required(VERSION 3.16)
project(static_web_server)

set(CMAKE_CXX_STANDARD 20)
include_directories(include)
add_executable(static_web_server main.cpp include/server.h++ include/constants.h++ src/server.cpp src/http_handler.cpp include/http_handler.h++ src/utils.cpp include/utils.h++ include/responses.h++)

target_link_libraries(static_web_server -lpthread)
