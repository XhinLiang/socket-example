cmake_minimum_required(VERSION 3.2)
project(Socket-Example)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c99")

add_executable(server server.c)
add_executable(client client.c)