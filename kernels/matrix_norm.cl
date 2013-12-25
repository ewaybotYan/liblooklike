
#define BLOCK_SIZE 16

//////////////////////////////////////////////////////
//! Matrix normalization on the device             !//
//! wA is A's width                                !//
//////////////////////////////////////////////////////
__kernel void
matrixNorm(__global float* A, int wA, int n int m)
{
    //Block size
    int block_size = BLOCK_SIZE;

    // Thread index
    int ty = get_local_id(1);

    // Declaration of the local memory array sA 
    // used to store a column of A
    __local float mu=0.0f;
    __local float sigma=0.0f;

    // Compute the column average;
    // each thread computes one average
    // of the block sub-matrix
    for (int k = 0; k < n; ++k) {

        // Calculation of the column average
        mu += A[k * m + ty];
     }

     mu = mu / n;

     // Compute the column variance;
     // each thread computes one variance
     // of the block sub-matrix
     for (int k = 0; k < n; ++k) {

         // Calculation of the column variance
         sigma += (A[k * m + ty] - mu)^2;
     }

     sigma = sigma / n;

     // Normalize the matrix;
     // each thread computes one element
     // of the block sub-matrix
     for (int k = 0; k < n; ++k) {

        A[k * m + ty] = (A[k * m + ty] - mu) / (sigma * sqrt(n));
     }
}
