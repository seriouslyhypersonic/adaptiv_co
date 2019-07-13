cmake_minimum_required(VERSION 3.13)
project(server_coro)

set(CMAKE_CXX_STANDARD  17)

# Boost
find_package(Boost REQUIRED COMPONENTS) #system coroutine REQUIRED)
if(MSVC)
    message("[server]---------------------------------------------------------")
    message("  Boost version: " ${Boost_LIB_VERSION})
    message("  Boost include: " ${Boost_INCLUDE_DIRS})
    message("Boost libraries: " ${Boost_LIBRARY_DIRS})
    message("-----------------------------------------------------------------")
endif()
include_directories(${Boost_INCLUDE_DIRS})
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

# Link libraries
target_link_directories(server PUBLIC ${Boost_LIBRARY_DIRS})
set(LINK_LIBS adaptiv Threads::Threads ${Boost_LIBRARIES})
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    list(APPEND LINK_LIBS boost_coroutine)  #Seems to be required (gcc 7.4.0)
endif ()
target_link_libraries(server ${LINK_LIBS})