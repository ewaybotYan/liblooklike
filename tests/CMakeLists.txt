###########################
# environment configuration

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/tests/bin)

file(
  GLOB
  SOURCES
  ${PROJECT_SOURCE_DIR}/src/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
  )

set(LIBS ${OPENCL_LIBRARIES} ${ARMADILLO_LIBRARIES} pthread)

#############
# executables

# test_context

include_directories(
  ${PROJECT_SOURCE_DIR}/include ${OPENCL_INCLUDE_DIRS}
)

message( STATUS "${PROJECT_SOURCE_DIR}/include/ ${OPENCL_INCLUDE_DIRS}" )

add_executable(
  test_context
  test_context.cpp
  ${PROJECT_SOURCE_DIR}/src/context.cpp
  ${PROJECT_SOURCE_DIR}/src/exception.cpp
  )

target_link_libraries(test_context ${LIBS})

# test_algorithm

# add_executable(
#   test_algorithm1
#   ../examples/example1/test_algorithm.cpp
#   ../examples/example1/real.cpp
#   ../examples/example1/real.h
#   ${PROJECT_SOURCE_DIR}/src/algorithm.cpp
#   ${PROJECT_SOURCE_DIR}/src/exception.cpp
#   )
# 
# include_directories(
#   "${PROJECT_SOURCE_DIR}/include"
# )
# 
# # test_algorithm2
# 
# add_executable(
#   test_algorithm2
#   ../examples/example2/test_algorithm.cpp
#   ../examples/example2/real.cpp
#   ../examples/example2/real.h
#   ${PROJECT_SOURCE_DIR}/src/algorithm.cpp
#   ${PROJECT_SOURCE_DIR}/src/exception.cpp
#   )
# 
# include_directories(
#   "${PROJECT_SOURCE_DIR}/include"
# )
# 
# target_link_libraries(test_algorithm2 "pthread")
# 
# # test_algorithm3
# 
# add_executable(
#   test_algorithm3
#   ../examples/example3/test_algorithm.cpp
#   ../examples/example3/real.cpp
#   ../examples/example3/real.h
#   ../examples/example3/real.cl
#   ${PROJECT_SOURCE_DIR}/src/algorithm.cpp
#   ${PROJECT_SOURCE_DIR}/src/clalgorithm.cpp
#   ${PROJECT_SOURCE_DIR}/src/context.cpp
#   ${PROJECT_SOURCE_DIR}/src/exception.cpp
#   )
# 
# include_directories(
#   "${PROJECT_SOURCE_DIR}/include"
# )
# 
# target_link_libraries(test_algorithm3 ${LIBS})

# test_clmatrix

add_executable(
  test_clmatrix
  test_clmatrix.cpp
  ${PROJECT_SOURCE_DIR}/include/simplematrix.hpp
  ${PROJECT_SOURCE_DIR}/src/clmatrix.cpp
  ${PROJECT_SOURCE_DIR}/src/context.cpp
  ${PROJECT_SOURCE_DIR}/src/algorithm.cpp
  ${PROJECT_SOURCE_DIR}/src/expression.cpp
  ${PROJECT_SOURCE_DIR}/src/clalgorithm.cpp
  ${PROJECT_SOURCE_DIR}/src/exception.cpp
  )

target_link_libraries(test_clmatrix ${LIBS})

# test_clmatrixoperations

add_executable(
  test_clmatrixoperations
  test_clmatrixoperations.cpp
  ${PROJECT_SOURCE_DIR}/src/clmatrix.cpp
  ${PROJECT_SOURCE_DIR}/src/clmatrixoperations.cpp
  ${PROJECT_SOURCE_DIR}/src/context.cpp
  ${PROJECT_SOURCE_DIR}/src/algorithm.cpp
  ${PROJECT_SOURCE_DIR}/src/expression.cpp
  ${PROJECT_SOURCE_DIR}/src/clalgorithm.cpp
  ${PROJECT_SOURCE_DIR}/src/exception.cpp
  )

