#ifndef BLOCK_SIZE
#define BLOCK_SIZE 1
#endif

//////////////////////////////////////////////////////
//! Matrix multiplication on the device: C = A * B !//
//! wA is A's width and wB is B's width            !//
//////////////////////////////////////////////////////
__kernel void
matrix_matrix_multiplication ( __global float* C,
                               __global float* A,
                               __global float* B,
                               int h, int depth )
{
    float tmp = 0;
    const int j = get_global_id(0);
    const int i = get_global_id(1);
    for( int k=0; k < depth; k++ ){
        tmp += A[i+k*h] * B[k+depth*j];
    }
    C[i+h*j] = tmp;
}


__kernel void
matrix_transpose_matrix_multiplication ( __global float* C,
                               __global float* A,
                               __global float* B,
                               int h, int depth )
{
    float tmp = 0;
    const int j = get_global_id(0);
    const int i = get_global_id(1);
    float err = 0;
    for( int k=0; k < depth; k++ ){
            float y =  A[k+i*depth] * B[k+depth*j] - err;
            float t = tmp + y;
            err = t - tmp - y;
            tmp = t;
        }
    C[i+h*j] = tmp;
}


//////////////////
/// Matrix scaling

/// Mulitply lines of a matrix by corresponding
/// coefficients taken from a vector
/// @param C result
/// @param A input matrix
/// @param V coefficients
__kernel void
matrix_matrix_scale ( __global float* C,
                      __global float* A,
                      __global float* M,
                      __global float* V,
                      int h)
{
    const int i = get_global_id(0);
    const int j = get_global_id(1);
    C[i+h*j] = (A[i+h*j]-M[i])/V[i];
}


//////////////////
// computes T*t(T)

/// @param C result
/// @param T input matrix
/// @param w T width
/// @param h T height
__kernel void
matrix_covariance ( __global float* C,
                    __global float* T,
                    int w, int h )
{
    float tmp = 0;
    const int i = get_global_id(0);
    const int j = get_global_id(1);
    float err = 0;
    for( int k=0; k < h; k++ ){
        float y =  T[i*h+k] * T[j*h+k] - err;
        float t = tmp + y;
        err = t - tmp - y;
        tmp = t;
    }
    C[i+w*j] = tmp / w;
}
