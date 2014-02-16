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
                               int w, int depth )
{
    float tmp = 0;
    const int j = get_global_id(0);
    const int i = get_global_id(1);
    for( int k=0; k < depth; k++ ){
        tmp += A[i*depth+k] * B[k*w+j];
    }
    C[i*w+j]=tmp;
}


// computes T*t(T)
__kernel void
matrix_covariance ( __global float* C,
                    __global float* T,
                    int w, int h )
{
    float tmp = 0;
    const int i = get_global_id(0);
    const int j = get_global_id(1);
    for( int k=0; k < h; k++ ){
        tmp += T[k*w+i] * T[k*w+j];
    }
    //C[i*w+j] = tmp;
}


__kernel void
matrix_vector_multiplication ( __global float* C,
                               __global float* A,
                               __global float* B,
                               int depth )
{
    float tmp = 0;
    const int i = get_global_id(1);
    for( int k = 0; k < depth; k++ ){
        tmp += A[i*depth+k] * B[k];
    }
    C[i] = tmp;
}