target_link_libraries(test_clmatrixoperations ${LIBS})

# test_clmatrixnorm

add_executable(
  test_clmatrixnorm
  test_clmatrixnorm.cpp
  ${PROJECT_SOURCE_DIR}/src/clmatrix.cpp
  ${PROJECT_SOURCE_DIR}/src/clmatrixnorm.cpp
  ${PROJECT_SOURCE_DIR}/src/context.cpp
  ${PROJECT_SOURCE_DIR}/src/algorithm.cpp
  ${PROJECT_SOURCE_DIR}/src/expression.cpp
  ${PROJECT_SOURCE_DIR}/src/clalgorithm.cpp
  ${PROJECT_SOURCE_DIR}/src/exception.cpp
  )

target_link_libraries(test_clmatrixnorm ${LIBS})

# test_eigenpair

add_executable(
  test_eigenpair
  test_eigenpair.cpp
  ${PROJECT_SOURCE_DIR}/src/eigenpair.cpp
  ${PROJECT_SOURCE_DIR}/src/algorithm.cpp
  ${PROJECT_SOURCE_DIR}/src/expression.cpp
  ${PROJECT_SOURCE_DIR}/src/exception.cpp
  )

target_link_libraries(test_eigenpair ${LIBS})

# test_file

add_executable(
  test_file
  test_file.cpp
  ${PROJECT_SOURCE_DIR}/include/simplematrix.hpp
  ${PROJECT_SOURCE_DIR}/src/file.cpp
  ${PROJECT_SOURCE_DIR}/src/jpegfile.cpp
  ${PROJECT_SOURCE_DIR}/src/image.cpp
  ${PROJECT_SOURCE_DIR}/src/algorithm.cpp
  ${PROJECT_SOURCE_DIR}/src/expression.cpp
  ${PROJECT_SOURCE_DIR}/src/context.cpp
  ${PROJECT_SOURCE_DIR}/src/exception.cpp
  ${PROJECT_SOURCE_DIR}/tests/sample_file.txt
  )

target_link_libraries(test_file "jpeg")
target_link_libraries(test_file ${LIBS})

# test_matrixImportExport

add_executable(
   test_matrixImportExport
   test_matrixImportExport.cpp
   ${PROJECT_SOURCE_DIR}/src/matrixImportExport.cpp
   ${PROJECT_SOURCE_DIR}/src/algorithm.cpp
   ${PROJECT_SOURCE_DIR}/src/expression.cpp
   ${PROJECT_SOURCE_DIR}/include/simplematrix.hpp
   ${PROJECT_SOURCE_DIR}/src/context.cpp
   ${PROJECT_SOURCE_DIR}/src/exception.cpp
   )

 target_link_libraries(test_matrixImportExport ${LIBS})

# test_sort

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

# test_clmatrix
add_test( test_clmatrix ${EXECUTABLE_OUTPUT_PATH}/test_clmatrix )

# test_clmatrix
add_test( test_eigenpair ${EXECUTABLE_OUTPUT_PATH}/test_eigenpair )

# test_clmatrixoperations
add_test( test_clmatrixoperations ${EXECUTABLE_OUTPUT_PATH}/test_clmatrixoperations "${PROJECT_SOURCE_DIR}/kernels" )

# test_clmatrixnorm
add_test( test_clmatrixnorm ${EXECUTABLE_OUTPUT_PATH}/test_clmatrixnorm "${PROJECT_SOURCE_DIR}/kernels" )

# test_file
add_test( test_file ${EXECUTABLE_OUTPUT_PATH}/test_file "${IMAGES_SAMPLE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/sample_file.txt" )

# test_matrixImportExport
add_test( test_matrixImportExport ${EXECUTABLE_OUTPUT_PATH}/test_matrixImportExport )

# test_sort
add_test( test_sort ${EXECUTABLE_OUTPUT_PATH}/test_sort )
