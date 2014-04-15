/**
 *  @file   matrix_norm.cl
 *  @author Gabriel de Longeaux <gabriel.de_longeaux@telecom-sudparis.eu>
 *  @author Nicolas Granger <nicolas.granger@telecom-sudparis.eu>
 *  @brief  provides column normalization of a matrix
 */

// ##########################
// # matrix row normalization

// normalizes the values on each row

__kernel void
matrix_normalize_cols( __global float* R,
                       __global float* C,
                       __global float* A,
                       int h, int w ){
    // Thread index
    int ty = get_global_id(0);

    // thread variables
    float mu = 0.0f; // average of the column values
    float n_var = 0.0f; // total square distance to the average

    // Compute the column average;
    for (int k = 0; k < h; ++k) {
        // Calculation of the column average
        mu += A[k+ty*h];
    }

    mu = mu / (float)h;

    // Compute the column variance
    for (int k = 0; k < h; ++k) {
        n_var += pown( A[k+ty*h] - mu, 2 );
    }

    // Normalize the matrix
    n_var = n_var / (float)(h-1);
    float stdDev = sqrt(n_var);
    C[ty] = stdDev;
    for (int k = 0; k < h; ++k) {
        R[k+ty*h] = (A[k+ty*h] - mu) / stdDev;
    }
}

// #############################
// # matrix column normalization

// normalizes the values on each column

__kernel void
matrix_normalize_rows( __global float* R,
                       __global float* C,
                       __global float* A,
                       int h, int w){
    // Thread index
    int ty = get_global_id(0);

    // thread variables
    float mu = 0.0f; // average of the rows values
    float n_var = 0.0f; // total square distance to the average

    // Compute the row average;
    for ( int k = 0; k < w; ++k ) {
        // Calculation of the column average
        mu += A[k*h+ty];
    }

    mu = mu / (float)w;

    // Compute the column variance
    for ( int k = 0; k < w; ++k ) {
        n_var += pown( A[k*h+ty] - mu, 2 );
    }

    // Normalize the matrix
    float stdDev = sqrt(n_var);
    C[ty] = stdDev;
    for ( int k = 0; k < w; ++k ) {
        R[k*h+ty] = (A[k*h+ty] - mu) / stdDev;
    }
}
