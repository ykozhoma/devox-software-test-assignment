cmake_minimum_required(VERSION 3.14)

project(http-server LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
set(BUILD_DIR ${CMAKE_SOURCE_DIR}/build)

file(GLOB SOURCES ${SOURCE_DIR}/*.cpp)

add_executable(http-server ${SOURCES})

install(TARGETS http-server DESTINATION bin)
