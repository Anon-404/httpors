cmake_minimum_required(VERSION 3.10)
project(httpors C)

set(CMAKE_C_STANDARD 11)

set(SOURCES httpors.c)

find_package(CURL REQUIRED)
find_package(Threads REQUIRED)

add_executable(httpors ${SOURCES})

target_link_libraries(httpors CURL::libcurl Threads::Threads)
