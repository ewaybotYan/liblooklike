/**
 *  @file   matrix_norm.cl
 *  @author Gabriel de Longeaux <gabriel.de_longeaux@telecom-sudparis.eu>
 *  @brief  provides column normalization of a matrix
 */


#ifndef BLOCK_SIZE
#define BLOCK_SIZE 16
#endif

// #############################
// # matrix column normalization

// each kernel instance normalizes one column

// a matrix is stored in memory as one big 1D vector which is 
// the concatenation of the lines of the matrix

__kernel void
matrix_normalize(__global float* R, __global float* A, int n, int m)
{
    //Block size
    int block_size = BLOCK_SIZE;

    // Thread index
    int ty = get_local_id(1);

    // thread variables
    __local float mu = 0.0f; // average of the column values
    __local float n_var = 0.0f; // total square distance to the average
    
    // Compute the column average;
    for (int k = 0; k < m; ++k) {
        // Calculation of the column average
        mu += A[k * n + ty];
     }

     mu = mu / m;

     // Compute the column variance
     for (int k = 0; k < n; ++k) {
         n_var += ( A[k * n + ty] - mu )^2;
     }

     // Normalize the matrix
     for (int k = 0; k < n; ++k) {
        R[k * n + ty] = (A[k * n + ty] - mu) / sqrt(n_var);
     }
}
