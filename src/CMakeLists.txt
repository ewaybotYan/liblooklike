set(LIBS ${OPENCL_LIBRARIES} ${ARMADILLO_LIBRARIES} "jpeg")

add_library (looklike
             ../include/context.h
             ../src/context.cpp
             ../include/exception.h
             ../src/exception.cpp
             ../include/file.h
             ../src/file.cpp
             ../include/image.h
             ../src/image.cpp
             ../include/jpegfile.h
             ../src/jpegfile.cpp
             ../include/algorithm.h
             ../src/algorithm.cpp
             ../include/clalgorithm.hpp
             ../src/clalgorithm.cpp
             ../include/matrix.h
             ../src/matrix.cpp
             ../kernels/matrix_mult.cl
             ../kernels/matrix_norm.cl
             ../kernels/scalar_product.cl
             )

include_directories( looklike ${ARMADILLO_INCLUDE_DIRS} ${OPENCL_INCLUDE_DIRS} "${PROJECT_SOURCE_DIR}/include" )

target_link_libraries( looklike ${ARMADILLO_LIBRARIES} )

add_executable(img2eigvec
            img2eigvec.cpp
)

target_link_libraries( img2eigvec ${LIBS} looklike )
