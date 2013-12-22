cmake_minimum_required(VERSION 2.8.8)

project(liblooklike)

enable_testing()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11")
set(warnings "-Wall -Wextra -Werror")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${warnings}")

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

set(PROJECT_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

add_subdirectory(tests)
