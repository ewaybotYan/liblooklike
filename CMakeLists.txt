cmake_minimum_required(VERSION 2.8.8)

project(liblooklike)

enable_testing()

# recommended option for cmake: -DCMAKE_BUILD_TYPE=DEBUG
# -DCMAKE_CXX_FLAGS="-Werror -Wall -Wextra -Wunreachable-code"

#######
# PATHS

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/output/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/output/lib)
set(DOC_OUTPUT_PATH ${PROJECT_BINARY_DIR}/output/doc)

set(PROJECT_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)


###############
# BUILD OPTIONS

include(CMakeDependentOption)

option(USE_POCL "Use POCL instead of a standard OpenCL library" FALSE)
option(BUILD_DOCUMENTATION "Build project code documentation" TRUE)
CMAKE_DEPENDENT_OPTION(USE_PLANTUML
                       "Generate UML Diagrams in  focumentation"
                       TRUE
                       "BUILD_DOCUMENTATION" FALSE)

#############
# BUILD FLAGS

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")
set(warnings "-Wall -Wextra -Werror -pedantic")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${warnings}")

#######################################
# EXTERNAL DEPENDENCIES PATHS AND FLAGS

if(USE_POCL)
  message(STATUS "using POCL")
  find_package(PkgConfig)
  pkg_check_modules(OPENCL REQUIRED pocl)
  pkg_search_module(OPENCL REQUIRED pocl)
else(USE_POCL)
  find_package( OpenCL REQUIRED )
  if( !OPENCL_HAS_CPP_BINDINGS )
    message(ERROR "OpenCL c++ bindings not found")
  endif( !OPENCL_HAS_CPP_BINDINGS )
endif(USE_POCL)

include_directories(${OPENCL_INCLUDE_DIRS})
link_directories(${OPENCL_LIBRARY_DIRS})

find_package(Armadillo REQUIRED)
find_package(LAPACK REQUIRED)

#############
# SUBPROJECTS

add_subdirectory(tests)
add_subdirectory(doc)
add_subdirectory(src)
