cmake_minimum_required(VERSION 2.8.8)

project(liblooklike)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11")
set(warnings "-Wall -Wextra -Werror")
if (NOT CONFIGURED_ONCE)
  set(CMAKE_CXX_FLAGS "${warnings}"
    CACHE STRING "Flags used by the compiler during all build types." FORCE)
endif()

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

set(PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

add_subdirectory(tests)
