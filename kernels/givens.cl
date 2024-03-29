/*__kernel void
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
*/

__kernel void
id(__global float* A2, __global float* A)
{

    // thread index
    int tx = get_local_id(0);

	A2[tx] = A[tx];
}

__kernel void
givens_rotation(__global float* A2, __global float* A, __global float* v, __global int i, __global int j, __global int n)
{

    // thread index
    int tx = get_local_id(0);
    int ty = get_local_id(1);

	__local float r = sqrt(v[i]*v[i]+v[j]*v[j]);
	__local float c = v[i]/r;
	__local float s = v[j]/r;

    id(A2,A);
	A2[i + n * tx] += A[i + n * tx] * c + A[j + n * tx] * sign(i-j) * s;
    A2[j + n * tx] += A[i + n * tx] * sign(j-i)*s + A[j + n * tx] * c;
}
