set(LIBS ${OPENCL_LIBRARIES} ${ARMADILLO_LIBRARIES} "jpeg")

add_library (looklike
             ../include/exception.h
             ../src/exception.cpp

             ../include/algorithm.h
             ../src/algorithm.cpp
             ../include/clalgorithm.hpp
             ../src/clalgorithm.cpp
             ../include/expression.hpp
             ../src/expression.cpp

             ../include/simplematrix.hpp
             ../include/clmatrix.hpp
             ../src/clmatrix.cpp
             ../include/clmatrixoperations.hpp
             ../src/clmatrixoperations.cpp
             ../include/clmatrixnorm.hpp
             ../src/clmatrixnorm.cpp
             ../include/sort.hpp

             ../include/context.h
             ../src/context.cpp

             ../kernels/matrix_mult.cl
             ../kernels/matrix_norm.cl
             ../kernels/scalar_product.cl

             ../include/file.h
             ../src/file.cpp
             ../include/files.hpp
             ../src/files.cpp
             ../include/image.h
             ../src/image.cpp
             ../include/jpegfile.h
             ../src/jpegfile.cpp
             )

include_directories( looklike ${ARMADILLO_INCLUDE_DIRS} ${OPENCL_INCLUDE_DIRS} "${PROJECT_SOURCE_DIR}/include" )

target_link_libraries( looklike ${ARMADILLO_LIBRARIES} )

add_executable(img2eigvec
            img2eigvec.cpp
)

target_link_libraries( img2eigvec ${LIBS} looklike )
