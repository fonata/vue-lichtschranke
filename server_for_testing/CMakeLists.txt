cmake_minimum_required(VERSION 3.0)

project(server_for_testing)

add_subdirectory(Simple-WebSocket-Server)

add_executable(web-socket-server web-socket-server.cpp)
target_link_libraries(web-socket-server simple-websocket-server)
