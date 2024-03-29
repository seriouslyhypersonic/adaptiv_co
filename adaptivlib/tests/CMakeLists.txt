cmake_minimum_required(VERSION 3.12)
project(adaptivlib_test)

set(CMAKE_CXX_STANDARD  17)

# ---------------------------------------------------------------------------- #
# Google Test with CMake                                                       #
# https://github.com/google/googletest/blob/master/googletest/README.md        #
# ---------------------------------------------------------------------------- #

# Download and unpack googletest at configure time
configure_file(cmake/CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main messages.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
        ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
        EXCLUDE_FROM_ALL)

# The gtest/gtest_main messages carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include")
endif()

# ---------------------------------------------------------------------------- #

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB SRC_FILES ${SOURCE_DIR}/*.cpp)
file(GLOB HEADER_FILES ${INCLUDE_DIR}/*.hpp)

add_executable(adaptivlib_tests ${SRC_FILES} ${HEADER_FILES})

target_include_directories(adaptivlib_tests PUBLIC ${INCLUDE_DIR})

target_link_libraries(adaptivlib_tests
        adaptiv
        gtest
        gtest_main)