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
                       __global float* M,
                       __global float* A,
                       int h, int w ){
    // Thread index
    int ty = get_global_id(0);

    // thread variables
    float mu = 0.0f; // average of the column values
    float n_var = 0.0f; // total square distance to the average

    // Compute the column average;
    float err = 0.0f;
    for (int k = 0; k < h; ++k) {
        float val = A[k+ty*h] - err;
        float t = mu + val;
        err = t - mu - val;
        mu = t;
    }
    mu /= (float)h;

    // Compute the column variance
    err = 0;
    for (int k = 0; k < h; ++k) {
        float d = pown( A[k+ty*h] - mu, 2 ) - err;
        float t = n_var + d;
        err = t - n_var - d;
        n_var = t;
    }
    n_var /= (float)(h-1);

    // Normalize the matrix
    float stdDev = sqrt(n_var);
    C[ty] = stdDev;
    M[ty] = mu;
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
                       __global float* M,
                       __global float* A,
                       int h, int w){
    // Thread index
    int ty = get_global_id(0);

    // thread variables
    float mu = 0.0f; // average of the rows values
    float n_var = 0.0f; // total square distance to the average

    // Compute the row average;
    float err = 0.0f;
    for (int k = 0; k < w; ++k) {
        float val = A[ty+k*h] - err;
        float t = mu + val;
        err = t - mu - val;
        mu = t;
    }
    mu = mu / (float)w;

    // Compute the column variance
    err = 0;
    for (int k = 0; k < w; ++k) {
        float d = pown( A[ty+k*h] - mu, 2 ) - err;
        float t = n_var + d;
        err = t - n_var - d;
        n_var = t;
    }
    n_var /= (float)(w-1);

    // Normalize the matrix
    float stdDev = sqrt(n_var);
    C[ty] = stdDev;
    M[ty] = mu;
    for ( int k = 0; k < w; ++k ) {
        R[k*h+ty] = (A[k*h+ty] - mu) / stdDev;
    }
}


// #######################
// # inter-column distance

// computes the euclidian distance between columns of a matrix

__kernel void
matrix_distances( __global float* R,
                  __global float* A,
                  int h, int w){
    // Thread index
    int tx = get_global_id(0);
    int ty = get_global_id(1);
    // skip upper triangle of the matrix since matrix is symmetrical
    if( tx > ty ){
        R[tx*w+ty] = 0;
        return;
    }

    // thread variables
    float distance = 0;

    for( int k = 0; k<h; k++ )
        distance += pown( A[tx*h+k] - A[ty*h+k], 2 );

    R[tx*w+ty] = distance;
}
