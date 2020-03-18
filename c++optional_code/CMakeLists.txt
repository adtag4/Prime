cmake_minimum_required(VERSION 3.15)
project(Prime)

set(CMAKE_CXX_STANDARD 14)


add_executable(Client
        pfp/Client.h
        pfp/Client.cpp
        clientMain.cpp
        algorithm/pollard.cpp
        algorithm/pollard.h
        algorithm/algorithm.h
        )

add_executable(Server
        pfp/Server.cpp
        pfp/Server.h
        svrMain.cpp
        algorithm/pollard.cpp
        algorithm/pollard.h
        algorithm/algorithm.h
        )

add_executable(Server2
        pfp/Server.cpp
        pfp/Server.h
        svrMain.cpp
        algorithm/pollard.cpp
        algorithm/pollard.h
        algorithm/algorithm.h
        )

add_executable(Server3
        pfp/Server.cpp
        Server.h
        pfp/svrMain.cpp
        algorithm/pollard.cpp
        algorithm/pollard.h
        algorithm/algorithm.h
        )

target_include_directories(Client PRIVATE)
target_include_directories(Server PRIVATE)
target_include_directories(Server2 PRIVATE)
target_include_directories(Server3 PRIVATE)

TARGET_LINK_LIBRARIES( Client pthread)
TARGET_LINK_LIBRARIES( Server pthread)
TARGET_LINK_LIBRARIES( Server2 pthread)
TARGET_LINK_LIBRARIES( Server3 pthread)
TARGET_LINK_LIBRARIES( Client gmp)
TARGET_LINK_LIBRARIES( Server gmp)
TARGET_LINK_LIBRARIES( Server2 gmp)
TARGET_LINK_LIBRARIES( Server3 gmp)