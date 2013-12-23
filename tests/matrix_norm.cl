
#define BLOCK_SIZE 16

//////////////////////////////////////////////////////
//! Matrix normalization on the device             !//
//! wA is A's width                                !//
//////////////////////////////////////////////////////
__kernel void
matrixNorm(__global float* A, int wA)
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
    int aStart = block_size * bx;

    // Index of the lsAt sub-matrix of A processed by the block
    int aEnd   = aStart + wA - 1;

    // Step size used to iterate through the sub-matrices of A
    int aStep  = block_size * wA;

    // Csub is used to store the element of the block sub-matrix
    // that is computed by the thread
    float* Csub = 0.0f;

    // Loop over all the sub-matrices of A and B
    // required to compute the block sub-matrix
    for (int a = aStart;
             a <= aEnd;
             a += aStep) {

        // Declaration of the local memory array sA 
        // used to store a column of A
        __local float Asub;
        __local float mu=0;
        __local float sigma=0;

        // Load the matrices from device memory
        // to shared memory; each thread loads
        // one element of each matrix
        sA[ty + tx * block_size] = A[a + wA * ty + tx];

        // Synchronize to make sure the matrice is loaded
        barrier(CLK_LOCAL_MEM_FENCE);

        // Compute the column average;
        // each thread computes one average
        // of the block sub-matrix        
        #pragma unroll
        for (int k = 0; k < block_size; ++k) {

            // Calculation of the column average
            mu += A[k + tx * block_size];
        }

        mu = mu / block_size;

        // Synchronize to make sure the average is calculated
        barrier(CLK_LOCAL_MEM_FENCE);

        // Compute the column variance;
        // each thread computes one variance
        // of the block sub-matrix        
        #pragma unroll
        for (int k = 0; k < block_size; ++k) {

            // Calculation of the column average
            sigma += (A[k + tx * block_size] - mu)^2;
        }

        sigma = sigma / block_size;

        // Synchronize to make sure the variance is calculated
        barrier(CLK_LOCAL_MEM_FENCE);

        // Normalize the matrix;
        // each thread computes one element
        // of the block sub-matrix        
        #pragma unroll
        for (int k = 0; k < block_size; ++k) {

            Asub = (A[k + tx * block_size] - sigma) / mu;
        }

        // Synchronize to make sure that the preceding
        // computation is done before loading a new
        // sub-matrice of A in the next iteration
        barrier(CLK_LOCAL_MEM_FENCE);
    }

    // Write the block sub-matrix to device memory;
    // each thread writes one element
    A[get_global_id(1) * get_global_size(0) + get_global_id(0)] = Asub;
}
