set(LIBS ${OPENCL_LIBRARIES} ${ARMADILLO_LIBRARIES} jpeg pthread)

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
             ../include/eigenpair.hpp
             ../src/eigenpair.cpp
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
             ../include/matrixImportExport.h
             ../src/matrixImportExport.cpp
             ../include/image.h
             ../src/image.cpp
             ../include/jpegfile.h
             ../src/jpegfile.cpp
             )

include_directories( looklike ${ARMADILLO_INCLUDE_DIRS} ${OPENCL_INCLUDE_DIRS} "${PROJECT_SOURCE_DIR}/include" )

target_link_libraries( looklike ${LIBS} )

add_executable(img2eigvec
            img2eigvec.cpp
)

target_link_libraries( img2eigvec looklike )


add_executable(imgProjection
            imgprojection.cpp
)

target_link_libraries( imgProjection looklike )
