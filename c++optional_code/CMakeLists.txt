cmake_minimum_required(VERSION 3.15)
project(Prime)

set(CMAKE_CXX_STANDARD 14)


add_executable(Client
        Client.h
        Client.cpp
        clientMain.cpp
        )

add_executable(Server
        Server.cpp
        Server.h
        svrMain.cpp
        )

add_executable(Server2
        Server.cpp
        Server.h
        svrMain.cpp
        )

target_include_directories(Client PRIVATE)
target_include_directories(Server PRIVATE)
target_include_directories(Server2 PRIVATE)

TARGET_LINK_LIBRARIES( Client pthread)
TARGET_LINK_LIBRARIES( Server pthread)
TARGET_LINK_LIBRARIES( Server2 pthread)