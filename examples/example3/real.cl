__kernel void real_real_sum( __global float* out,
                             __global float* a,
                             __global float* b ){
    out[0] = a[0] + b[0];
}

__kernel void real_real_mul( __global float* out,
                             __global float* a,
                             __global float* b ){
    out[0] = a[0] * b[0];
}
