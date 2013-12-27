householder_matrix <- function( A ){
	n <- sqrt(length(A))

	H <- diag(n)

	for (i in 1:(n-1)){
		P <- P_k(A,i)
		H <- P %*% H
		A <- P %*% A %*% P
	}

	return( list(A=A,H=H) )
}

__kernel void
norm(__global float norm, __global float* v)
{

    // thread index
    int tx = get_local_id(0);

    //initializing variables
    norm_temp=0.0f;

    // thread variables
    __local float norm;
    norm_temp += v[tx]^2;
    norm_temp = sqrt(norm_temp);

    norm = norm_temp; //?
}

__kernel void
householder_symmetry_vector(__global float* v, __global float* u, int n, int k)
{

    // thread index
    int tx = get_local_id(0);

    // thread variables
    __local float* vect1; // comment indiquer la fin du vect?
    __local float* vect2; //pb de mémoire locale
    vect[tx] = (tx > k ? 1:0);
    v[tx] = u[tx] * vect[ty];

    vect2[tx] = (tx == k+1 ? 1:0);
    v = v + sign(u[k+1])*norm(v)*vect2;

}

__kernel void
nomàdonner(__global float* V, __global float* v, int n) //v*t(v)
{

    // thread index
    int tx = get_local_id(0);
    int ty = get_local_id(1);

    __local float* temp; //pb de mémoire locale
    temp[ty + (n-1) * tx] = v[tx]*v[ty];
    V = temp;
}

__kernel void
diag(__global float* D, int n)
{

    // thread index
    int tx = get_local_id(0);
    int ty = get_local_id(1);

    __local float* tmp;
    tmp[ty + n * tx] = 0;

    int i = 0;
    for (int k = 0; k + i < n; k+=n, i++){
        tmp[k + i] = 1;
    }
    
    D= tmp;
}

P_k <- function( A, k ){
  n <- dim(A)[1]
  v = householder_symmetry_vector( A[,k], k )
  return( diag(n) - 2 * ( v %*% t(v) ) / ( norm(v)^2 ) )
}

__kernel void
P_k(__global float* P, __global float* A, int k, int n)
{

   // Thread index
    int ty = get_local_id(1);

    // thread variables
    __local float* v;
    Ak;
    householder_symmetry_vector(v, Ak, int n, int k);

    Dn;

}
