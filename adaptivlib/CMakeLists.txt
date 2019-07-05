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

# Heade files
file(GLOB HEADER_FILES ${ADAPTIV_INCLUDE_DIR}/adaptiv/*.hpp)

# Source files
file(GLOB SRC_FILES ${SOURCE_DIR}/*.cpp)
file(GLOB SRC_FILES ${SOURCE_DIR}/input/*.cpp)

# External libraries
set(EXTERNAL_CEREAL_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}/include/adaptiv/serialization/external)

add_library(adaptiv STATIC ${SRC_FILES} ${ADAPTIV_INCLUDE_DIRS})

target_include_directories(adaptiv PUBLIC ${ADAPTIV_INCLUDE_DIRS}
            ${EXTERNAL_CEREAL_DIRS})

target_link_libraries(adaptiv
        Threads::Threads
        ${Boost_LIBRARIES})

add_subdirectory(tests)
