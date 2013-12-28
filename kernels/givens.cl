__kernel void
diag(__global float* D, int n)
{

    // thread index
    int tx = get_local_id(0);
    int ty = get_local_id(1);

    D[ty + n * tx] = 0;

    int i = 0;
    for (int k = 0; k + i < n; k+=n, i++){
        D[k + i] = 1;
    }
}


__kernel void
norm(__global float* G, __global float* v, __global float i, __global float j, __global float n)
{

    // thread index
    int tx = get_local_id(0);

	diag(G,n);

	__local float r = sqrt(v[i]*v[i]+v[j]*v[j]);
	__local float c = v[i]/r;
	__local float s = v[j]/r;

	G[i + (n-1) * i] = c;
	G[j + (n-1) * j] = c;
	G[j + (n-1) * i] = -sign(j-i)*s;
	G[i + (n-1) * j] = sign(j-i)*s;
}
