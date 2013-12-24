#pragma OPENCL EXTENSION cl_khr_byte_addressable_store : enable

__constant char hw[] = "Hello World\n";

__kernel void real_real_add(__global float* out, __global float* a, __global float* b){
  out[0] = a[0] + b[0];
}