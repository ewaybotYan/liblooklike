###########################
# environment configuration

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/tests/bin)

file(
  GLOB
  SOURCES
  ${PROJECT_SOURCE_DIR}/src/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
  )

set(LIBS ${OPENCL_LIBRARIES} ${Armadillo_LIBRARIES} pthread)
message(STATUS ${OPENCL_LIBRARIES})
message(STATUS ${OPENCL_INCLUDE})


#############
# executables

# test_context

include_directories(
  "${PROJECT_SOURCE_DIR}/include ${OPENCL_INCLUDE_DIRS}"
)

add_executable(
  test_context
  test_context.cpp
  ${PROJECT_SOURCE_DIR}/src/context.cpp
  ${PROJECT_SOURCE_DIR}/src/exception.cpp
  )

target_link_libraries(test_context ${LIBS})

# test_algorithm

add_executable(
  test_algorithm1
  ../examples/example1/test_algorithm.cpp
  ../examples/example1/real.cpp
  ../examples/example1/real.h
  ${PROJECT_SOURCE_DIR}/src/algorithm.cpp
  ${PROJECT_SOURCE_DIR}/src/exception.cpp
  )

include_directories(
  "${PROJECT_SOURCE_DIR}/include"
)

# test_algorithm2

add_executable(
  test_algorithm2
  ../examples/example2/test_algorithm.cpp
  ../examples/example2/real.cpp
  ../examples/example2/real.h
  ${PROJECT_SOURCE_DIR}/src/algorithm.cpp
  ${PROJECT_SOURCE_DIR}/src/exception.cpp
  )

include_directories(
  "${PROJECT_SOURCE_DIR}/include"
)

target_link_libraries(test_algorithm2 "pthread")

# test_algorithm3

add_executable(
  test_algorithm3
  ../examples/example3/test_algorithm.cpp
  ../examples/example3/real.cpp
  ../examples/example3/real.h
  ../examples/example3/real.cl
  ${PROJECT_SOURCE_DIR}/src/algorithm.cpp
  ${PROJECT_SOURCE_DIR}/src/clalgorithm.cpp
  ${PROJECT_SOURCE_DIR}/src/context.cpp
  ${PROJECT_SOURCE_DIR}/src/exception.cpp
  )

include_directories(
  "${PROJECT_SOURCE_DIR}/include"
)

target_link_libraries(test_algorithm3 ${LIBS})

# test_matrix

include_directories(
  ${OPENCL_INCLUDE_DIRS}
  )

add_executable(
  test_matrix
  test_matrix.cpp
  ${PROJECT_SOURCE_DIR}/src/matrix.cpp
  ${PROJECT_SOURCE_DIR}/src/matrixnorm.cpp
  ${PROJECT_SOURCE_DIR}/src/context.cpp
  ${PROJECT_SOURCE_DIR}/src/algorithm.cpp
  ${PROJECT_SOURCE_DIR}/src/clalgorithm.cpp
  ${PROJECT_SOURCE_DIR}/src/exception.cpp
  )

target_link_libraries(test_matrix ${LIBS})

# test_file

add_executable(
  test_file
  test_file.cpp
  ${PROJECT_SOURCE_DIR}/src/matrix.cpp
  ${PROJECT_SOURCE_DIR}/src/file.cpp
  ${PROJECT_SOURCE_DIR}/src/jpegfile.cpp
  ${PROJECT_SOURCE_DIR}/src/image.cpp
  ${PROJECT_SOURCE_DIR}/src/algorithm.cpp
  ${PROJECT_SOURCE_DIR}/src/clalgorithm.cpp
  ${PROJECT_SOURCE_DIR}/src/context.cpp
  ${PROJECT_SOURCE_DIR}/src/exception.cpp
  ${PROJECT_SOURCE_DIR}/tests/sample_file.txt
  )

target_link_libraries(test_file "jpeg")
target_link_libraries(test_file ${LIBS})

# test_files

add_executable(
  test_files
  test_files.cpp
  ${PROJECT_SOURCE_DIR}/src/files.cpp
  ${PROJECT_SOURCE_DIR}/src/matrix.cpp
  ${PROJECT_SOURCE_DIR}/src/algorithm.cpp
  ${PROJECT_SOURCE_DIR}/src/clalgorithm.cpp
  ${PROJECT_SOURCE_DIR}/src/context.cpp
  ${PROJECT_SOURCE_DIR}/src/exception.cpp
  )

target_link_libraries(test_files ${LIBS})

# test_files

add_executable(
  test_sort
  test_sort.cpp
  ${PROJECT_SOURCE_DIR}/include/simplematrix.hpp
  ${PROJECT_SOURCE_DIR}/src/algorithm.cpp
  ${PROJECT_SOURCE_DIR}/src/expression.cpp
  ${PROJECT_SOURCE_DIR}/src/exception.cpp
  ${PROJECT_SOURCE_DIR}/include/sort.hpp
  )

target_link_libraries(test_sort ${LIBS})


#######
# tests

enable_testing()

# test_context
add_test( test_context ${EXECUTABLE_OUTPUT_PATH}/test_context "${CMAKE_CURRENT_SOURCE_DIR}" )

# test_algorithm
add_test( test_algorithm1 ${EXECUTABLE_OUTPUT_PATH}/test_algorithm1 "${CMAKE_CURRENT_SOURCE_DIR}" )

# test_algorithm2
add_test( test_algorithm2 ${EXECUTABLE_OUTPUT_PATH}/test_algorithm2 "${CMAKE_CURRENT_SOURCE_DIR}" )

# test_matrix
add_test( test_matrix ${EXECUTABLE_OUTPUT_PATH}/test_matrix "${PROJECT_SOURCE_DIR}/kernels" )

# test_file
add_test( test_file ${EXECUTABLE_OUTPUT_PATH}/test_file "${PROJECT_SOURCE_DIR}/data/photos" "${CMAKE_CURRENT_SOURCE_DIR}/sample_file.txt" )

# test_files
add_test( test_files ${EXECUTABLE_OUTPUT_PATH}/test_files )

# test_sort
add_test( test_sort ${EXECUTABLE_OUTPUT_PATH}/test_sort )
