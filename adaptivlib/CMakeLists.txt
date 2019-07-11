cmake_minimum_required(VERSION 3.13)
project(adaptiv_lib)

set(CMAKE_CXX_STANDARD 17)

# Boost
find_package(Boost REQUIRED COMPONENTS)
if(MSVC)
    message("[libadaptiv]-----------------------------------------------------")
    message("  Boost version: " ${Boost_LIB_VERSION})
    message("  Boost include: " ${Boost_INCLUDE_DIRS})
    message("Boost libraries: " ${Boost_LIBRARY_DIRS})
    message("-----------------------------------------------------------------")
endif()
include_directories(${Boost_INCLUDE_DIRS})

# Threads
find_package(Threads REQUIRED)

# Source and include dirs
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(ADAPTIV_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include/)

# Header files
file(GLOB HEADER_FILES ${ADAPTIV_INCLUDE_DIR}/adaptiv/*.hpp)

# Source files (cross-platform)
file(GLOB_RECURSE SRC_FILES ${SOURCE_DIR}/*.cpp/)
list(FILTER SRC_FILES EXCLUDE REGEX "windows*")
list(FILTER SRC_FILES EXCLUDE REGEX "linux*")
list(FILTER SRC_FILES EXCLUDE REGEX "mac*")

# Source files (platform-specific)
# todo: test this on windows
if (MSVC)
    file(GLOB_RECURSE WIN_SRC_FILES ${SOURCE_DIR}/windows*.cpp/)
    list(APPEND SRC_FILES ${WIN_SRC_FILES})
endif ()

# External libraries
set(EXTERNAL_CEREAL_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}/include/adaptiv/serialization/external)

add_library(adaptiv STATIC ${SRC_FILES} ${ADAPTIV_INCLUDE_DIRS})

target_include_directories(adaptiv PUBLIC
        ${ADAPTIV_INCLUDE_DIRS}
        ${EXTERNAL_CEREAL_DIRS})

target_link_directories(adaptiv PUBLIC ${Boost_LIBRARY_DIRS})
target_link_libraries(adaptiv
        Threads::Threads
        ${Boost_LIBRARIES})

add_subdirectory(tests)
