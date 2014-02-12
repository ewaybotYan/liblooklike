###########################
# environment configuration

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/tests/bin)

file(
  GLOB
  SOURCES
  ${PROJECT_SOURCE_DIR}/src/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
  )

set(LIBS ${OPENCL_LIBRARIES} ${Armadillo_LIBRARIES})

#############
# executables

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

target_link_libraries(test_context ${LIBS})

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

target_link_libraries(test_mathexpression ${LIBS})

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

target_link_libraries(test_mathexpression2 ${LIBS})

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

target_link_libraries(test_matrix ${LIBS})

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
target_link_libraries(test_file ${LIBS})

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
