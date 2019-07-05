cmake_minimum_required(VERSION 3.10)
project(server_coro)

set(CMAKE_CXX_STANDARD  17)

# Boost
find_package(Boost REQUIRED COMPONENTS system coroutine REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
#add_definitions(-DBOOST_ASIO_ENABLE_HANDLER_TRACKING)

# Threads
find_package(Threads REQUIRED)

# Source and include dirs
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Source and header files
file(GLOB SRC_FILES ${SOURCE_DIR}/*.cpp)
file(GLOB HEADER_FILES ${INCLUDE_DIR}/*.hpp)

add_executable(server ${SRC_FILES} ${HEADER_FILES})

target_include_directories(server PUBLIC ${INCLUDE_DIR})

target_link_libraries(server
        adaptiv
        Threads::Threads
        boost_coroutine         # Seems to be required (gcc 7.4.0)
        ${Boost_LIBRARIES})