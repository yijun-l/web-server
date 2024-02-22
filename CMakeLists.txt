cmake_minimum_required(VERSION 3.15)
project(web-server)

set(CMAKE_CXX_STANDARD 17)
set(SRC src)

set(SERVER_CODE
        ${SRC}/server.cpp
        ${SRC}/Epoll.cpp
        ${SRC}/NetAddr.cpp
        ${SRC}/Socket.cpp
        ${SRC}/util.cpp
)

set(CLIENT_CODE
        ${SRC}/client.cpp
        ${SRC}/util.cpp
)

add_executable(tcp_server ${SERVER_CODE})

add_executable(tcp_client ${CLIENT_CODE})

