cmake_minimum_required(VERSION 3.16)
project(18)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_REENTRANT")


add_executable(webserver webserver.c)
target_link_libraries(
    webserver
    pthread
)