cmake_minimum_required(VERSION 3.10)
project(adaptivlib_test)

set(CMAKE_CXX_STANDARD  17)

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB SRC_FILES ${SOURCE_DIR}/*.cpp)
file(GLOB HEADER_FILES ${SOURCE_DIR}/*.hpp)

add_executable(adaptivlib_tests ${SRC_FILES} ${HEADER_FILES})

target_link_libraries(adaptivlib_tests
        adaptivlib)