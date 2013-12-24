###########################
# environment configuration

# for OpenCL
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/findopencl/")

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/lib)

SET(USE_POCL FALSE CACHE STRING "Wether pocl or a normal OpenCL library is used")

if(USE_POCL)
  find_package(PkgConfig)
  pkg_check_modules(OPENCL pocl)
  pkg_search_module(OPENCL pocl)
else(USE_POCL)
  find_package( OpenCL REQUIRED )
  if( !OPENCL_HAS_CPP_BINDINGS )
    message(ERROR "OpenCL c++ bindings not found")
  endif( !OPENCL_HAS_CPP_BINDINGS )
endif(USE_POCL)

if(USE_GUI)
  # for qt precompliation operations
  set(CMAKE_INCLUDE_CURRENT_DIR ON)
  set(CMAKE_AUTOMOC ON)

  # find include dir
  find_package(Qt5Gui REQUIRED)
  find_package(OpenGL REQUIRED)

  if (Qt5Gui_FOUND)
    message(STATUS "qt5 found")
  else (Qt5Gui_FOUND)
    message(ERROR "qt5 not found") 
  endif (Qt5Gui_FOUND)

  add_definitions(${Qt5Gui_DEFINITIONS})
endif(USE_GUI)

file(
  GLOB
  SOURCES
  ${PROJECT_SOURCE_DIR}/src/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
  )


#############
# executables

# test_preview

if(USE_GUI)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Gui_EXECUTABLE_COMPILE_FLAGS}")

  include_directories(
    ${Qt5Gui_INCLUDES}
    ${OPENGL_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/include 
    "/usr/include/qt"
    )

  add_executable(
    test_preview 
    test_preview.cpp 
    ../src/openglwindow.cpp
    ../src/previewwindow.cpp
    )

  target_link_libraries(test_preview ${OPENGL_LIBRARIES})

  qt5_use_modules(test_preview Gui)
endif(USE_GUI)

# test_context

include_directories(
  ${OPENCL_INCLUDE_DIRS}
  )

add_executable(
  test_context
  test_context.cpp
  ${PROJECT_SOURCE_DIR}/src/context.cpp
  ${PROJECT_SOURCE_DIR}/src/exception.cpp
  )

target_link_libraries(test_context ${OPENCL_LIBRARIES})

# test_mathexpression

include_directories(
  ${OPENCL_INCLUDE_DIRS}
  )

add_executable(
  test_mathexpression
  test_mathexpression.cpp
  real.cpp
  ${PROJECT_SOURCE_DIR}/src/context.cpp
  ${PROJECT_SOURCE_DIR}/src/mathexpression.cpp
  ${PROJECT_SOURCE_DIR}/src/exception.cpp
  )

target_link_libraries(test_mathexpression ${OPENCL_LIBRARIES})

#######
# tests

enable_testing()

# test_context
add_test( test_context ${EXECUTABLE_OUTPUT_PATH}/test_context "${CMAKE_CURRENT_SOURCE_DIR}" )

# test_mathexpression
add_test( test_mathexpression ${EXECUTABLE_OUTPUT_PATH}/test_mathexpression "${CMAKE_CURRENT_SOURCE_DIR}" )
