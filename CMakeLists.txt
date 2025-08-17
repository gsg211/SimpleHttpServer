cmake_minimum_required(VERSION 4.0)
project(Http_server_2)

set(CMAKE_CXX_STANDARD 20)

add_executable(Http_server_2 main.cpp
        HttpRequest.h
        HttpRequest.cpp)
