cmake_minimum_required(VERSION 3.4)
project(client_server)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -g -O0 -ggdb")
set(CMAKE_CXX_STANDARD 11)
include_directories(${PROJECT_SOURCE_DIR}/include)

ADD_SUBDIRECTORY(${PROJECT_SOURCE_DIR}/client)
ADD_SUBDIRECTORY(${PROJECT_SOURCE_DIR}/server)