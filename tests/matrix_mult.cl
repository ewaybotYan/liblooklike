//La prise en compte des symétries semble être compliquée...
//J'ai l'impression que les i et j sont inversés.

#define BLOCK_SIZE 16

//////////////////////////////////////////////////////
//! Matrix multiplication on the device: C = A * B !//
//! wA is A's width and wB is B's width            !//
//////////////////////////////////////////////////////
__kernel void
matrixMult( __global float* C, __global float* A, __global float* B, 
           int wA, int wB)
{

    //Block size
    int block_size = BLOCK_SIZE;

    // Block index
    int bx = get_group_id(0);
    int by = get_group_id(1);

    // Thread index
    int tx = get_local_id(0);
    int ty = get_local_id(1);

    // Index of the first sub-matrix of A processed by the block
    int aStart = wA * block_size * by;

    // Index of the lsAt sub-matrix of A processed by the block
    int aEnd   = aStart + wA - 1;

    // Step size used to iterate through the sub-matrices of A
    int aStep  = block_size;

    // Index of the first sub-matrix of B processed by the block
    int bStart = block_size * bx;

    // Step size used to iterate through the sub-matrices of B
    int bStep  = block_size * wB;

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
        __local float sA[block_size][block_size];
 
        // Declaration of the local memory array sB 
        // used to store the sub-matrix of B
        __local float sB[block_size][block_size];

        // Load the matrices from device memory
        // to shared memory; each thread loads
        // one element of each matrix
        sA[ty + tx * block_size] = A[a + wA * ty + tx];
        sB[ty + tx * block_size] = B[b + wB * ty + tx];
	
        // Synchronize to make sure the matrices are loaded
        barrier(CLK_LOCAL_MEM_FENCE);

        // Multiply the two matrices together;
        // each thread computes one element
        // of the block sub-matrix        
        #pragma unroll
        for (int k = 0; k < block_size; ++k)
            Csub += sA[ty + k * block_size] * sB[k + tx * block_size];

        // Synchronize to make sure that the preceding
        // computation is done before loading two new
        // sub-matrices of A and B in the next iteration
        barrier(CLK_LOCAL_MEM_FENCE);
    }

    // Write the block sub-matrix to device memory;
    // each thread writes one element

    //int c = wB * block_size * by + block_size * bx;
    //C[c + wB * ty + tx] = Csub;

    C[get_global_id(1) * get_global_size(0) + get_global_id(0)] = Csub;

}

