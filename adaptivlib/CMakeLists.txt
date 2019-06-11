cmake_minimum_required(VERSION 3.10)
project(adaptiv_lib)

set(CMAKE_CXX_STANDARD 17)

# Boost
find_package(Boost REQUIRED COMPONENTS)
include_directories(${Boost_INCLUDE_DIR})

# Threads
find_package(Threads REQUIRED)

# Source and include dirs
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(ADAPTIV_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include/)

# Source and header files
file(GLOB SRC_FILES ${SOURCE_DIR}/*.cpp)
file(GLOB HEADER_FILES ${ADAPTIV_INCLUDE_DIR}/adaptiv/*.hpp)

add_library(adaptivlib STATIC ${SRC_FILES} ${ADAPTIV_INCLUDE_DIRS})

target_include_directories(adaptivlib PUBLIC ${ADAPTIV_INCLUDE_DIRS})

target_link_libraries(adaptivlib
        Threads::Threads
        ${Boost_LIBRARIES})

add_subdirectory(tests)
