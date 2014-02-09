#ifndef BLOCK_SIZE
#define BLOCK_SIZE 16
#endif

//////////////////////////////////////////////////////
//! Matrix multiplication on the device: C = A * B !//
//! wA is A's width and wB is B's width            !//
//////////////////////////////////////////////////////
__kernel void
matrix_matrix_multiplication ( __global float* C,
                               __global float* A,
                               __global float* B,
                               int wA, int wB )
{

    //Block size
    const int block_size = BLOCK_SIZE;

    // Block index
    int bx = get_group_id(0);
    int by = get_group_id(1);

    // Thread index
    int i = get_local_id(0);
    int j = get_local_id(1);

    // Index of the first sub-matrix of A processed by the block
    int aStart = wA * BLOCK_SIZE * by;

    // Index of the lsAt sub-matrix of A processed by the block
    int aEnd   = aStart + wA - 1;

    // Step size used to iterate through the sub-matrices of A
    int aStep  = BLOCK_SIZE;

    // Index of the first sub-matrix of B processed by the block
    int bStart = BLOCK_SIZE * bx;

    // Step size used to iterate through the sub-matrices of B
    int bStep  = BLOCK_SIZE * wB;

    // Csub is used to store the element of the block sub-matrix
    // that is computed by the thread
    float Csub = 0.0f;

    // Loop over all the sub-matrices of A and B
    // required to compute the block sub-matrix
    for (int a = aStart, b = bStart;
             a <= aEnd;
             a += aStep, b += bStep) {

        // Declaration of the local memory array sA
        // used to store the sub-matrix of A
        __local float sA[BLOCK_SIZE * BLOCK_SIZE];

        // Declaration of the local memory array sB
        // used to store the sub-matrix of B
        __local float sB[BLOCK_SIZE * BLOCK_SIZE];

        // Load the matrices from device memory
        // to shared memory; each thread loads
        // one element of each matrix
        sA[j + i * BLOCK_SIZE] = A[a + j + i * wA];
        sB[j + i * BLOCK_SIZE] = B[b + j + i * wB];

        // Synchronize to make sure the matrices are loaded
        barrier(CLK_LOCAL_MEM_FENCE);

        // Multiply the two matrices together;
        // each thread computes one element
        // of the block sub-matrix
        #pragma unroll
        for (int k = 0; k < BLOCK_SIZE; ++k)
            Csub += (sA[k + i * BLOCK_SIZE] * sB[j + k * BLOCK_SIZE]);

        // Synchronize to make sure that the preceding
        // computation is done before loading two new
        // sub-matrices of A and B in the next iteration
        barrier(CLK_LOCAL_MEM_FENCE);
    }

    // Write the block sub-matrix to device memory;
    // each thread writes one element
    C[get_global_id(1) * get_global_size(0) + get_global_id(0)] = Csub;

}
