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
             ../include/matrix.h
             ../src/matrix.cpp
             ../kernels/matrix_mult.cl
             ../kernels/matrix_norm.cl
             ../kernels/scalar_product.cl
             )

# target_link_libraries ( looklike ${Armadillo_LIBS} )

add_executable(img2eigvec
            img2eigvec.cpp
)

set(LIBS ${OPENCL_LIBRARIES} ${ARMADILLO_LIBRARIES} looklike "jpeg")

target_link_libraries( img2eigvec ${LIBS} )
