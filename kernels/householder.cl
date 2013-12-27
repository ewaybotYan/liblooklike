

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
    __global float* vect1; // comment indiquer la fin du vect?
    __global float* vect2; //pb de mémoire locale
    vect[tx] = (tx > k ? 1:0);
    v[tx] = u[tx] * vect[ty];

    vect2[tx] = (tx == k+1 ? 1:0);
    v = v + sign(u[k+1])*norm(v)*vect2;

}

__kernel void
outer_product(__global float* V, __global float* v, int n) //v*t(v)
{

    // thread index
    int tx = get_local_id(0);
    int ty = get_local_id(1);

    V[ty + (n-1) * tx] = v[tx]*v[ty];
}

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
submatrix(__global float* Ak, global float* A, int k, int m, int n)
{

    for (int i=0; k < n * (m-i); i++)
        Ak[i]=A[k + i * n];

}

__kernel void
P_k(__global float* P, __global float* A, int k, int n)
{

    // thread variables
    __global float* v; //pb avec mémoire locale?
    __global float* Ak;
    __global float* Dn;
    __global float* V;

    submatrix(Ak, A, int k, int m, int n);
    householder_symmetry_vector(v, Ak, int n, int k);
    diag (Dn, int n);
    outer_product(__global float* V, __global float* v, int n);
    P = Dn - (2/ norm(v)^2) * V;
}

__kernel void
diag(__global float* A, __global float* H, int n)
{

    __global float* P;
    __global float* tmp;
    diag(H,n);

    for (int k = 0; k < n; i++){
        P_k(P,A,k,n);
        matrix_mult(H,P,H);
        matrix_mult(tmp,A,P);
        matrix_mult(A,P,tmp);
    }
}
