###########################
# environment configuration

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/tests/bin)

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

# test_mathexpression2

include_directories(
  ${OPENCL_INCLUDE_DIRS}
  )

add_executable(
  test_mathexpression2
  test_mathexpression2.cpp
  real.cpp
  ${PROJECT_SOURCE_DIR}/src/context.cpp
  ${PROJECT_SOURCE_DIR}/src/mathexpression.cpp
  ${PROJECT_SOURCE_DIR}/src/exception.cpp
  )

target_link_libraries(test_mathexpression2 ${OPENCL_LIBRARIES})

# test_matrix

include_directories(
  ${OPENCL_INCLUDE_DIRS}
  )

add_executable(
  test_matrix
  test_matrix.cpp
  ${PROJECT_SOURCE_DIR}/src/matrix.cpp
  ${PROJECT_SOURCE_DIR}/src/context.cpp
  ${PROJECT_SOURCE_DIR}/src/mathexpression.cpp
  ${PROJECT_SOURCE_DIR}/src/exception.cpp
  )

target_link_libraries(test_matrix ${OPENCL_LIBRARIES})

# test_file

add_executable(
  test_file
  test_file.cpp
  ${PROJECT_SOURCE_DIR}/src/matrix.cpp
  ${PROJECT_SOURCE_DIR}/include/matrix.h
  ${PROJECT_SOURCE_DIR}/src/file.cpp
  ${PROJECT_SOURCE_DIR}/include/file.h
  ${PROJECT_SOURCE_DIR}/src/jpegfile.cpp
  ${PROJECT_SOURCE_DIR}/include/jpegfile.h
  ${PROJECT_SOURCE_DIR}/src/image.cpp
  ${PROJECT_SOURCE_DIR}/include/image.h
  ${PROJECT_SOURCE_DIR}/src/mathexpression.cpp
  ${PROJECT_SOURCE_DIR}/src/context.cpp
  ${PROJECT_SOURCE_DIR}/src/exception.cpp
  ${PROJECT_SOURCE_DIR}/tests/sample_file.txt
  )
  
target_link_libraries(test_file "jpeg")
target_link_libraries(test_file ${OPENCL_LIBRARIES})

#######
# tests

enable_testing()

# test_context
add_test( test_context ${EXECUTABLE_OUTPUT_PATH}/test_context "${CMAKE_CURRENT_SOURCE_DIR}" )

# test_mathexpression
add_test( test_mathexpression ${EXECUTABLE_OUTPUT_PATH}/test_mathexpression "${CMAKE_CURRENT_SOURCE_DIR}" )

# test_mathexpression2
add_test( test_mathexpression2 ${EXECUTABLE_OUTPUT_PATH}/test_mathexpression2 "${CMAKE_CURRENT_SOURCE_DIR}" )

# test_matrix
add_test( test_matrix ${EXECUTABLE_OUTPUT_PATH}/test_matrix "${PROJECT_SOURCE_DIR}/kernels" )

# test_file
add_test( test_file ${EXECUTABLE_OUTPUT_PATH}/test_file "${PROJECT_SOURCE_DIR}/data/photos" "${CMAKE_CURRENT_SOURCE_DIR}/sample_file.txt" )
