__kernel void
prod(__global float* r, __global float* u, __global float* v, __global float n)
{

    // Thread index
    int tx = get_local_id(0);

	r[tx] = u[tx]*v[tx];
}

__kernel void
sum(__global float* result, __global float* r, __global float n)
{
	for (int i=0;i<n;i++){
		*result += r[i];
	}
}
