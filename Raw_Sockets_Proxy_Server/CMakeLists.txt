cmake_minimum_required(VERSION 3.10)

project(RawSocketsProxyServer VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra)
endif()

link_libraries(pthread)

include_directories(${PROJECT_SOURCE_DIR}/Client)
include_directories(${PROJECT_SOURCE_DIR}/Proxy)
include_directories(${PROJECT_SOURCE_DIR}/Server)

add_executable(${PROJECT_NAME}
    main.cpp
    Client/Client.cpp
    Proxy/Proxy.cpp
    Server/Server.cpp
    RawSockets/RawSocket.cpp
)
